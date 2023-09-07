/*
  Qtouch1wire.h - 1 wire communication for Atmel qtouch, specifically QT1103
  Copyright (c) 2009 Dave W. Vondle  All right reserved.
 
  Based on the SoftwareSerial Library by David A. Mellis

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

#ifndef Qtouch1wire_h
#define Qtouch1wire_h

#include <inttypes.h>

class Qtouch1wire
{
  private:
    uint8_t _oneWirePin;
    long _baudRate;
    int _bitPeriod;
	uint8_t read();
  public:
	Qtouch1wire();
    void begin(uint8_t, long);
    uint16_t requestRead();
};

extern Qtouch1wire OneWire;

#endif

