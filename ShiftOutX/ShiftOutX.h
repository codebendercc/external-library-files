//**************************************************************//
//  Name    : shiftOutX library                              
//  Author  : Juan Hernandez 
//  Date    : March 3 2011    
//  Modified: January 29 2012                                 
//  Version : 1.0                                             
//  Notes   : free as in libre not gratis, actually it's also gratis.
//          : you can shift out to as many as 256 registers because
//			: the variable that holds the register quantity can only 
//			: count up to 256. but I doubt that anybody needs that many  
//			: that would be 256 X 8 = 2048 outputs that's pazzo
//			: juanh0238@yahoo.com   please send any suggestions 
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
#ifndef ShiftOutX_h
#define ShiftOutX_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #else
  #include "WProgram.h"
  #endif
#include <SPI.h>
 
#define ON  1
#define OFF 0
#define ALL_BITS_ON 0XFFFFFFFFFFFFFFFFLL //18446744073709551615LL //B11111111 11111111 11111111 11111111 11111111 11111111 11111111 11111111 that is a long string of unos

//if you prefer to do it manually but below are functions to do it
void shiftOut_16(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint16_t val);
void shiftOut_24(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint32_t val);
void shiftOut_32(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint32_t val);
void shiftOut_X(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t NofRegisters, uint8_t *val);


//shiftOutX class
class shiftOutX
{				
	public:
		shiftOutX(uint8_t latchPin, uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t NofRegisters); //software SPI
		
		shiftOutX(uint8_t ssPin, uint8_t NofRegisters, uint8_t bitOrder, uint8_t clockDivider); //hardware SPI
		
		shiftOutX(uint8_t ssPin, uint8_t NofRegisters, uint8_t bitOrder); //hardware SPI 
		//I did not included dataMode in the constructors because all the parameters are the same uint8_t
		//I am new to programming I did not know how to do it, but I included functions to set both clockDivider
		//and dataMode, by default they are set to SPI_CLOCK_DIV4 and SPI_MODE0 for dataMode.	
		//I used the already available functions from the SPI library to initialize SPI 
		//since they take care of setting the right pins and everything for the right MCU and board like Mega, 
		//UNO and all that, the only function I did not use was SPI.transfer() 
		//I figured including the actual code in each call to send data would be better then calling a function,
		//if you have 8 registers thats eight calls to the function or 32 if you are shifting out to 32 registers. 
		//also instead of using an integer like in version one. I used a pointer to uint8_ datatype and reseved memory with 
		//malloc for the number of registers needed. A function to send out the bits was added sendOut() this function
		//sets clockDivider and dataMode each time bits are send out just in case you are using more then one chain of 
		//registers or if you are using SPI for other devices and they require those settings to be diferent 
		//you don't have to remember to make calls to SPI.setDataMode() or SPI.setClockDivider() just set it once and
		//after that just use the functions to turn on or off pins and that's it.
		
		//next project is to add the functionality of SPI to the LiquidCrystal Library to use a shift register
		//I have seen a library that does that but with software. I am going to try to implement that same idea with SPI
		//to be able to use the same pins except of course the ssPin, 
		
		
		//turn on and off specific pins
		//self explanatory these functions turn on and off individual shift register pins by number
		//the numbers are defined in shiftPinNo.h and are named shPin1 through shPin256 	you can use integer numbers too			
		void pinOn(uint8_t shPin);
		void pinOff(uint8_t shPin);
		void allOn(void);
		void allOff(void);				
		//this following function returns true if the pin is on or false if not, it does not check the register state
		//though it only checks if the bit is set or not in the _bitString assuming you have not changed it directly
		//without shifting it out that is why it is private.
		bool pinState(uint8_t shPin);
		void setClockDivider(uint8_t clockDivider);
		void setDataMode(uint8_t _dataMode);
	
	private:
		bool _software_spi;
		uint8_t _latchPin;
		uint8_t _dataPin;
		uint8_t _clockPin;
		uint8_t _bitOrder;
		uint8_t _NofRegisters;
		uint8_t _clockDivider;
		uint8_t _dataMode;
		uint8_t *_register_array; //to point to the array of bytes or uint8_t type
		//function to setup shiftOut like setting pins to output, and populate members of the class as well.			
		void init(uint8_t latchPin, uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t NofRegisters);
		//function to setup shiftOutX with SPI
		void initSPI(uint8_t ssPin, uint8_t NofRegisters, uint8_t bitOrder, uint8_t clockDivider);
        
        protected:
        uint8_t *_register_array; //to point to the array of bytes or uint8_t type
		//function to send out bits
		void sendOut(void);
				
	}; //shiftOutX class
	
//shiftOutX class

							
// è tutto per oggi

#endif