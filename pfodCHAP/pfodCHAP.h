#ifndef pfodCHAP_h
#define pfodCHAP_h
/**
pfodCHAP for Arduino
 Parses commands of the form { cmd | arg1 ` arg2 ... } hashCode
 Arguments are separated by ` 
 The | and the args are optional
 This is a complete paser for ALL commands a pfodApp will send to a pfodDevice
 see www.pfod.com.au  for more details.
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

#include "Arduino.h"
#include "pfodParser.h"
#include "Stream.h"
#include "pfodMAC.h"

class pfodCHAP : public Print{
public:	
pfodCHAP();

// methods required for Print
size_t write(uint8_t);
size_t write(const uint8_t *buffer, size_t size);


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
void setIdleTimeout(unsigned long timeout);

void setDebugStream(Stream* debugOut);


/**
 * initialize the CHAP parser
 * args
 * io_arg the Stream pointer to read and write to for pfod messages
 *
 * eepromAddress the starting address in eeprom to save the key and power cycles
 *    amount of eeprom used is (2 bytes power cycles + 1 byte key length + key bytes) ==> 3 + (input hexKey length)/2, 
 *
 * hexKey  pointer to program memory F("hexString") holding the key
 *  if this key is different from the current one in eeprom the eeprom is updated and the
 *  power cycles are reset to 0xffff
 *  if changing the key suggest you add 2 to your eepromAddress to move on from the previous
 *  one.  The power cycle EEPROM addresses are are written to on each power up 
 * 
 */
void init(Stream* io_arg, int eepromAddress, const __FlashStringHelper *hexKey);

Stream* getPfodAppStream();

void disconnected(); // call this when the main code has disconnected the link
void connected(); // call this when connected, if the main program can tell a connection has occured. Not essential
static const byte DisconnectNow = 0xff; // this is returned if pfodDevice should drop the connection 

byte parse(); // call this in loop() every loop, it will read bytes, if any, from the pfodAppStream and parse them
// returns 0 if message not complete, else returns the first char of a completed and verified message

byte* getCmd();  // pointer to current parsed command, points to /0 if no command, 0xff if DisconnectNow was returned from the parser
byte* getFirstArg(); // pointer to the first command argument, points to /0 if there are no arguments
byte getArgsCount(); // number of arguments in the current command, 0 if none
byte* parseLong(byte* idxPtr, long *result); // parse the argument, pointed to by idxPtr, as a long


private:
size_t writeToPfodApp(uint8_t* idxPtr);
size_t writeToPfodApp(uint8_t b);
Stream *io;
Stream *debugOut;
pfodParser parser;
pfodMAC mac;
boolean parsing; // true when parsing, after disconnected() called and before returning 0xff, false between returning 0xff and disconnected being called
byte authorizing;  
byte challenge[pfodMAC::challengeByteSize]; 
unsigned long lastMillis; // holds the last read millis()
unsigned long timeSinceLastConnection; // limited to 0x7fffffff
boolean failedHash; // true if any hash check failed last connection
long connectionTimer; 
void setSecretKey(int eepromAddress, byte *key, int len);
boolean noPassword;
void setAuthorizeState(int auth);
int msgHashCount;
static const byte Msg_Hash_Size = 8; // number of hex digits for msg hash 
static const byte Msg_Hash_Size_Bytes = (Msg_Hash_Size>>1); // number of hex bytes for msg hash i.e. 4 
byte msgHashBytes[Msg_Hash_Size+1]; // allow for null outgoing
unsigned long inMsgCount;
unsigned long outMsgCount;
byte outputParserState;
boolean initialization;
unsigned long idleTimeOut;

};

#endif // pfodCHAP_h

	