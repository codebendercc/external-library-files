
//*****************************************************************************
//
//! \addtogroup common_api
//! @{
//
//*****************************************************************************
/******************************************************************************
 *
 * Include files
 *
 *****************************************************************************/
#include "cc3000_common.h"
#include "socket.h"
#include "wlan.h"
#include "evnt_handler.h"
#include "debug.h"
#include <Arduino.h>

//*****************************************************************************
//
//!  __error__
//!
//!  @param  pcFilename - file name, where error occurred
//!  @param  ulLine     - line number, where error occurred
//!
//!  @return none
//!
//!  @brief stub function for ASSERT macro
//
//*****************************************************************************
void
__error__(char *pcFilename, unsigned long ulLine)
{
    //TODO full up function
}



//*****************************************************************************
//
//!  UINT32_TO_STREAM_f
//!
//!  @param  p       pointer to the new stream
//!  @param  u32     pointer to the 32 bit
//!
//!  @return               pointer to the new stream
//!
//!  @brief                This function is used for copying 32 bit to stream
//!						   while converting to little endian format.
//
//*****************************************************************************

uint8_t* UINT32_TO_STREAM_f (uint8_t *p, uint32_t u32)
{
	*(p)++ = (uint8_t)(u32);
	*(p)++ = (uint8_t)((u32) >> 8);
	*(p)++ = (uint8_t)((u32) >> 16);
	*(p)++ = (uint8_t)((u32) >> 24);
	return p;
}

//*****************************************************************************
//
//!  UINT16_TO_STREAM_f
//!
//!  @param  p       pointer to the new stream
//!  @param  u32     pointer to the 16 bit
//!
//!  @return               pointer to the new stream
//!
//!  @brief               This function is used for copying 16 bit to stream
//!                       while converting to little endian format.
//
//*****************************************************************************

uint8_t* UINT16_TO_STREAM_f (uint8_t *p, uint16_t u16)
{
	*(p)++ = (uint8_t)(u16);
	*(p)++ = (uint8_t)((u16) >> 8);
	return p;
}

//*****************************************************************************
//
//!  STREAM_TO_UINT16_f
//!
//!  @param  p          pointer to the stream
//!  @param  offset     offset in the stream
//!
//!  @return               pointer to the new 16 bit
//!
//!  @brief               This function is used for copying received stream to
//!                       16 bit in little endian format.
//
//*****************************************************************************

uint16_t STREAM_TO_UINT16_f(char* cp, uint16_t offset)
{
  uint8_t *p = (uint8_t *)cp;
  /*
  DEBUGPRINT_F("Stream2u16: ");
  DEBUGPRINT_HEX(cp[offset+1]);
  DEBUGPRINT_F(" + ");
  DEBUGPRINT_HEX(cp[offset]);
  DEBUGPRINT_F("\n\r");
  */

  return (uint16_t)((uint16_t)
		    ((uint16_t)(*(p + offset + 1)) << 8) +
		    (uint16_t)(*(p + offset)));
}

//*****************************************************************************
//
//!  STREAM_TO_UINT32_f
//!
//!  @param  p          pointer to the stream
//!  @param  offset     offset in the stream
//!
//!  @return               pointer to the new 32 bit
//!
//!  @brief               This function is used for copying received stream to
//!                       32 bit in little endian format.
//
//*****************************************************************************

uint32_t STREAM_TO_UINT32_f(char * cp, uint16_t offset)
{
  uint8_t *p = (uint8_t *)cp;
  
  /*
  DEBUGPRINT_F("\tStream2u32: ");
  DEBUGPRINT_HEX(cp[offset+3]);  DEBUGPRINT_F(" + ");
  DEBUGPRINT_HEX(cp[offset+2]);  DEBUGPRINT_F(" + ");
  DEBUGPRINT_HEX(cp[offset+1]);  DEBUGPRINT_F(" + ");
  DEBUGPRINT_HEX(cp[offset]);
  DEBUGPRINT_F("\n\r");
  */

  return (uint32_t)((uint32_t)((uint32_t)
	(*(p + offset + 3)) << 24) + (uint32_t)((uint32_t)
	(*(p + offset + 2)) << 16) + (uint32_t)((uint32_t)
	(*(p + offset + 1)) << 8) + (uint32_t)(*(p + offset)));
}



//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
