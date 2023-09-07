#ifndef pfodCmdParser_h
#define pfodCmdParser_h
/*
 pfod cmdParser for Arduino
 parses { cmd }
 return cmd found (first byte only)  when get }
 otherwise return 0 if no complete msg yet.
 see www.pfod.com.au  for more details.
 
 pfodCmdParser adds about 152 bytes to the program and uses about 3 bytes RAM

 (c)2012 Forward Computing and Control Pty. Ltd. 
 This code may be freely used for both private and commerical use.
 Provide this copyright is maintained.
 */

#include "Arduino.h"

class pfodCmdParser {
public:	
pfodCmdParser();	
byte parse(byte in);

private:
void parserSetup();	
byte cmdByte;
byte parserByteCounter;
byte parserState;
static const byte pfodMaxMsgLen = 0xff; // == 255, if no closing } by now ignore msg 
static const byte pfodStartMsg = (byte)'{';
static const byte pfodEndMsg = (byte)'}';
static const byte pfodWaitingForStart = 0xff;
static const byte pfodInMsg = 0;
};

#endif // pfodCmdParser_h

	