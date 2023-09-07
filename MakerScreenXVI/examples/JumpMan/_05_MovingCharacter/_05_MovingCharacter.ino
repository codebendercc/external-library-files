/*
 * In this lesson, you'll start making things move- an important component
 * for any video game.
 * 
 * You'll use a new variable called 'position' and represent the placement 
 * on the screen of a single character that you'll draw.
 */

#include "MakerScreenXVI.h"

MakerScreenXVI lcd;

// Declare your 'position' variable here, just like you did with the 
// counter before, and set its to value 0.
int position = 0; //position of cursor on the Maker Screen

void setup() {
  lcd.begin();
  
  lcd.backlightOn();

}

void loop() {

  lcd.clear();

  // This method allows you to set where the cursor is on the screen.
  // All lcd.print() commands will start from the cursor's position
  lcd.setCursor(position,0);
  /*
   * .setCursor() takes two arguments. The first is the column number-
   * it can move through 16 cells horizontally (0-15). The second is
   * which row. The first column is 0 and the first row is 0.
   */
  
  lcd.print("H"); // Print one letter to move around the screen.

  delay(1000); //pause 1 second between movements.

  /*
   * When the position variable's value changes, 
   * .setCursor will use that new value to place the cursor. lcd.print() 
   * then prints the character from the cursor's position.The delay means 
   * that the cursor will move once a second, to the right because the 
   * screen's horizontal cells are labeled from left to right with 
   * 0 through 15.
    */
  position = position + 1; //move the character by one to the right

}

/*
 * Now you have something that is beginning to look like a game: an item 
 * on the screen moving around. You can imagine this becoming a character.
 * 
 * The next challenge is that once the 'H' gets to the end of the screen, 
 * it walks off the edge. Because the screen is 16X2, meaning it has cells 
 * for characters arranged in 2 rows of 16 columns, when position gets to 
 * 16, theH is no longer on the screen. The computer doesn't 'know' the 
 * length of the screen and it will set the cursor anywhere you tell it.
 * 
 * To repair this, you code to check when position is at 16. You'll do 
 * this in the next lesson.
 * 
 * Try different characters in your lcd.print(). Change the delay() to 
 * make your character move faster. Think about how these code lines could
 * affect gameplay like choosing your character or changing levels (speed).
 */ 
 
 //Need help? http://www.letsstartcoding.com/help 
 
 //(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
