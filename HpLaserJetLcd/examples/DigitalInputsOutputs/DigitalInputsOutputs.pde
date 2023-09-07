/*
  HpLaserJetLcd libraries - Raw digitial I/O example
 
 Demonstrates the use of the 3 digital outputs and 9 digital inputs
 on the LCD module from LaserJet printers.
  
 See the web link below for latest wiring information.
 
 Originally created 2 April 2011
 
 This example code is in the public domain.

 http://arduino.cc/playground/Main/HpLaserJetLcd
*/

// include the library code:
#include <HpLaserJetLcd.h>

// initialize the library with the numbers of the interface pins
HpLaserJetLcd lcd(3, 5, 7, 8);

void printBits(uint16_t value, uint8_t bitCount)
{
  for (uint16_t mask = (1<<(uint16_t)(bitCount-1)); mask; mask >>= 1)
  {
    lcd.write((value & mask) ? '1' : '0');    
  }
}

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  // Print a message to the LCD.
  lcd.print("I/O Example");
}

void loop() {
  // set the cursor to column 0, line 0
  lcd.setCursor(0, 0);
  lcd.print("In: ");
  
  // Display the current raw digital input state bit values.
  printBits(lcd.getDigitalInputs(), 9);

  // Update the current raw digital output state.
  static uint8_t outputState = 0;
  lcd.setDigitalOutputs(outputState);

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print("Out: ");

  // Display the output bit values.
  printBits(outputState, 3);

  // Increment the output state once per second.
  static uint32_t lastChangeTimeInMillis = 0;
  if ((millis() - lastChangeTimeInMillis) > 1000)
  {
    lastChangeTimeInMillis = millis();

    outputState++;
    outputState &= 0b111;
  }

}


