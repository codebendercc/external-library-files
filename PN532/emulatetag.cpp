/**************************************************************************/
/*!
    @file     emulatetag.cpp
    @author   Armin Wieser
    @license  BSD
*/
/**************************************************************************/

#include "emulatetag.h"
#include "PN532_debug.h"

#include <string.h>

#define MAX_TGREAD


// Command APDU
#define C_APDU_CLA   0
#define C_APDU_INS   1 // instruction
#define C_APDU_P1    2 // parameter 1
#define C_APDU_P2    3 // parameter 2
#define C_APDU_LC    4 // length command
#define C_APDU_DATA  5 // data

#define C_APDU_P1_SELECT_BY_ID   0x00
#define C_APDU_P1_SELECT_BY_NAME 0x04

// Response APDU
#define R_APDU_SW1_COMMAND_COMPLETE 0x90 
#define R_APDU_SW2_COMMAND_COMPLETE 0x00 

#define R_APDU_SW1_NDEF_TAG_NOT_FOUND 0x6a
#define R_APDU_SW2_NDEF_TAG_NOT_FOUND 0x82

#define R_APDU_SW1_FUNCTION_NOT_SUPPORTED 0x6A
#define R_APDU_SW2_FUNCTION_NOT_SUPPORTED 0x81

#define R_APDU_SW1_MEMORY_FAILURE 0x65
#define R_APDU_SW2_MEMORY_FAILURE 0x81

#define R_APDU_SW1_END_OF_FILE_BEFORE_REACHED_LE_BYTES 0x62
#define R_APDU_SW2_END_OF_FILE_BEFORE_REACHED_LE_BYTES 0x82

// ISO7816-4 commands
#define ISO7816_SELECT_FILE 0xA4
#define ISO7816_READ_BINARY 0xB0
#define ISO7816_UPDATE_BINARY 0xD6

typedef enum { NONE, CC, NDEF } tag_file;   // CC ... Compatibility Container

const uint8_t compatibility_container[] = {
  0, 0x0F,
  0x20,
  0, 0x54,
  0, 0xFF,
  0x04,
  0x06,
  0xE1, 0x04,
  0xFF, 0xFE,
  0x00,
  0x00
};

bool EmulateTag::init(){
  pn532.begin();
  return pn532.SAMConfig();
}

void EmulateTag::setNdefFile(const uint8_t* ndef, const int16_t ndefLength){
  if(ndefLength >  (NDEF_MAX_LENGTH -2)){
	DMSG("ndef file too large (> NDEF_MAX_LENGHT -2) - aborting");
	return;
  }

  ndef_file[0] = ndefLength >> 8;
  ndef_file[1] = ndefLength & 0xFF;
  memcpy(ndef_file+2, ndef, ndefLength);
}

void EmulateTag::emulate(const uint8_t* uid){

  uint8_t command[] = {
        PN532_COMMAND_TGINITASTARGET,
        5,                  // MODE: PICC only, Passive only

        0x04, 0x00,         // SENS_RES
        0x00, 0x00, 0x00,   // NFCID1
        0x20,               // SEL_RES

        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,   // FeliCaParams
        0,0,

        0,0,0,0,0,0,0,0,0,0, // NFCID3t

        0, // length of general bytes
        0  // length of historical bytes
	  };

  if(uid != 0){  // if uid is set copy 3 bytes to nfcid1
    memcpy(command + 4, uid, 3);
  }

  if(1 != pn532.tgInitAsTarget(command,sizeof(command))){
    DMSG("tgInitAsTarget failed!");
    return;
  }

  tagWrittenByInitiator = false;

  uint8_t rwbuf[128];
  uint8_t sendlen;
  int16_t status;
  tag_file currentFile = NONE;
  uint16_t cc_size = sizeof(compatibility_container);

  while(1){

    status = pn532.tgGetData(rwbuf, sizeof(rwbuf));
    if(status < 0){
      DMSG("tgGetData failed!\n");
      return;
    }

    uint8_t p1 = rwbuf[C_APDU_P1];
    uint8_t p2 = rwbuf[C_APDU_P2];
    uint8_t lc = rwbuf[C_APDU_LC];

    switch(rwbuf[C_APDU_INS]){
    case ISO7816_SELECT_FILE:
      switch(p1){
      case C_APDU_P1_SELECT_BY_ID:
	if(p2 != 0x0c){
	  DMSG("C_APDU_P2 != 0x0c\n");
	  setResponse(COMMAND_COMPLETE, rwbuf, &sendlen);
	} else if(lc == 2 && rwbuf[C_APDU_DATA] == 0xE1 && (rwbuf[C_APDU_DATA+1] == 0x03 || rwbuf[C_APDU_DATA+1] == 0x04)){
	  setResponse(COMMAND_COMPLETE, rwbuf, &sendlen);
	  if(rwbuf[C_APDU_DATA+1] == 0x03){
	    currentFile = CC;
	  } else if(rwbuf[C_APDU_DATA+1] == 0x04){
	    currentFile = NDEF;
	  }
	} else {
	  setResponse(TAG_NOT_FOUND, rwbuf, &sendlen);
	}
	break;
      case C_APDU_P1_SELECT_BY_NAME: 
        const uint8_t ndef_tag_application_name_v2[] = {0, 0x7, 0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01 };
	if(0 == memcmp(ndef_tag_application_name_v2, rwbuf + C_APDU_P2, sizeof(ndef_tag_application_name_v2))){
	  setResponse(COMMAND_COMPLETE, rwbuf, &sendlen);
	} else{
	  DMSG("function not supported\n");
	  setResponse(FUNCTION_NOT_SUPPORTED, rwbuf, &sendlen);
	} 
	break;
      }
      break;
    case ISO7816_READ_BINARY:
      switch(currentFile){
      case NONE:
	setResponse(TAG_NOT_FOUND, rwbuf, &sendlen);
	break;
      case CC:
	if((int16_t)(p1 << 8) + p2 > NDEF_MAX_LENGTH){
	  setResponse(END_OF_FILE_BEFORE_REACHED_LE_BYTES, rwbuf, &sendlen);
	}else {
	  memcpy(rwbuf,compatibility_container + (p1 << 8) + p2, lc);
	  setResponse(COMMAND_COMPLETE, rwbuf + lc, &sendlen, lc);
	}
	break;
      case NDEF:
	if((int16_t)(p1 << 8) + p2 > NDEF_MAX_LENGTH){
	  setResponse(END_OF_FILE_BEFORE_REACHED_LE_BYTES, rwbuf, &sendlen);
	}else {
	  memcpy(rwbuf, ndef_file + (p1 << 8) + p2, lc);
	  setResponse(COMMAND_COMPLETE, rwbuf + lc, &sendlen, lc);
	}
	break;
      }
      break;    
    case ISO7816_UPDATE_BINARY:
      if((int16_t)(p1 << 8) + p2 > NDEF_MAX_LENGTH){
	setResponse(MEMORY_FAILURE, rwbuf, &sendlen);
      }
      else{
	memcpy(ndef_file + (p1 << 8) + p2, rwbuf + C_APDU_DATA, lc);
	setResponse(COMMAND_COMPLETE, rwbuf, &sendlen);
	tagWrittenByInitiator = true;
      }
      break;
    default:
      DMSG("Command not supported!");
      DMSG_HEX(rwbuf[C_APDU_INS]);
      DMSG("\n");
      setResponse(FUNCTION_NOT_SUPPORTED, rwbuf, &sendlen);
    }
    status = pn532.tgSetData(rwbuf, sendlen);
    if(status < 0){
      DMSG("tgSetData failed\n!");
      return;
    }
  }
}

void EmulateTag::setResponse(responseCommand cmd, uint8_t* buf, uint8_t* sendlen, uint8_t sendlenOffset){
  switch(cmd){
  case COMMAND_COMPLETE:
    buf[0] = R_APDU_SW1_COMMAND_COMPLETE;
    buf[1] = R_APDU_SW2_COMMAND_COMPLETE;
    *sendlen = 2 + sendlenOffset;
    break;
  case TAG_NOT_FOUND:
    buf[0] = R_APDU_SW1_NDEF_TAG_NOT_FOUND;
    buf[1] = R_APDU_SW2_NDEF_TAG_NOT_FOUND;
    *sendlen = 2;    
    break;
  case FUNCTION_NOT_SUPPORTED:
    buf[0] = R_APDU_SW1_FUNCTION_NOT_SUPPORTED;
    buf[1] = R_APDU_SW2_FUNCTION_NOT_SUPPORTED;
    *sendlen = 2; 
    break;
  case MEMORY_FAILURE:
    buf[0] = R_APDU_SW1_MEMORY_FAILURE;
    buf[1] = R_APDU_SW2_MEMORY_FAILURE;
    *sendlen = 2;
    break;
  case END_OF_FILE_BEFORE_REACHED_LE_BYTES:
    buf[0] = R_APDU_SW1_END_OF_FILE_BEFORE_REACHED_LE_BYTES;
    buf[1] = R_APDU_SW2_END_OF_FILE_BEFORE_REACHED_LE_BYTES;
    *sendlen= 2;
    break;
  }
}
