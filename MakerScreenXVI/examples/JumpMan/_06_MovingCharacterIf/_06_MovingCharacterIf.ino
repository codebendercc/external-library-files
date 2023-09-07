/*
 * The last lesson had an 'H' moving across the screen, but when it
 * got to the edge, it fell off the screen.
 * 
 * Instead, the 'H' should go back to the left hand side of the screen 
 * and start moving over again. An 'if' statement can check to see if
 * 'position' is 16 and, if it is, reset its value to 0.
 */

#include "MakerScreenXVI.h"

MakerScreenXVI lcd;

int position = 0;//posiiton of the cursor

void setup() {
  lcd.begin();
  
  lcd.backlightOn();

}

void loop() {

  lcd.clear();

  lcd.setCursor(position,0); //cursor in 'position', top row

  lcd.print("H");

  delay(1000);

  position = position + 1; //move the character by one to the right

  /*
   * There are only 16 cells across the screen, so you reset the position 
   * every time it reaches 16. Do this with an 'if' statement:
   */
  if (position == 16){ //test the condition in ( ) for true or false
    /*
     * An 'if' statement checks if some condition is true. If it is, it
     * executes a block of code following it in { }. If the statement
     * is not true, the 'if' statement's code in { } is passed over.
     * 
     * Here, check if 'position' is 16 (notice it is ==, not =) and if so:
      */
    position = 0; //reset position to 0
  } //This brace ends the 'if' statement's code
}

/*  
 *  In this lesson, you've been introduced to one of the most fundamental 
 *  parts of programming, the 'if' statement.
 *   
 *  An 'if' statement checks a condition and then does something if 
 *  that condition is true. This allows you to make your code branch and 
 *  "make decisions" based on tests. With this tool, you can build up many 
 *  options for what the program will do and 'let it decide' what to do.
 *  
 *  Without the ability to branch like this, code would have to do 
 *  the exact same thing over and over. The existence of 'if', and 
 *  other conditional statements like it are what make a programmed game 
 *  different from a movie, at the most basic level.
 *  
 *  Try changing the 'if' statement to reset when position is a different
 *  number than 16.
 */ 
 
 // Need help? http://www.letsstartcoding.com/help
  
 // (c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
