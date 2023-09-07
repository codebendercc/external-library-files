//**************************************************************//
//  Name    : shiftOutX library                              
//  Author  : Juan Hernandez 
//  Date    : March 3 2011    
//  Modified: January 29 2012                                 
//  Version : 1.0                                             
//  Notes   : free as in libre not gratis actually it's also gratis
//          : this functions are just modified versions of the 
//			: shiftOut function in wiring_shift.c                           
//****************************************************************

/*  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA  
*/
#include "ShiftOutX.h"

//for 2 shift registers
void shiftOut_16(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint16_t val)
{
	uint8_t i;
	for (i = 0; i < 16; i++)  {
		if (bitOrder == LSBFIRST)
			digitalWrite(dataPin, !!(val & (1 << i)));
		else	
			digitalWrite(dataPin, !!(val & (1 << (15 - i)))); 
			
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);		
	}
}
//for 3 shift registers
void shiftOut_24(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint32_t val)
{
	uint8_t i;
	uint8_t i1;
	uint8_t Sbyte;
	uint8_t shiftNo;
	if (bitOrder == LSBFIRST){
				Sbyte = val;
				for (i1 = 0; i1 < 3; i1++) {
					for (i = 0; i < 8; i++)  {
						digitalWrite(dataPin, !!(Sbyte & (1 << i)));
						digitalWrite(clockPin, HIGH);
						digitalWrite(clockPin, LOW);
					}
					shiftNo += 8;
					Sbyte = val >> shiftNo;
				}
			}
			else if (bitOrder == MSBFIRST){
				shiftNo = 16;
				for (i1 = 0; i1 < 3; i1++) {
					Sbyte = val >> shiftNo;
					for (i = 0; i < 8; i++)  {
						digitalWrite(dataPin, !!(Sbyte & (1 << (7 - i))));
						digitalWrite(clockPin, HIGH);
						digitalWrite(clockPin, LOW);
					}
					shiftNo -= 8;
				}
			}
}
//for 4 shift registers
void shiftOut_32(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint32_t val)
{
	uint8_t i;
	uint8_t i1;
	uint8_t Sbyte;
	uint8_t shiftNo;				
			if (bitOrder == LSBFIRST){
				Sbyte = val;
				for (i1 = 0; i1 < 4; i1++) {
					for (i = 0; i < 8; i++)  {
						digitalWrite(dataPin, !!(Sbyte & (1 << i)));
						digitalWrite(clockPin, HIGH);
						digitalWrite(clockPin, LOW);
					}
					shiftNo += 8;
					Sbyte = val >> shiftNo;
				}
			}
			else if (bitOrder == MSBFIRST){
				shiftNo = 24;
				for (i1 = 0; i1 < 4; i1++) {
					Sbyte = val >> shiftNo;
					for (i = 0; i < 8; i++)  {
						digitalWrite(dataPin, !!(Sbyte & (1 << (7 - i))));
						digitalWrite(clockPin, HIGH);
						digitalWrite(clockPin, LOW);
					}
					shiftNo -= 8;
				}
			}
				
}

void shiftOut_X(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t NofRegisters, uint8_t *val)
{
	uint8_t i;
	uint8_t i1;
	uint8_t Sbyte;
	uint8_t shiftNo;
			if (bitOrder == LSBFIRST)
			{
				shiftNo = 0;
				Sbyte = *(val + shiftNo);
				for (i1 = 0; i1 < NofRegisters; i1++) 
				{					
					for (i = 0; i < 8; i++)  
					{
						digitalWrite(dataPin, !!(Sbyte & (1 << i)));
						digitalWrite(clockPin, HIGH);
						digitalWrite(clockPin, LOW);
					}
					Sbyte = *(val + (++shiftNo));
				}
			}
			else if (bitOrder == MSBFIRST)
			{
				shiftNo = NofRegisters - 1;
				Sbyte = *(val + shiftNo);
				for (i1 = 0; i1 < NofRegisters; i1++) 
				{
					for (i = 0; i < 8; i++)  
					{
						digitalWrite(dataPin, !!(Sbyte & (1 << (7 - i))));						
						digitalWrite(clockPin, HIGH);
						digitalWrite(clockPin, LOW);
					}
					Sbyte = *(val + (--shiftNo));
				}
			}
			
}

////////////////////////////////////// Constructors ///////////////////////////////////////////////

 shiftOutX::shiftOutX(uint8_t latchPin, uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t NofRegisters) //software ISP
{
	init(latchPin, dataPin, clockPin, bitOrder, NofRegisters);
}

shiftOutX::shiftOutX(uint8_t ssPin, uint8_t NofRegisters, uint8_t bitOrder, uint8_t clockDivider) //SPI
{
	initSPI(ssPin, NofRegisters, bitOrder, clockDivider);
}

shiftOutX::shiftOutX(uint8_t ssPin, uint8_t NofRegisters, uint8_t bitOrder) //SPI
{
	initSPI(ssPin, NofRegisters, bitOrder, 0);
}

/////////////////////////////////////// initializers ///////////////////////////////////////////////
void shiftOutX::init(uint8_t latchPin, uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t NofRegisters)
{
	//populate info
	_software_spi = true;
	_latchPin = latchPin;
	_dataPin = dataPin;
	_clockPin = clockPin;
	_bitOrder = bitOrder;
	_NofRegisters = NofRegisters;
	_register_array = (uint8_t*) malloc(_NofRegisters); //reserve as many bytes as number of registers
	for (int i = 0; i < _NofRegisters; i++)
	{
		*(_register_array + i) = 0; //set every bit to zero
	}
	//set needed pins to output
	pinMode(_latchPin, OUTPUT);
	pinMode(_clockPin, OUTPUT);
	pinMode(_dataPin, OUTPUT);
}

void shiftOutX::initSPI(uint8_t ssPin, uint8_t NofRegisters, uint8_t bitOrder, uint8_t clockDivider)
{
	_software_spi = false;
	// initialize SPI:
	_latchPin = ssPin;
	pinMode (_latchPin, OUTPUT); //just in case _latchPin is not 10 or 53 set it to output 
								 //otherwise SPI.begin() will set it to output but just in case
		
	SPI.begin();
	
	_NofRegisters = NofRegisters;
	_register_array = (uint8_t*) malloc(_NofRegisters); //reserve as many bytes as number of registers
	for (int i = 0; i < _NofRegisters; i++)
	{
		*(_register_array + i) = 0;   //set every bit to zero
	}
	
	
	if (bitOrder == LSBFIRST)
	{
		_bitOrder = bitOrder;
	}
	else
	{
		_bitOrder = MSBFIRST;
	}
	
	if (clockDivider)
	{
		_clockDivider = clockDivider;
		SPI.setClockDivider(_clockDivider);
	}
	else
	{
		_clockDivider = SPI_CLOCK_DIV4;
		SPI.setClockDivider(_clockDivider);
	}
	
	//set data mode to SPI_MODE0 by default
	_dataMode = SPI_MODE0;
	SPI.setDataMode(_dataMode);
		
}

////////////////////////////functions/////////////////////////////////////////

void shiftOutX::setClockDivider(uint8_t clockDivider)
{
	_clockDivider = clockDivider;
	SPI.setClockDivider(_clockDivider);
}


void shiftOutX::setDataMode(uint8_t dataMode)
{
	_dataMode = dataMode;
	SPI.setDataMode(_dataMode);
}


void shiftOutX::pinOn(uint8_t shPin)
{
	uint8_t _regNo = shPin / 8;
	uint8_t _bitNo;
	//determine register number	
	if (shPin % 8 > 0)
    {
		_regNo++;
    }
	
	//determine bit number
	if (shPin % 8 > 0)
    {
       _bitNo = (shPin % 8) - 1; // zero is number one bit, one is two and so on...
    }
    else if (shPin % 8 == 0) // if we get zero the bit is the last one of the register
    {
        _bitNo = 7;  //so we set it to sette not otto
    }
	
	bitSet(*(_register_array + (_regNo - 1)), _bitNo); //set the bit we want to turn on
		
	sendOut();
}					
				
void shiftOutX::pinOff(uint8_t shPin)
{
	uint8_t _regNo = shPin / 8;
	uint8_t _bitNo;
	
	//determine register number
	if (shPin % 8 > 0)
    {
		_regNo++;
    }
	
	//determine bit number
	if (shPin % 8 > 0)
    {
       _bitNo = (shPin % 8) - 1; // zero is number one bit, one is two and so on...
    }
    else if (shPin % 8 == 0) // if we get zero the bit is the last one of the register
    {
        _bitNo = 7;  //so we set it to sette not otto
    }
	
	bitClear(*(_register_array + (_regNo - 1)), _bitNo); //now clear the bit we currently want to turn off
		
	sendOut();
}

void shiftOutX::allOn()
{
	for (int i = 0; i < _NofRegisters; i++)
	{
		*(_register_array + i) = 255;	//set all bits to 1
	}
	
	sendOut();
}

void shiftOutX::allOff()
{
	for (int i = 0; i < _NofRegisters; i++)
	{
		*(_register_array + i) = 0;	//set all bits to zero
	}
	 
	sendOut();
}

bool shiftOutX::pinState(uint8_t shPin)
{
	uint8_t _regNo = shPin / 8;
	uint8_t _bitNo;
	//determine register number
	if (shPin % 8 > 0)
    {
		_regNo++;
    }
	
	//determine bit number
	if (shPin % 8 > 0)
    {
       _bitNo = (shPin % 8) - 1; // zero is number one bit, one is two and so on...
    }
    else if (shPin % 8 == 0) // if we get zero the bit is the last one of the register
    {
        _bitNo = 7;  //so we set it to sette not otto
    }
	
	if(bitRead(*(_register_array + (_regNo - 1)), _bitNo))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void shiftOutX::sendOut(void)
{
	uint8_t Sbyte;
	uint8_t shiftNo;
	// take the latchPin low to let the right device know you are sending data
	digitalWrite(_latchPin, LOW);
	if (_software_spi == true)
	{		 
		// shift out the bits:
		shiftOut_X(_dataPin, _clockPin, _bitOrder, _NofRegisters, _register_array);			
	}
	else //we use SPI
	{
		//just in case you are using SPI for more then one device
		//set bitOrder, clockDivider and dataMode each time
		SPI.setClockDivider(_clockDivider); 
		SPI.setBitOrder(_bitOrder);
		SPI.setDataMode(_dataMode); 		
		
		if (_bitOrder == LSBFIRST)
		{
			shiftNo = 0;
			Sbyte = *(_register_array + shiftNo);
			for (uint8_t i = 0; i < _NofRegisters; i++)
			{
				SPDR = Sbyte;
				while (!(SPSR & (1<<SPIF)))
					;
				//shiftNo++;
				Sbyte = *(_register_array + (++shiftNo));
			}
		}
		else if (_bitOrder == MSBFIRST)
		{
			shiftNo = _NofRegisters - 1;
			Sbyte = *(_register_array + shiftNo);
			for (uint8_t i = 0; i < _NofRegisters; i++) 
			{
				SPDR = Sbyte;
				while (!(SPSR & (1<<SPIF)))
					;
				Sbyte = *(_register_array + (--shiftNo));
			}
		}		
	}
	//take the latch pin high so the device stops listening for data (I think)
	digitalWrite(_latchPin, HIGH);
}
