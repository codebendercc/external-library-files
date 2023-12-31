#include <Shift595.h>

#define   dataPin          11      // pin 14 on the 74HC595
#define   latchPin          4      // pin 12 on the 74HC595
#define   clockPin          2      // pin 11 on the 74HC595

#define   numOfRegisters    1      // number of shift registers present

Shift595 Shifter(dataPin, latchPin, clockPin, numOfRegisters);

void setup(){}

void loop()
{
  Shifter.setRegisterPin(0, HIGH);
  Shifter.setRegisterPin(1, HIGH);
  Shifter.setRegisterPin(2, HIGH);
  Shifter.setRegisterPin(3, HIGH); 
  Shifter.setRegisterPin(4, HIGH);
  Shifter.setRegisterPin(5, HIGH);
  Shifter.setRegisterPin(6, HIGH);
  Shifter.setRegisterPin(7, HIGH);

  delay(1000);
  
  Shifter.setRegisterPin(0, LOW);
  Shifter.setRegisterPin(1, LOW);
  Shifter.setRegisterPin(2, LOW);
  Shifter.setRegisterPin(3, LOW); 
  Shifter.setRegisterPin(4, LOW);
  Shifter.setRegisterPin(5, LOW);
  Shifter.setRegisterPin(6, LOW);
  Shifter.setRegisterPin(7, LOW);

  delay(1000);
  
}
