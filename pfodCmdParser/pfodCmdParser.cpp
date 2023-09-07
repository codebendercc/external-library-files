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


#include "pfodCmdParser.h"
	
pfodCmdParser::pfodCmdParser() {
  parserSetup();
}

void pfodCmdParser::parserSetup() {
  parserByteCounter = 0;
  cmdByte = 0; // not started yet
  parserState = pfodWaitingForStart;
  // switch to pfodInMsg when see {
}

//-------------------------------------
//PROCESS_RECEIVED_CHAR
// { char }
// save first char after {
// store in cmdByte 
//  
// before {  parserState == pfodWaitingForStart
// when { seen parserState == pfodInMsg
// after { first char ( != } ) is stored in cmdByte
// cmdByte returned when terminating } seen but
// not if no } seen for 254 bytes after starting { then
// ignore msg and start looking for { again
// 
//-------------------------------------
// return 0 if no cmd found yet
// else return cmd when see }
// or ignore if >254 bytes after {
//
// in is byte read from Serial port
byte pfodCmdParser::parse(byte in) {
  parserByteCounter++;
  if (parserState == pfodWaitingForStart) {
    if (in == pfodStartMsg) { // found {
       parserSetup(); // clean out last cmd
       parserState = pfodInMsg;
    } // else ignore this char as waiting for start {
    // always reset counter if waiting for {
    parserByteCounter = 1;
    return 0;
  }    
  
  // else have seen { 
  if ((parserByteCounter == pfodMaxMsgLen) &&
        (in != pfodEndMsg)) {
    // ignore this msg and reset
    // should not happen as pfodApp should limit
    // msgs sent to pfodDevice to <=255 bytes
    parserSetup();
    return 0;
  }
  
  // else is this the end of the msg
  if (in == pfodEndMsg) {
    byte pfodMsgCmd = cmdByte;
    // this will return 0 when parsing {} msg
    // set up to wait for next {
    parserSetup();
    // return command byte found
    return pfodMsgCmd;
  // else normal byte
  } else if (cmdByte == 0) {
    // save first cmd byte (only the first)
    cmdByte = in;
  } 
  return 0;
 }


