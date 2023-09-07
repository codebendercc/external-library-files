


#ifndef __SPI_H__
#define __SPI_H__

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <SPI.h>

#include "utility/wlan.h"

typedef void (*gcSpiHandleRx)(void *p);
typedef void (*gcSpiHandleTx)(void);

extern unsigned char wlan_tx_buffer[];

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern void SpiOpen(gcSpiHandleRx pfRxHandler);
extern void SpiClose(void);
extern long SpiWrite(unsigned char *pUserBuffer, unsigned short usLength);
extern void SpiResumeSpi(void);
extern void SpiCleanGPIOISR(void);
extern int  init_spi(void);
extern long TXBufferIsEmpty(void);
extern long RXBufferIsEmpty(void);
extern void CC3000_UsynchCallback(long lEventType, char * data, unsigned char length);
extern void WriteWlanPin( unsigned char val );
extern long ReadWlanInterruptPin(void);
extern void WlanInterruptEnable();
extern void WlanInterruptDisable();
extern char *sendDriverPatch(unsigned long *Length);
extern char *sendBootLoaderPatch(unsigned long *Length);
extern char *sendWLFWPatch(unsigned long *Length);
extern void SPI_IRQ(void);

#endif

