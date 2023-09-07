/*
 * In this project, you'll build on the text message you showed in lesson
 * one and learn a new method from the MakerScreenXVI library to control
 * items on the screen.
 */

#include "MakerScreenXVI.h"

MakerScreenXVI lcd;

void setup() {

  lcd.begin();
  
  lcd.backlightOn();

}

void loop() {

  /*
   * If you tried putting the lcd.print into loop without anything
   * else, as suggested in the previous project, you saw how the 
   * text was pushed along the screen. That's because the screen 
   * starts writing wherever it left off after the last print.
    */

  // lcd.clear is a method that resets the screen, clearing off
  // old text and putting the cursor in the upper-left of the screen
  lcd.clear();

  lcd.print("Hello world!");

  

}

/*
 * In this project, you changed something on the screen over time by 
 * printing the message in the loop. You got a taste of how fast the 
 * Maker Board is: when you print and clear only, the screen flickers 
 * really badly. Next, you'll use a new function to slow it down.
 */ 

  
 //Need help? http://www.letsstartcoding.com/help

 //(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
