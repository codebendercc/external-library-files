 
 /* Demonstrate the scrollDisplayLeft() and scrollDisplayRight() methods 
  * of the MakerScreenXVI library by Let's Start Coding. Move your 
  * message back and forth across a screen.
  * 
  * Make sure you have the Maker Screen library to do this program.
  * www.letsstartcoding.com/libraries
  */

//Include the MakerScreenXVI library to use its methods
#include <MakerScreenXVI.h>

//Create an object named lcd in the MakerScreenXVI library
MakerScreenXVI lcd;

void setup() {

  lcd.begin();//.begin() method sets the default screen settings for you
  
  lcd.print("Hello There!");
  delay(1000); //Hold the message still for one second
}

void loop() {
  
  //The 'for' loop runs 12 times because there are 11 characters in the
  //message above. That allows it to shift all the way off the screen.
  for (int position = 0; position< 12; position++) {     
    //Calling the .scrollDisplayLeft method moves the string one space over
    lcd.scrollDisplayLeft(); 
    delay(200);  //length of the pause between 'shifts'.
  }

  // Scroll 28 positions (string length + display length (16)) to the
  // right to move the message offscreen right:
  for (int position = 0; position < 28; position++) {
    lcd.scrollDisplayRight();
    delay(200); //Try changing these delays to see the effect
  }

  // scroll 16 positions (display length + string length) to the left
  // to move it back to center:
  for (int position = 0; position < 16; position++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    delay(200);//length of the pause between 'shifts'
  }
  
  delay(1000);  //Hold the message on screen for one second
}

