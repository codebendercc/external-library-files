/* 
 *  The Makerscreen library makes controlling the 16x2 LED screen
 *  easy, with a collection of simple methods to control it.
 *  
 *  NOTE: This code will not compile or upload! It's designed to show
 *  you how the methods work and what arguments they need. Take a method
 *  or two from the loop() and use them in an example program to see 
 *  how they work.
 */

// include the library code:
#include <MakerScreenXVI.h>

// initialize the library
MakerScreenXVI lcd;

void setup() {
  //Start the LCD
  lcd.begin();
}

void loop() {

  //Clear all text off the LCD screen
  lcd.clear();

  //Return the cursor to the (0,0) position (top left)
  lcd.home();

  //Write some text to the screen
  lcd.write("yourText");

  //Stop displaying text on the screen
  lcd.noDisplay();

  //Resume displaying text that is written to the screen
  lcd.display();

  //Remove the illustration of the cursor on the screen
  lcd.noCursor();

  //Show an illustration of the cursor on the screen
  lcd.cursor();

  //Set the screen to scroll text to the left
  lcd.scrollDisplayLeft();

  //Set the screen to scroll text to the right
  lcd.scrollDisplayRight();

  //Set the screen to automatically scroll text
  lcd.autoscroll();

  //Disable automatic scrolling of text
  lcd.noAutoscroll();

  //Turn on the LCD backlight
  lcd.backlightOn();

  //Turn off the LCD backlight
  lcd.backlightOff();

  //Switch the LCD backlight off if on, and on if off
  lcd.backlightToggle();

  //Display a bargraph on the LCD, plotting value as a proportion
  //  from lowLimit to highLimit
  lcd.bargraph(int value, int lowLimit, int highLimit);

  //Create a custom character, drawn as the binary array below
  lcd.createChar(2,[0b00110,
                    0b10110,
                    0b01100,
                    0b01111,
                    0b01110,
                    0b01011,
                    0b10000,
                    0b00000]
                            );

  //Set the cursor to the location (y,x)
  lcd.setCursor(int column number,int row number);

}

// Need help? http://www.letsstartcoding.com/help

// (c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense


