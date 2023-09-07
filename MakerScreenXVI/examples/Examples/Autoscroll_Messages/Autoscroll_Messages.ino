/*A demo of the autoscroll method from the MakerScreenXVI library 
 * by Let's Start Coding. Make sure you have a Maker Screen and \
 * the MakerScreenXVI library installed to use this example.
 * www.letsstartcoding.com/libraries
 */

//Include the MakerScreenXVI library to use the methods it holds.
#include <MakerScreenXVI.h>


//Create an object named lcd in the MakerScreenXVI library
MakerScreenXVI lcd;

void setup() {
  //Initialize the LCD
  lcd.begin(); //Columns, Rows
}

void loop() {
  //Turning on the autoscroll allows the screen to show a longer message
  lcd.autoscroll();
  
  // set the cursor to the upper left of the screen
  lcd.setCursor(0, 0);
  
  //Print your long message
  lcd.print("Hello & Welcome to the World of Coding!");
  
  delay(500);//pause half a second between shifts of the message

}

// Need help? http://www.letsstartcoding.com/help

//(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

