/*
  SoftwareSerial.cpp - Software serial library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/******************************************************************************
 * Includes
 ******************************************************************************/

#include "WConstants.h"
#include "Qtouch1wire.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/

Qtouch1wire::Qtouch1wire()
{
}

/******************************************************************************
 * User API
 ******************************************************************************/

void Qtouch1wire::begin(uint8_t oneWirePin, long speed)
{
	_oneWirePin = oneWirePin;
	
	_baudRate = speed;
	_bitPeriod = 1000000 / _baudRate;

	pinMode(_oneWirePin,INPUT);
}

uint8_t Qtouch1wire::read()
{
	int val = 0;
	int bitDelay = _bitPeriod - clockCyclesToMicroseconds(50);
	
	unsigned long t;
	unsigned long t2;
  // one byte of serial data (LSB first)
  // ...--\    /--\/--\/--\/--\/--\/--\/--\/--\/--...
  //	 \--/\--/\--/\--/\--/\--/\--/\--/\--/
  //	start  0   1   2   3   4   5   6   7 stop
	t=micros();
	while (digitalRead(_oneWirePin) && (t2-t)<(bitDelay*3)){ //datasheet says between 1 and 3 bit cycles
		t2=micros();
	}
    // frame start indicated by a falling edge and low start bit
    // jump to the middle of the low start bit
    delayMicroseconds(bitDelay / 2  + clockCyclesToMicroseconds(50)); //trial and error...
    // offset of the bit in the byte: from 0 (LSB) to 7 (MSB)
    for (int offset = 0; offset < 8; offset++) {
		// jump to middle of next bit
		delayMicroseconds(bitDelay);
		// read bit
		val |= digitalRead(_oneWirePin) << offset;
    }
    delayMicroseconds(bitDelay * 2);
    return val;
}

uint16_t Qtouch1wire::requestRead()
{
	if (_baudRate == 0)
		return 0;

	uint16_t result;
	int bitDelay = _bitPeriod - clockCyclesToMicroseconds(50); // a digitalWrite is about 50 cycles
	byte mask;
	
	pinMode(_oneWirePin,OUTPUT);
	digitalWrite(_oneWirePin, LOW);
	delayMicroseconds(bitDelay);

	for (mask = 0x01; mask; mask <<= 1) {
		if (80 & mask){ // choose bit, send "80" or "P"
			digitalWrite(_oneWirePin,HIGH); // send 1
		}
		else{
			digitalWrite(_oneWirePin,LOW); // send 1
		}
		delayMicroseconds(bitDelay);
	}

	digitalWrite(_oneWirePin, HIGH);
	delayMicroseconds(bitDelay);
	
	pinMode(_oneWirePin,OUTPUT);
	
	result=read()<<8; 
	result+=read();  
	
	return result;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

Qtouch1wire OneWire = Qtouch1wire();
