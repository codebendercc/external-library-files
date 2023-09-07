/*
 * A demo of the display() and noDisplay() methods in the MakerScreenXVI
 * library form Let's Start Coding.They allow you to show or hide what's
 * on the screen without clearing it.
 * 
 * Make sure you have the Maker Screen library to do this program.
 * www.letsstartcoding.com/libraries
 */
 

//Include the MakerScreenXVI library to use the commands
#include <MakerScreenXVI.h>

//Create an object named lcd in the MakerScreenXVI library
MakerScreenXVI lcd;

void setup() {
  // set up the LCD
  lcd.begin();
  
  // Print a message to the LCD.
  lcd.print("Hello There!");
}

void loop() {
  // Turn off the display for a second
  lcd.noDisplay();
  delay(1000);
  
  // Turn on the display for a second
  lcd.display();
  delay(1000);//change the speed of the blink with these delays.
}

// Need help? http://www.letsstartcoding.com/help

//(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

