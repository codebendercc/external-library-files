/*
  HpLaserJetLcd libraries - TextDirection
 
 This sketch demonstrates how to use leftToRight() and rightToLeft()
 to move the cursor.
  
 See the web link below for latest wiring information.
 
 Originally created 2 April 2011
 
 This example code is in the public domain.

 http://arduino.cc/playground/Main/HpLaserJetLcd
*/

// include the library code:
#include <HpLaserJetLcd.h>

// initialize the library with the numbers of the interface pins
HpLaserJetLcd lcd(3, 5, 7, 8);

int thisChar = 'a';

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // turn on the cursor:
  lcd.cursor();
  Serial.begin(9600);
}

void loop() {
  // reverse directions at 'm':
  if (thisChar == 'm') {
    // go right for the next letter
    lcd.rightToLeft(); 
  }
  // reverse again at 's':
  if (thisChar == 's') {
    // go left for the next letter
    lcd.leftToRight(); 
  }
  // reset at 'z':
  if (thisChar > 'z') {
    // go to (0,0):
    lcd.home(); 
    // start again at 0
    thisChar = 'a';
  }
  // print the character
  lcd.print(thisChar, BYTE);
  // wait a second:
  delay(1000);
  // increment the letter:
  thisChar++;
}








