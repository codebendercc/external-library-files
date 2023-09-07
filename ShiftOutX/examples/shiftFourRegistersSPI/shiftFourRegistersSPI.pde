//****************************************************************
//  Name    : shiftFourRegisters                                
//  Author  : Juan Hernandez 
//  Date    : 02 Jan 2011    
//  Modified:                                  
//  Version : 1.0                                             
//  Notes   : Code for using four 74HC595 Shift Registers         
//          : with shiftOutX class                           
//****************************************************************
#include <SPI.h>
#include <ShiftOutX.h>
#include <ShiftPinNo.h>
//_latchPin connected to ST_CP of 74HC595
//_dataPin connected to DS of 74HC595
//_clockPin connected to SH_CP of 74HC595
//_bitOrder is the order in which the bits are send to the register
//values are the constants MSBFIRST for most significant bits first
//or LSBFIRST for least significant bit first they are declared in 
//wiring.h and arduino.h
//the way I see it is do you want Q0 to be shPin1 
//or do you want Q7 to be shPin1, 
//Q0 and Q7 are pin on the 74HC595 shift register.
//number of registers is how many registers you have in series
//so if you have 2 you will shift 16 bits or 24 and 32 if you
//have 3 or 4 registers and so on
//this is how you input the parameters to the class constructor
//shiftOutX(_latchPin, _dataPin, _clockPin, _bitOrder, _NofRegisters);
//shiftOutX regOne(8, 11, 12, MSBFIRST, 4); 

shiftOutX regOne(10, 4, MSBFIRST, SPI_CLOCK_DIV2);

//    SPI_CLOCK_DIV2
//    SPI_CLOCK_DIV4
//    SPI_CLOCK_DIV8
//    SPI_CLOCK_DIV16
//    SPI_CLOCK_DIV32
//    SPI_CLOCK_DIV64 
//    SPI_CLOCK_DIV128 

//there are four shiftOut functions shiftOut16, shiftOut24, shiftOut32
//and shiftOut_X, of all these the first three are used the same as shiftOut
//with the exception that they shift to 2, 3, and 4 registers.
//the fourth one shiftOut_X can shift from 1 to as many as 256 registers and it takes 
//one more parameter than the other three which is NofRegisters 
//so the syntax is as follows
//shiftOut_X(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t NofRegisters, uint8_t *val);
//
//*val is a pointer to an "array" of uint8_t type, array because it is reserved 
//with malloc so is as many as you need long, it is not public so it can only 
//be modified by the public functions

void setup() {
  regOne.setDataMode(SPI_MODE0);
}

void loop() {  
  regOne.pinOff(shPin28);
  delay(200);
  regOne.pinOn(shPin28);
  delay(200);   
  regOne.pinOff(shPin6);
  delay(200);
  regOne.pinOn(shPin6);
  delay(200); 
  regOne.pinOff(shPin13);
  delay(200);
  regOne.pinOn(shPin13);
  delay(200);
  regOne.pinOff(shPin21);
  delay(200);
  regOne.pinOn(shPin21);
  delay(200);

  if (regOne.pinState(shPin21) == true)
  {
    regOne.pinOn(shPin1);
    regOne.pinOn(shPin32);
	regOne.pinOn(shPin16);
	regOne.pinOn(shPin17);
    delay(1000);
    regOne.pinOff(shPin1);
    regOne.pinOff(shPin32);
	regOne.pinOff(shPin16);
	regOne.pinOff(shPin17);
  }
  delay(1000);
  regOne.allOn();
  delay(1000);
  regOne.allOff();
  delay(1000);
   
}