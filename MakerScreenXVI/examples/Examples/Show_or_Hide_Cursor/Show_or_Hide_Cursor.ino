/* A demo of the cursor display method from the MakerScreenXVI library 
 * by Let's Start Coding. Make sure you have a Maker Screen and \
 * the MakerScreenXVI library installed to use this example.
 * www.letsstartcoding.com/libraries
 */

//Include the MakerScreenXVI library to use the methods inside it
#include <MakerScreenXVI.h>


//Include the MakerScreenXVI library to use the methods it holds.
MakerScreenXVI lcd;

void setup() {
  //Initialize the LCD
  lcd.begin();

  // Print a message to the LCD.
  lcd.print("Hello There!");
}

void loop() { 
  // Turn off the cursor for one second
  lcd.noCursor();
  delay(1000);
  
  // Turn on the cursor for one second
  lcd.cursor();
  delay(1000);
}

// Need help? http://www.letsstartcoding.com/help

//(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
