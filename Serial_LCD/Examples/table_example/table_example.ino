//
/// @file 	table_example.pde
/// @brief	Example
/// @details
/// @n @a 	Example for Serial_LCD Library Suite
/// @n @a	Serial_LCD Library Suite for 4D Systems uLCD-uLED-uVGA screens
/// @n 		on Arduino 0023 and 1.0, chipKIT MPIDE 0023, Wiring 1.0
///
/// @a 		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author 	Rei VILO
/// @author 	http://embeddedcomputing.weebly.com
/// @date	Jul 12, 2012
/// @version	release 132
/// @n
/// @copyright 	© Rei VILO, 2010-2013
/// @copyright 	CC = BY NC SA
/// @n		http://embeddedcomputing.weebly.com/serial-lcd.html
/// @n		http://github.com/rei-vilo/Serial_LCD
///
/// @see 	4D Systems Goldelox and Picaso SGC Command Set
/// @n		http://www.4dsystems.com.au/
///

// Core library - IDE-based
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#else // error
#error Platform not supported
#endif

// Include application, user and local libraries
#include "Wire.h"
#include "proxySerial.h"
#include "Serial_LCD.h"
#include "Graphics.h"

// test release
#if GRAPHICS_RELEASE < 318
#error required GRAPHICS_RELEASE 318
#endif

// === Serial port selection ===
#define mySerial Serial1
ProxySerial myPort(&mySerial); // hardware abstraction layer
Serial_LCD myLCD(&myPort); // LCD
// === End of Serial port selection ===

uint8_t x, y;
gTable myTable;


// Add setup code
void setup() {
  Serial.begin(9600);
  Serial.println("\n\n\n***");

  // === Serial port initialisation ===
  Serial.println("Serial1 with reset on pin 4");
  mySerial.begin(9600);
  myLCD.begin(4);  // 9600 at start-up
  // === End of Serial port initialisation ===

  // === Serial port speed change ===
  myLCD.setSpeed(38400);
  mySerial.begin(38400);
  // === End of Serial port speed change ===

  myLCD.setOrientation(0x03);
  myLCD.setTouch(true);
  myLCD.clear();

  myLCD.clear(grayColour);

  myLCD.gText(0, 0, "hello");
  delay(1000);


  uint8_t x, y;

  myTable.dDefine(&myLCD, 0, 0, myLCD.maxX(), myLCD.maxY(), 4, 4, blackColour, yellowColour);

  for (uint8_t i=0; i<4; i++) {
    myTable.draw(0, i, redColour);
    myTable.draw(i, 0, blueColour);
    myTable.draw(i, i, greenColour);
    delay(500);
  }

  myTable.enable();


}


// Add loop code
void loop() {

  if (myLCD.getTouchActivity()>0) {
    if (myTable.check()) {
      myTable.getIndex(x, y);

      Serial.print("Touched cell \t");
      Serial.print(x, DEC);
      Serial.print("\t");
      Serial.print(y, DEC);
      Serial.println();

      myTable.draw(x, y, whiteColour);
    } 
  } 
  else {
    delay(100);
  }

}






