/**
 pfodParser for Arduino
The pfodParser parses messages of the form
 { cmd ` arg1 ` arg2 ` ... }
 or
 { cmd ~ arg1}
 or 
 { cmd }
 The args are optional
 This is a complete paser for ALL commands a pfodApp will send to a pfodDevice
 see www.pfod.com.au  for more details.
 
  pfodParser adds about 482 bytes to the program and uses about 260 bytes RAM
 
The pfodParser parses messages of the form
 { cmd ` arg1 ` arg2 ` ... }
 or
 { cmd ~ arg1}
 or 
 { cmd }
The message is parsed into the args array by replacing '|', '`' and '}' with '/0' (null)
When the the end of message } is seen
  parse() returns the first byte of the cmd
  getCmd() returns a pointer to the null terminated cmd
  skipCmd() returns a pointer to the first arg (null terminated)
      or a pointer to null if there are no args
  getArgsCount() returns the number of args found.
These calls are valid until the start of the next msg { is parsed.
At which time they are reset to empty command and no args.

 (c)2012 Forward Computing and Control Pty. Ltd. 
 This code may be freely used for both private and commerical use.
 Provide this copyright is maintained.
*/

#include "pfodParser.h"

pfodParser::pfodParser() {
  init();
}

void pfodParser::init() {
  argsCount = 0;
  argsIdx = 0;
  args[0] = 0; // no cmd yet
  args[1] = 0; //
  parserState = pfodWaitingForStart; // not started yet pfodInMsg when have seen {
}

void pfodParser::setCmd(byte cmd) {
	init();
	args[0] = cmd;
	args[1] = 0;
}

/**
 * Return pointer to start of args[] 
 */ 
byte* pfodParser::getCmd() {
  return args;
}	

/**
 * Return pointer to first arg (or pointer to null if no args)
 *
 * Start at args[0] and scan for first null
 * if argsCount > 0 increment to point to  start of first arg
 * else if argsCount == 0 leave pointing to null
 */ 
byte* pfodParser::getFirstArg() {
  byte* idxPtr = args;
  while( *idxPtr != 0) {
    ++idxPtr;
  }
  if (argsCount > 0) {
    ++idxPtr;
  }
  return idxPtr;
}  

/**
 * Return number of args in last parsed msg
 */ 
byte pfodParser::getArgsCount() {
  return argsCount;
}	

/**
 * pfodWaitingForStart if outside msg
 * pfodMsgStarted if just seen opening {
 * pfodInMsg in msg after {
 * prodEndMsg if just seen closing }
 */
byte pfodParser::getParserState() {
	if ((parserState == pfodWaitingForStart) || 
	   (parserState == pfodMsgStarted) || 
	   (parserState == pfodInMsg) || 
	   (parserState == pfodMsgEnd) ) {
			return parserState;
	} //else {
	return pfodInMsg;
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
 * { cmd ` arg1 ` arg2 ... }
 * { cmd ~ arg1 ~ arg2 ... }
 * save the cmd in args[] replace |, ~ and ` with null (\0)
 * then save arg1,arg2 etc in args[] 
 * count of args saved in argCount 
 * on seeing } return first char of cmd 
 * if no } seen for pfodMaxCmdLen bytes after starting { then
 * ignore msg and start looking for { again
 *
 * States:
 * before {   parserState == pfodWaitingForStart
 * when   { seen parserState == pfodInMsg
 */   
byte pfodParser::parse(byte in) {
	
  if ((parserState == pfodWaitingForStart) || (parserState == pfodMsgEnd)) {
  	  parserState = pfodWaitingForStart;
    if (in == pfodMsgStarted) { // found {
       init(); // clean out last cmd
       parserState = pfodMsgStarted;
    } 
    // else ignore this char as waiting for start {
    // always reset counter if waiting for {
    return 0;
  }    
  
  // else have seen { 
  if ((argsIdx >= (pfodMaxMsgLen-2)) &&
        (in != pfodMsgEnd)) {
    // ignore this msg and reset
    // should not happen as pfodApp should limit
    // msgs sent to pfodDevice to <=255 bytes
    init();
    return 0;
  }
  // first char after opening {
  if (parserState == pfodMsgStarted) {
  	  parserState = pfodInMsg;
  }	  
  
  // else process this msg char
  // look for special chars | ' }
  if ((in == pfodMsgEnd) || (in == pfodBar) || (in == pfodTilda) || (in == pfodAccent)) {
    args[argsIdx++] = 0;
    if (parserState == pfodArgStarted) {
    	// increase count if was parsing arg    
        argsCount++; 
    }
    if (in == pfodMsgEnd) {
      parserState = pfodMsgEnd; // reset state
      // return command byte found
      // this will return 0 when parsing {} msg
      return args[0];
    } else {
    	parserState = pfodArgStarted;
    }	
    return 0;   
  }
  // else normal byte
  args[argsIdx++] = in;
  return 0;
}

/**
 * Write bytes to serial out until reach null '/0'
 * Inputs:
 *  idxPtr - byte* pointer to start of bytes to write
 * return
 *   byte* updated pointer to bytes after skipping terminator
 */
byte* pfodParser::writeToSerial(Stream* out, byte* idxPtr) {
  while ( *idxPtr != 0) {
      out->write(*idxPtr);
      ++idxPtr;
  }  
  return ++idxPtr; // skip null
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
byte* pfodParser::parseLong(byte* idxPtr, long *result) {
  long rtn = 0;
  boolean neg = false;
  while ( *idxPtr != 0) {
    if (*idxPtr == '-') {
      neg = true;
    } else {
      rtn = (rtn<<3) + (rtn<<1); // *10 = *8+*2
      rtn = rtn +  (*idxPtr-'0');
    }
    ++idxPtr;
  }  
  if (neg) {
    rtn = -rtn;
  }  
  *result = rtn;
  return ++idxPtr; // skip null
}



