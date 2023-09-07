#ifndef pfodParser_h
#define pfodParser_h
/**
pfodParser for Arduino
 Parses commands of the form { cmd | arg1 ` arg2 ... } 
 Arguments are separated by ` 
 The | and the args are optional
 This is a complete paser for ALL commands a pfodApp will send to a pfodDevice
 see www.pfod.com.au  for more details.
 
  pfodParser adds about 482 bytes to the program and uses about 260 bytes RAM
 
The pfodParser parses messages of the form
 { cmd | arg1 ` arg2 ` ... }
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

#include "Arduino.h"

class pfodParser {
public:	
pfodParser();
void init();
byte parse(byte in);
byte* getCmd();
byte* getFirstArg();
byte getArgsCount();
byte* parseLong(byte* idxPtr, long *result);
/**
 * pfodWaitingForStart if outside msg
 * pfodMsgStarted if just seen opening {
 * pfodInMsg in msg after {
 * prodEndMsg if just seen closing }
 */
byte getParserState();
void setCmd(byte cmd);
static const byte pfodWaitingForStart = 0xff;
static const byte pfodMsgStarted = '{';
static const byte pfodInMsg = 0;
static const byte pfodMsgEnd = '}';


private:
// you can reduce the value if you are handling smaller messages
// but never increase it.
static const byte pfodMaxMsgLen = 0xff; // == 255, if no closing } by now ignore msg 

byte argsCount;  // no of arguments found in msg
byte argsIdx;
byte parserState;  
byte args[pfodMaxMsgLen];

static const byte pfodEndCmd = (byte)'|';
static const byte pfodArgSeparator = (byte)'`';
static const byte pfodArgStarted = 0xfe;
};

#endif // pfodParser_h

	