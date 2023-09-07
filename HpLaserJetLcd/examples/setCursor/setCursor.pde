/*
  HpLaserJetLcd libraries - setCursor
 
 This sketch prints to all the positions of the LCD using the
 setCursor() method:
 
 See the web link below for latest wiring information.
 
 Originally created 2 April 2011
 
 This example code is in the public domain.

 http://arduino.cc/playground/Main/HpLaserJetLcd
*/

// include the library code:
#include <HpLaserJetLcd.h>

// these constants won't change.  But you can change the size of
// your LCD using them:
const int numRows = 2;
const int numCols = 16;

// initialize the library with the numbers of the interface pins
HpLaserJetLcd lcd(3, 5, 7, 8);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(numCols,numRows);
}

void loop() {
  // loop from ASCII 'a' to ASCII 'z':
  for (int thisLetter = 'a'; thisLetter <= 'z'; thisLetter++) {
    // loop over the columns:
    for (int thisCol = 0; thisCol < numRows; thisCol++) {
      // loop over the rows:
      for (int thisRow = 0; thisRow < numCols; thisRow++) {
        // set the cursor position:
        lcd.setCursor(thisRow,thisCol);
        // print the letter:
        lcd.print(thisLetter, BYTE);
        delay(200);
      }
    }
  }
}


