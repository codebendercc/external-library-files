/**
pfodCHAP for Arduino
 Parses commands of the form { cmd | arg1 ` arg2 ... } hashCode
 Arguments are separated by ` 
 The | and the args are optional
 This is a complete paser for ALL commands a pfodApp will send to a pfodDevice
 
 pfodCHAP is also used to send pfod messages to the pfodApp and add a SipHash code to the end
 of each messge so that the authenticity and correctness of the message can be verified.
 
 see www.pfod.com.au  for more details and example applications
 see http://www.forward.com.au/pfod/secureChallengeResponse/index.html for the details on the 
 128bit security and how to generate a secure secret password
 
  pfodCHAP adds about 2100 bytes to the program and uses about 400 bytes RAM
   and upto 19 bytes of EEPROM starting from the address passed to init()

 
The pfodCHAP parses messages of the form
 { cmd | arg1 ` arg2 ` ... } hashCode
 The hashCode is checked against the SipHash_2_4 using the 128bit secret key.
 The hash input is the message count, the challenge for this connection and the message
 If the hash fails the cmd is set to DisconnectNow (0xff)
 It is then the responsability of the calling program to close the connection.
 and to call pfodCHAP::disconnected() when the connection is closed.
 
 If the hash passes the message is parsed into the args array
  by replacing '|', '`' and '}' with '/0' (null)

  When the the end of message } is seen
  parse() returns the first byte of the cmd
  getCmd() returns a pointer to the null terminated cmd
  skipCmd() returns a pointer to the first arg (null terminated)
      or a pointer to null if there are no args
  getArgsCount() returns the number of args found.
These calls are valid until the start of the next msg { is parsed.
At which time they are reset to empty command and no args.

 (c)2013 Forward Computing and Control Pty. Ltd. 
 This code may be freely used for both private and commerical use.
 Provide this copyright is maintained.
*/

#include "pfodCHAP.h"
#include "pfodParser.h"
#include "SipHash_2_4.h"
#include "HexConversionUtils.h"
#include <string.h>

#define CR 13
#define NL 10

#define AUTHORIZATION_TIMEOUT 10000 
// if don't get auth msg within this time disconnect
// if don't get response to auth within this time disconnect default 10sec interval

#define IDLE_TIMEOUT 0 
// idle_timeout in mS, 0 == never timeout 
// call setIdleTimeout(60000) to set 60sec timeout

//#define DEBUG_AUTHORIZATION
//#define DEBUG_EEPROM_KEY_UPDATE
//#define DEBUG_TIMER
//#define DEBUG_PARSER
//#define DEBUG_ECHO_INPUT
// WARNING do not uncomment this define when connected to the network
// as it sends the expected response as well as the challenge
// ONLY use this via the serial monitor for debugging
//#define DEBUG_DISPLAY_RESPONSE 

#define NOT_AUTHORIZING 0
// set in disconnnected
#define AUTHORIZING_START (NOT_AUTHORIZING+1)
// set in connected or from loop if NOT_AUTHORIZING to go from disconnected to AUTHORIZING_START if not call to connected()
#define AUTHORIZING_SENT_CHALLENGE (AUTHORIZING_START+1)
#define AUTHORIZING_SUCCESS (AUTHORIZING_SENT_CHALLENGE+1)
#define AUTHORIZING_CMD '_'



pfodCHAP::pfodCHAP() {
	debugOut = NULL;
	idleTimeOut = IDLE_TIMEOUT;
}



/**
 * Set the idle Timeout in mS  i.e. 60000 ==> 60sec
 * Default is 0mS i.e. never timeout
 * When set to >0 then if no incoming msg received for this time (in mS), then the
 * parser will return DisconnectNow to disconnect the link
 *
 * Setting this to non-zero value protects against a hacker taking over your connection and just hanging on to it
 * not sending any msgs but not releasing it, so preventing you from re-connecting.
 * when setting to non-zero you can use the pfod re-request time to ask the pfodApp to re-request a menu every so often to prevent the connection timing out
 * while the pfodApp is running. See the pfod Specification for details.
 * 
 * Set to >0 to enable the timeout (recommended)
 */
void pfodCHAP::setIdleTimeout(unsigned long timeout) {
	idleTimeOut = timeout;
}	

/**
 * Call this before calling init() if you want the debug output
 * to go to some other Stream, other then the communication link
 *
 * debugOut is not written to unless you uncomment one of the #define DEBUG_ settings above
 */
void pfodCHAP::setDebugStream(Stream* out) {
	debugOut = out;
	mac.setDebugStream(debugOut);
}	

/**
 * initialize the CHAP parser
 * args
 * io_arg the Stream pointer to read and write to for pfod messages
 *
 * eepromAddress the starting address in eeprom to save the key and power cycles
 *    amount of eeprom used is (2 bytes power cycles + 1 byte key length + key bytes) ==> 3 + (input hexKey length)/2
 *
 * hexKey  pointer to program memory F("hexString") holding the key
 *  if this key is different from the current one in eeprom the eeprom is updated and the
 *  power cycles are reset to 0xffff
 *  if changing the key suggest you add 2 to your eepromAddress to move on from the previous
 *  one.  The power cycle EEPROM addresses are are written to on each power up 
 * 
 */
void pfodCHAP::init(Stream* io_arg, int eepromAddress, const __FlashStringHelper *hexKeyPgr) {
	// max key size for this code which uses SipHash is 128 bits (16 bytes) (32 hex digits) 
	initialization = true;
	noPassword = false;
	timeSinceLastConnection = 0;
	failedHash = false;
	msgHashCount = -1; // not yet
	inMsgCount = 0;
	outMsgCount = 0;
	outputParserState = pfodParser::pfodWaitingForStart;
	io = io_arg;
	if (debugOut == NULL) {
		debugOut = io;  // debugOut is not written to unless you uncomment one of the #define DEBUG_ settings above
	}	
	mac.setDebugStream(debugOut);
	char hexKey[mac.maxKeySize*2+2]; // max  32 hexDigits for hex key
	//const char PROGMEM *p = (const char PROGMEM *)hexKeyPgr;
	const char *p = (const char PROGMEM *)hexKeyPgr;
    size_t n = 0;
    while (1) {
		unsigned char c = pgm_read_byte(p++);
		hexKey[n++] =c;
		if (c == 0) {
			n--;
			break; // store terminator
		};
		if (n>=mac.maxKeySize*2) {
			hexKey[n] = 0; // limit to mac.maxKeySize*2 hexDigits
			break;
		}	
    }
    byte keyBytes[mac.maxKeySize];
	#ifdef DEBUG_EEPROM_KEY_UPDATE
       debugOut->print(F(" Input Key of length "));
       debugOut->print(n);
       debugOut->println();
	#endif
    int keyLen = asciiToHex(hexKey, keyBytes, mac.maxKeySize);
	#ifdef DEBUG_EEPROM_KEY_UPDATE
        debugOut->print(F(" New Key of length "));
        debugOut->print(keyLen);
        debugOut->print(F(" 0x"));
        mac.printBytesInHex(debugOut,keyBytes,keyLen);
        debugOut->println();
	#endif
    // now check if key has changed
    byte currentKeyBytes[mac.maxKeySize];
    int currentKeyLen = mac.readSecretKey(currentKeyBytes,eepromAddress+mac.KeyOffset);
	#ifdef DEBUG_EEPROM_KEY_UPDATE
        debugOut->print(F(" Current Key of length "));
        debugOut->print(currentKeyLen);
         debugOut->print(F(" 0x"));
         mac.printBytesInHex(debugOut,currentKeyBytes,currentKeyLen);
         debugOut->println();
	#endif
    boolean keysMatch = true;
    if (keyLen == currentKeyLen) {
    	// check bytes
    	for (int i=0; i<(int)keyLen; i++) {
    		if (keyBytes[i] != currentKeyBytes[i]) {
    			keysMatch = false;
    			break;
    		}	
    	} 	
	} else {
		keysMatch = false;
	}	
	if (keysMatch) {
	#ifdef DEBUG_EEPROM_KEY_UPDATE
        debugOut->println(F(" Keys match do NOT update key"));
	#endif
	} else {
	#ifdef DEBUG_EEPROM_KEY_UPDATE
        debugOut->println(F(" Keys do not match update to new key"));
	#endif
		setSecretKey(eepromAddress,keyBytes,keyLen);
	}	
	disconnected(); // start disconnected
	if (keyLen == 0) {
		noPassword = true;
		#ifdef DEBUG_EEPROM_KEY_UPDATE
			debugOut->println(F(" No Password"));
		#endif
	}	
	mac.init(debugOut,eepromAddress);
	lastMillis = 0; // holds the last read millis()
}

/**
 * Return the stream pfodCHAP is writing its output to
 * Don't write directly to this stream
 * unless forceing a disconnect
 */
Stream* pfodCHAP::getPfodAppStream() {
	return io;
}

/**
 * Implement the methods need for print
 */
size_t pfodCHAP::write(uint8_t b) {
	return writeToPfodApp((byte)b);
}

size_t pfodCHAP::write(const uint8_t *buffer, size_t size) {
	size_t rtn = 0;
	for (size_t i=0; i<size; i++) {
	   rtn += writeToPfodApp(buffer[i]);
	}
	return rtn;
}


/**
 * setSecretKey set the key and also sets the power cycles to 65535
 * eepromAddress is starting address len+3 bytes are used
*/
void pfodCHAP::setSecretKey(int eepromAddress, byte *key, int len) {
   int add = eepromAddress+mac.PowerCyclesOffset;
   for (int i=0;i<2;i++) {
     mac.eeprom_write(add++,(byte)0xff);
   }  
   add = eepromAddress+mac.KeyOffset;
   mac.eeprom_write(add++,len);
   byte* keyPtr = key;
   for (int i=0;i<len;i++) {
     mac.eeprom_write(add++,*keyPtr++);
   }  
} 




/**
* Return pointer to start of args[]
*/
byte* pfodCHAP::getCmd() {
	return parser.getCmd();
}

/**
* Return pointer to first arg (or pointer to null if no args)
*
* Start at args[0] and scan for first null
* if argsCount > 0 increment to point to  start of first arg
* else if argsCount == 0 leave pointing to null
*/
byte* pfodCHAP::getFirstArg() {
	return parser.getFirstArg();
}

/**
 * Return number of args in last parsed msg
 */ 
byte pfodCHAP::getArgsCount() {
	return parser.getArgsCount();
}	


/**
 * parse
 * Inputs:
 * byte in -- byte read from Serial port
 * Return:
 * return 0 if complete message not found yet
 * else return first char of cmd when see closing }
 * or ignore msg if pfodMaxCmdLen bytes after {
 * On non-zero return args[] contains 
 * the cmd null terminated followed by the args null terminated
 * argsCount is the number of args
 *
 * parses
 * { cmd | arg1 ` arg2 ... }
 * save the cmd in args[] replace | with null (\0)
 * then save arg1,arg2 etc in args[] replaceing ` with null
 * count of args saved in argCount 
 * on seeing } return first char of cmd 
 * if no } seen for pfodMaxCmdLen bytes after starting { then
 * ignore msg and start looking for { again
 *
 * States:
 * before {   parserState == pfodWaitingForStart
 * when   { seen parserState == pfodInMsg
 */   
 
 #ifdef DEBUG_TIMER
  unsigned long ms = millis();
 #endif
 
 void pfodCHAP::setAuthorizeState(int authState) {
 	 if (noPassword) {
 	 	 authorizing = AUTHORIZING_SUCCESS;
 	 } else {
 	 	authorizing = authState;
 	 }
 } 	 
 	 	 
 

/**
 * Call this AFTER the connection has been stopped by the main code.
 *  parser return 0xff when the connection should be stopped and will
 * not start parsing until this method is called
 * any bytes in the input buffer after 0xff is returned are dropped
 * any bytes recieved while not parsing are dropped
 */
void pfodCHAP::disconnected() {
	#ifdef DEBUG_AUTHORIZATION
	debugOut->println(F(" disconnected called"));
	#endif // DEBUG_AUTHORIZATION
	setAuthorizeState(NOT_AUTHORIZING);
	parser.init(); // clear disconnect not cmd
	parsing = true;
	connectionTimer = 0; // will start on first char recieved
	inMsgCount = 0;
	outMsgCount = 0;
	msgHashCount = -1; // stop collecting hash
	outputParserState = pfodParser::pfodWaitingForStart;
}

/**
 * Call this AFTER the connection has been established
 *  parser return 0xff when the connection should be stopped and will
 * not start parsing until this method is called
 * any bytes in the input buffer after 0xff is returned are dropped
 * any bytes recieved while not parsing are dropped
 */
void pfodCHAP::connected() {
	setAuthorizeState(AUTHORIZING_START);
	parser.init(); // clear disconnect not cmd
	parsing = true;
	inMsgCount = 0;
	outMsgCount = 0;
	outputParserState = pfodParser::pfodWaitingForStart;
	#ifdef DEBUG_AUTHORIZATION
		debugOut->println(F(" initialize authorization timer"));
	#endif // DEBUG_AUTHORIZATION
	connectionTimer = AUTHORIZATION_TIMEOUT;
}


/**
* Write bytes to serial out until reach null '/0'
* Inputs:
*  idxPtr - byte* pointer to start of bytes to write
* return
*/
size_t pfodCHAP::writeToPfodApp(uint8_t* idxPtr) {
	size_t rtn = 0;
	while ( *idxPtr != 0) {
		byte b = *idxPtr;
		rtn += writeToPfodApp(b);
		++idxPtr;
	}
	return rtn;
}

/**
*  write a byte to the connected stream
*  If the byte is part of a { } msg then add it to the hash
*  and write the hash after the final }
*  Raw data, i.e. data outside { } is just passed through
*/
size_t pfodCHAP::writeToPfodApp(uint8_t b) {
	size_t rtn = 0;
	if (authorizing != AUTHORIZING_SUCCESS) {
		// have not completed authorization yet so just consume all output
		// and tell the program that the byte has been handled
		return 1; 
	}
	if (noPassword) {
		// skip hash just output the bytes
		rtn =  io->write(b);
		#ifdef DEBUG_DISPLAY_RESPONSE
			debugOut->write(b);
		#endif
		return rtn;
	}	
	// have password
	if (outputParserState == pfodParser::pfodWaitingForStart) {
		if (b ==  pfodParser::pfodMsgStarted) {
			outputParserState = pfodParser::pfodMsgStarted;
		} else {
			// skip as byte is outside { }
		}
	} else if (outputParserState == pfodParser::pfodMsgStarted) {
	// {} Empty  {@ Language {. Menu {: Update menu/navigation items {^ Navigation input {= Streaming raw data
	// {' String input {# Numeric input {? Single Selection input {* Multiple
		if ((b == '}') || (b=='@') || (b=='.') || (b==':') || (b=='^') || (b=='=') || (b=='\'') // '
			|| (b=='#') || (b=='?') || (b=='*')) {
			// this is real command
			// initial hash
			#ifdef DEBUG_DISPLAY_RESPONSE
				debugOut->println(F(" init output hash"));
			#endif
			mac.initHash();
			mac.putByteToHash('{');
			outputParserState = pfodParser::pfodInMsg; // in msg
			if (b == '}') {
				outputParserState = pfodParser::pfodMsgEnd; //empty msg
			}
		} else if (b=='{') {
			// {{ go back to pfodMsgStarted
			outputParserState = pfodParser::pfodMsgStarted; // finished
		} else {
			// not a real command just raw data
			outputParserState = pfodParser::pfodWaitingForStart; // finished
		}
	} else if (outputParserState == pfodParser::pfodInMsg) {
		if (b == pfodParser::pfodMsgEnd) {
			// end of msg
			outputParserState = pfodParser::pfodMsgEnd;
		}
	} else {
		// ignore bytes if not pfodInMsg
	}
	// write it
	while (!rtn) {
		// loop here until write succeeds
	   rtn = io->write(b);
	}   
	#ifdef DEBUG_DISPLAY_RESPONSE
		debugOut->write(b);
	#endif
	if ((outputParserState == pfodParser::pfodInMsg) || (outputParserState == pfodParser::pfodMsgEnd)) {
		// add it to the hash
	  mac.putByteToHash(b);
	}
	if (outputParserState == pfodParser::pfodMsgEnd) {
	  outputParserState = pfodParser::pfodWaitingForStart; // finished
	  // create hash and send
	  #ifdef DEBUG_DISPLAY_RESPONSE
		debugOut->println(F(" send hash"));
		debugOut->print(F(" out msg Count "));
		debugOut->println(outMsgCount);
	  #endif	
	   mac.putLongToHash(outMsgCount);
	   outMsgCount++; // increment for next msg out
	   for (int i=0; i<pfodMAC::challengeByteSize; i++) {
		   mac.putByteToHash(challenge[i]);
	   }
	   mac.finishHash();
	   #ifdef DEBUG_DISPLAY_RESPONSE
			debugOut->println(F(" add hashResult"));
	   #endif
		// convert to hex
		hexToAscii((const unsigned char *)mac.getHashedResult(), mac.msgHashByteSize, (char*)msgHashBytes,  Msg_Hash_Size+1);
		for (int i=0; i<Msg_Hash_Size; ) {
			  i += io->write(msgHashBytes[i]); 
			  // note if output is full may lose byte here and hash at the other end will fail
			  // so increment i from return from write so will loop here until write of all bytes is successful
		}
	}	  
	return rtn;
}

/**
 * parseLong
 * will parse between  -2,147,483,648 to 2,147,483,647
 * No error checking done.  
 * will return 0 for empty string, i.e. first byte == null
 *
 * Inputs:
 *  idxPtr - byte* pointer to start of bytes to parse
 *  result - long* pointer to where to store result
 * return
 *   byte* updated pointer to bytes after skipping terminator
 *
 */
byte* pfodCHAP::parseLong(byte* idxPtr, long *result) {
	return parser.parseLong( idxPtr, result);
}


byte pfodCHAP::parse() {
  unsigned long deltaMillis = 0; // clear last result
  unsigned long thisMillis = millis(); // do this just once to prevent getting different answers from multiple calls to millis()
  	  
  if (thisMillis != lastMillis) {
    // we have ticked over
    // calculate how many millis have gone past
    deltaMillis = thisMillis-lastMillis;  // note this works even if millis() has rolled over back to 0
    lastMillis = thisMillis;
  } 
  
  timeSinceLastConnection += deltaMillis;
  if (timeSinceLastConnection > MAX_ELAPSED_TIME) {
  	  // limit it
  	  timeSinceLastConnection = MAX_ELAPSED_TIME;
  }	  
  	    
 #ifdef DEBUG_TIMER
  if (millis() > ms) {
  	  debugOut->print(F("Connection Timer:"));
  	 debugOut->println(connectionTimer);
	 ms = millis()+1000;
   }	
 #endif  
   byte	cmd = 0;
   byte in = 0;
  if (initialization) {  // first loop only
     cmd = DisconnectNow;
     parsing = false;
     initialization = false;
  }   
    
  // else not in getPasswordMode
   if ((!parsing) || (!mac.isValid())) {
   	   while (io->available()) {
   	   	   in = io->read();
   	   	   #ifdef DEBUG_ECHO_INPUT
   	   	       	if ((in == NL) || (in == CR)) {
   	   	       		debugOut->println();
   	   	       	} else {	
   	   	       		debugOut->print((char)in);
   	   	       	}	
   	   	   #endif
   	   }
   	   cmd = DisconnectNow;
   } else { 	
		if ((deltaMillis != 0) && (connectionTimer >0)) {
			connectionTimer -= deltaMillis;
			if (connectionTimer <=0) {
				#ifdef DEBUG_PARSER
				debugOut->print(F(" timer timed out"));
				#endif
				cmd = DisconnectNow;
			}
		}	
   	   // parsing
   	   if (cmd != DisconnectNow) {
		   if (io->available()) {
				in = io->read();
   	   	   #ifdef DEBUG_ECHO_INPUT					
				debugOut->print(F(" read "));
				debugOut->println((char)in);
            #endif
				if (msgHashCount >=0) {
					msgHashBytes[msgHashCount++] = in;
					#ifdef DEBUG_DISPLAY_RESPONSE
						debugOut->print(F(" hashHex:"));
						debugOut->println((char)in);
                   #endif
					if (msgHashCount == Msg_Hash_Size) {
						msgHashBytes[Msg_Hash_Size] = 0; //terminate
						// check hash
						msgHashCount = -1;
						#ifdef DEBUG_DISPLAY_RESPONSE
							debugOut->print(F(" checkMsgHash: inMsgCount:"));
							debugOut->print(inMsgCount);
							debugOut->print(" ");
							mac.printBytesInHex(debugOut,mac.getHashedResult(),Msg_Hash_Size_Bytes);
							debugOut->println();
							debugOut->println((char*)msgHashBytes);
			           #endif
						if (mac.checkMsgHash(msgHashBytes,Msg_Hash_Size_Bytes) ) {
			            #ifdef DEBUG_ECHO_INPUT					
								debugOut->println((char)in);
				        #endif
							cmd = *parser.getCmd(); // reset cmd from parser
							inMsgCount++;
						} else {
	   	   	   #ifdef DEBUG_ECHO_INPUT					
								debugOut->println(F("Hash failed "));
	            #endif
							failedHash = true;
							cmd = DisconnectNow;
						}	
					}	
				} else {	
					// if not completed authorization and  authorization timer zero start it now
					if ((authorizing != AUTHORIZING_SUCCESS) && (connectionTimer == 0)) {
						#ifdef DEBUG_TIMER
							debugOut->println(F(" reset connection timer because == 0 and not success"));
						#endif
						connectionTimer = AUTHORIZATION_TIMEOUT;
					}
					if (authorizing == NOT_AUTHORIZING) {
						setAuthorizeState(AUTHORIZING_START);
					}	
					#ifdef DEBUG_ECHO_INPUT
						if ((in == NL) || (in == CR)) {
							debugOut->println();
						} else {	
							debugOut->print((char)in);
						}	
					#endif 
					cmd = parser.parse(in);
					if (authorizing == AUTHORIZING_SUCCESS) { 
					 #ifdef	DEBUG_AUTHORIZATION
					   debugOut->println(F(" AUTHORIZING_SUCCESS"));
                     #endif
						if (noPassword) {
							// skip this
						} else {	
							#ifdef	DEBUG_AUTHORIZATION
							 debugOut->println(F(" with password"));
							#endif 
							if (parser.getParserState() == parser.pfodMsgStarted) {
								// initial hash
								#ifdef	DEBUG_AUTHORIZATION
									debugOut->println(F(" init hash"));
								#endif	
								mac.initHash();
								mac.putByteToHash(parser.pfodMsgStarted);
							} else if (parser.getParserState() == parser.pfodInMsg) {
								mac.putByteToHash(in);
							} else if (parser.getParserState() == parser.pfodMsgEnd) {
								if ((cmd == AUTHORIZING_CMD) && (strlen((const char*)parser.getCmd())==1) && (parser.getArgsCount() == 0)) {
									 if (authorizing == AUTHORIZING_SENT_CHALLENGE) {
                        // pfodApp trying to start a new connection but we think we are have already started one 
                        // i.e. got {_} and send challenge and are waiting for response
                        // note real response will NOT have zero args
                        // so other side just sending {_} followed by {_}
                        // just disconnnect now to resync
											#ifdef	DEBUG_AUTHORIZATION
														debugOut->println(F(" got {_} response to AUTHORIZING_SENT_CHALLENGE -- disconnect."));
											#endif            
											cmd = DisconnectNow;
									 } else {
									 	 // got through challenge and response other side just disconnected and reconnected
											// else call disconnect /connect
											disconnected();
											connected(); // set authorizing to AUTHORIZING_START
											parser.setCmd(AUTHORIZING_START); // connected initializes parser  
											// Note NO hash for {_} msg
									 }	
                 } else {   
			   					 #ifdef DEBUG_DISPLAY_RESPONSE
			   					 	debugOut->println(F(" clear cmd"));
							     #endif
									mac.putByteToHash(in);
									cmd = 0; // clear cmd
									msgHashCount = 0;
  								    #ifdef DEBUG_DISPLAY_RESPONSE
									   debugOut->print(F(" msg Count "));
									   debugOut->println(inMsgCount);
								    #endif
								   mac.putLongToHash(inMsgCount);
								   for (int i=0; i<pfodMAC::challengeByteSize; i++) {
									mac.putByteToHash(challenge[i]);
								   }  
								   mac.finishHash();
								}	   
							} else {
								// ignore
							}	
	
						}
					}	
				}	
		   }
	   }   
	} // end  if (!parsing)

	if (cmd != 0) {
		if (cmd == DisconnectNow) {
			#ifdef DEBUG_PARSER
				debugOut->println(F(" cmd == DisconnectNow "));
			#endif
		} else {
			#ifdef DEBUG_PARSER
				debugOut->print(F(" found cmd "));
				debugOut->println((char)cmd);
			#endif
		}	
    }
    
	if ((cmd != DisconnectNow)  && (cmd != 0) ){
		  if (authorizing == AUTHORIZING_START) { // will not get this state if noPassword see setAuthorizeState()
					if ((cmd == AUTHORIZING_CMD) && (strlen((const char*)parser.getCmd())==1) && (parser.getArgsCount() == 0)) {
							// send challenge
							setAuthorizeState(AUTHORIZING_SENT_CHALLENGE);
							connectionTimer = AUTHORIZATION_TIMEOUT; // reset timer
							// get challenge
							long mS = (long)timeSinceLastConnection;
							if (failedHash) {
								mS = -mS;
							}	
							 mac.buildChallenge(challenge, mS);
							 // start again
							timeSinceLastConnection = 0;
							 failedHash = false;
							 // add the hash type 
							 challenge[pfodMAC::challengeByteSize] = 2;
							 // send challenge
							 io->print(F("{"));
							 io->print(AUTHORIZING_CMD);
							 mac.printBytesInHex(io, challenge, pfodMAC::challengeByteSize+1);
							 io->print(F("}"));
							 #ifdef DEBUG_DISPLAY_RESPONSE
							     debugOut->println(F("hashed result"));
								 mac.printBytesInHex(debugOut, mac.getHashedResult(), pfodMAC::challengeByteSize);
							     debugOut->println();
							 #endif
						 cmd = 0; // finished with this command
					} else {
						// command not correct drop connection
						#ifdef DEBUG_PARSER
							debugOut->println(F(" challenge request missing"));
						#endif
						failedHash = true;
						cmd = DisconnectNow;
					}
		  } else if (authorizing == AUTHORIZING_SENT_CHALLENGE) {
			// looking for = cmd
				if ((cmd == AUTHORIZING_CMD) && (strlen((const char*)parser.getCmd())==1) && (parser.getArgsCount() == 1)) {
					// check response
					if (mac.checkMsgHash((const byte*)parser.getFirstArg(),pfodMAC::challengeHashByteSize)) {
							cmd = 0; // 
							setAuthorizeState(AUTHORIZING_SUCCESS);
							connectionTimer = idleTimeOut; // clear timer and set idle timeout
							#ifdef DEBUG_PARSER
								debugOut->println(F(" response matched, initialized idle timout"));
							#endif
							io->print(F("{"));
					        io->print(AUTHORIZING_CMD);
					        io->print(F("}"));
							 cmd = 0; // finished with this command
					 } else {
							#ifdef DEBUG_PARSER
								debugOut->println(F(" response did not match"));
							#endif
							failedHash = true;
							cmd = DisconnectNow;
					}                	
				} else { // not = or not one arg	
						cmd = DisconnectNow;
				}
		} else if (authorizing == AUTHORIZING_SUCCESS) {
			// either no password OR finished auth
				if ((cmd == AUTHORIZING_CMD) && (strlen((const char*)parser.getCmd())==1) && (parser.getArgsCount() == 0)) {
					// got {_}
					if (noPassword) {
						#ifdef DEBUG_AUTHORIZATION
							debugOut->println(F(" no password just send back {}"));
						#endif
						//no password just send back {}
						io->print(F("{}"));
						cmd = 0;
						parser.setCmd(0);
					} else {
						// have password should not get this
						#ifdef DEBUG_AUTHORIZATION
							debugOut->println(F(" Authorized with password but got {_}"));
						#endif
						failedHash = true;
						cmd = DisconnectNow;
					}	
				}	// else not {_}
		} 
    }
    
    if (cmd == DisconnectNow) {
    	parsing = false;
    	msgHashCount = -1;
    	setAuthorizeState(NOT_AUTHORIZING);
    	#ifdef DEBUG_TIMER
			debugOut->println(F(" reset connection timer because cmd set to DisconnectNow"));
		#endif 
    	parser.setCmd(DisconnectNow);
    	connectionTimer = 0; //stop timer
    } else {
    	if (cmd != 0) {
    		if (cmd ==AUTHORIZING_CMD) {
    			#ifdef DEBUG_AUTHORIZATION
					debugOut->println();
					debugOut->println(F(" found cmd {=  disconnect"));
				#endif
    			cmd = DisconnectNow; 
    		} else if ((authorizing == AUTHORIZING_SUCCESS) && (cmd != 0)) {
				// got another command reset idle timeout
				#ifdef DEBUG_TIMER
					debugOut->println(F(" reset connection timer"));
				#endif
				connectionTimer = idleTimeOut; // clear timer and set idle timeout
				
				// read the next 8 bytes for hash
	    #ifdef DEBUG_PARSER
				debugOut->println();
				debugOut->print(F(" found cmd "));
				debugOut->println((char)cmd);
		#endif
			}
   		}
   	}	
		#ifdef DEBUG_PARSER
			if (cmd != 0) {
				debugOut->print(F(" found cmd "));
				if (cmd != DisconnectNow) {
					debugOut->println((char)cmd);
				} else {
					debugOut->println(F(" DisconnectNow"));
				}    		
			}
		#endif
	return cmd;
}



