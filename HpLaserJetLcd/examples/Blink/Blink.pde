/*
  HpLaserJetLcd libraries - Blink
 
 This sketch prints "Hello World!" to the LCD and makes the 
 cursor block blink.
  
 See the web link below for latest wiring information.
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe 
 modified 25 July 2009
 by David A. Mellis
 Ported to HpLaserJetLcd 2 April 2011
 
 This example code is in the public domain.

 http://arduino.cc/playground/Main/HpLaserJetLcd
*/

// include the library code:
#include <HpLaserJetLcd.h>

// initialize the library with the numbers of the interface pins
HpLaserJetLcd lcd(3, 5, 7, 8);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // Turn off the blinking cursor:
  lcd.noBlink();
  delay(3000);
   // Turn on the blinking cursor:
  lcd.blink();
  delay(3000);
}


