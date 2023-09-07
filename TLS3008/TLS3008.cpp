#include "TLS3008.h"

// TLS3008 library Version 1.1.0

TLS3008::TLS3008( byte bPin, byte* ptRGBTable, unsigned int uiRGBTableSize ) {	// Constructor
//TLS3008::TLS3008( byte bPin ) {	// Constructor
	this->_bLEDPin=13; // LED pin
	this->_bPulsUs=2; // Pulse length
	this->_bEOSMs=35; // End Of Sync delay in ms
	this->_bEORMs=2; // End Of Reset delay in ms
	this->_bEOEFrameMs=3; // End of Empty Data Frame delay in ms
	this->_bEOFrameMs=5; // End of Data Frame delay (end of all leds data in string)
	this->_bFrameCount=9999; // Sent Frame counter
    this->_ptRGBT = ptRGBTable; // RGB table pointer
    this->_uiRGBTabSz = uiRGBTableSize; // RGB table size
	
	setOutputPin( bPin ); // Setup output pin
}

/*
TLS3008::~TLS3008() {	// Destructor
}
*/

void TLS3008::setOutputPin( byte bPin ) {
 this->_bLEDPin=bPin;
 pinMode(bPin, OUTPUT);
 digitalWrite(bPin, LOW);
}

void TLS3008::setPulseLength( byte bPulse ) {
	this->_bPulsUs = bPulse;
	if ( (this->_bPulsUs) < 2) this->_bPulsUs = 2;
}

void TLS3008::setEOSyncDelay( byte bEOSD ) {
	this->_bEOSMs = bEOSD;
}

void TLS3008::setEOResetDelay( byte bEOR ) {
	this->_bEORMs = bEOR;
}

void TLS3008::setEOEmptyFrameDelay( byte bEOEF ) {
	this->_bEOEFrameMs = bEOEF;
}

void TLS3008::setEOFrameDelay( byte bEOF ) {
	this->_bEOFrameMs = bEOF;
}

void TLS3008::initCom() {
	reset();
	sync();
	reset();
	sync();
	reset();
	sync();
	sendEmptyFrame();
}

void TLS3008::sync()
 {
    noInterrupts(); // Interrupts OFF
	bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);

    bitd(0);
    bitd(0);
    bitd(0);
    bitd(1);
    
    bitd(0);
    bitd(0);
    bitd(0);
    bitd(0);
    bitd(0);
    bitd(0);
    bitd(0);
    bitd(0);
    bitd(0);
    bitd(0);
    bitd(0);
	delayMicroseconds(1); // Last bit length compensation
    digitalWrite(this->_bLEDPin, LOW);

    interrupts(); // Interrupts ON
    delay(this->_bEOSMs);
 }

 void TLS3008::reset()
 {
	noInterrupts(); // Interrupts OFF
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);

    bitd(0);
    bitd(1);
    bitd(0);
    bitd(0);
	delayMicroseconds(1); // Last bit length compensation
	digitalWrite(this->_bLEDPin, LOW);

   interrupts(); // Interrupts ON
   delay(this->_bEORMs);
 }

void TLS3008::frame_start()
 {
	noInterrupts(); // Interrupts OFF
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);
    bitd(1);

    bitd(0);
    bitd(0);
    bitd(1);
    bitd(0);
 }

void TLS3008::bitd(boolean b)
 {
     digitalWrite(this->_bLEDPin, !!b);
     delayMicroseconds(this->_bPulsUs);
     digitalWrite(this->_bLEDPin, !b);
     if (this->_bPulsUs-2) delayMicroseconds(this->_bPulsUs-2); // Last bit length compensation

 }
 
void TLS3008::sendEmptyFrame() {
       frame_start();
       digitalWrite(this->_bLEDPin, LOW);
	   interrupts(); // Interrupts ON
       delay(this->_bEOEFrameMs);
       reset();
       sync();
	   this->_bFrameCount=0;
 }

 void TLS3008::sendRGBFrame() {
	unsigned int uiTS = this->_uiRGBTabSz;
	byte* ptTable = this->_ptRGBT;
	byte bMask;
	
	if (++(this->_bFrameCount)>MAXFRAMESBEFORESYNC) {
		this->_bFrameCount=0;
		sendEmptyFrame();
	}

	frame_start();
	
	while(uiTS-- > 0)
	{
		bitd(0); // Start tag
		for (bMask=0x80;bMask!=0;bMask>>=1) bitd( *ptTable & bMask );
		ptTable++;
	}
	digitalWrite(this->_bLEDPin, LOW);
	interrupts(); // Interrupts ON
	delay(this->_bEOEFrameMs);
 }

void TLS3008::setPixelColor(unsigned int pixel, byte bR, byte bG, byte bB) {
	byte* ptRGBSP = (byte*) (this->_ptRGBT) + (pixel * 3);

	*ptRGBSP = bR;
  	*++ptRGBSP = bG;
  	*++ptRGBSP = bB;
}

void TLS3008::fillPixels(byte bR, byte bG, byte bB) {
	byte* ptRGBSP = this->_ptRGBT;
	unsigned int uiTS = 0;
	
	while ( uiTS < this->_uiRGBTabSz ) {
		*ptRGBSP++ = bB;
		*ptRGBSP++ = bR;
		*ptRGBSP++ = bB;
		uiTS+=3;
  }
}

void TLS3008::clearPixels() {
	unsigned int uiTS = this->_uiRGBTabSz;
	byte* ptRGBSP = this->_ptRGBT;

	while (uiTS > 0) {
		*ptRGBSP++ = 0;
		uiTS--;
  }
}