/* Using a Maker Screen, learn to write a message with code.
 * Be sure that you have the library from Let's Start Coding
 * to complete this program. www.letstartcoding.com/libraries
 */

// include the library to use its methods.
#include <MakerScreenXVI.h>

//Create an object for the MakerScreenXVI library named 'lcd'
MakerScreenXVI lcd;

void setup() {
  //Always type the lcd's name followed by .begin() in the void setup()
  lcd.begin(); //.begin() method 'turns on' a number of settings in the library
  
  lcd.print("Hello There!"); //the .print() method sends a message to the lcd.
  
  lcd.setCursor(0,1);//set the cursor to the first column (0), second row (1)
  
  lcd.print("LetsStartCoding");
}

void loop() { //No code needed in the loop!
}

//Try a different message. What happens when a message is longer than the screen? 

//Remove one of the quotes (") from the message- what happens?

// Need help? http://www.letsstartcoding.com/help

// (c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
