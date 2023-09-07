/*
  HpLaserJetLcd libraries - Autoscroll
 
 This sketch demonstrates the use of the autoscroll()
 and noAutoscroll() functions to make new text scroll or not.
 
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
  lcd.begin(16,2);
}

void loop() {
  // set the cursor to (0,0):
  lcd.setCursor(0, 0);
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
   lcd.print(thisChar);
   delay(500);
  }

  // set the cursor to (16,1):
  lcd.setCursor(16,1);
  // set the display to automatically scroll:
  lcd.autoscroll();
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    lcd.print(thisChar);
    delay(500);
  }
  // turn off automatic scrolling
  lcd.noAutoscroll();
  
  // clear screen for the next loop:
  lcd.clear();
}

