/*
 * In this lesson, your animated character will run across Maker Screen.
 * 
 * The animation for running will continually cycle and 
 * the cursor for the character moves across the screen. 
 * 
 * The position of the runner will be a variable. The animation that
 * will be drawn next will be a variable that changes between 0 and 1.
 * 
 * Until now, there have been two separate tasks: making things move 
 * across the screen and making an animated character. In this lesson, 
 * the two combine. This combining of parts into a bigger finished 
 * product is an important skill in programming and it is not always easy.
 */
 
#include "MakerScreenXVI.h"

MakerScreenXVI lcd;

byte run0[8] = { //legs in partial stride
  0b00000,
  0b01100,
  0b01100,
  0b01100,
  0b01110,
  0b01100,
  0b01010,
  0b01010
};

byte run1[8] = { //legs in full stride
  0b00000,
  0b01100,
  0b01100,
  0b01111,
  0b01100,
  0b01100,
  0b01010,
  0b10001
};

int position = 0; //holds the changing position of the cursor

/*
 * Keep track of which custom character to draw next with a variable, 
 * which will either be 0 or 1. Create runState as a boolean variable type.
 * Bools (for boolean) can only be 0 or 1. Any other number will cause an
 * error. It's helpful to use bool when you only need 0 or 1 because it
 * immediately gives you or another programmer a clue about the variable:
 * it's not a counter, a decimal, or any large number. It's often a 'state'.
 */
bool runState = 0; //which running animation to draw (0 or 1).

void setup() {
  lcd.begin();
  
  lcd.backlightOn();

  lcd.createChar(0, run0);//put run0 array character into 'slot' 0
  lcd.createChar(1, run1);//put run1 array character into 'slot' 1

}

void loop() {

  lcd.clear();
  
  lcd.setCursor(position,1);//cursor in 'position' column, row 1

 /*
  * This lcd.write replaces a number with a variable. runState will equal
  * either 0 or 1. If variables in place of numbers are confusing, imagine
  * this runState as a 0.
  */
  lcd.write(runState); // Draw custom character

  delay(500); // Hold runState on the screen for half a second

  
  runState = 1 - runState; //update runState
  /*
   * Because runState will be 0 or 1, this expression switches the
   * value from either of these to the other. 0 becomes 1 and vice
   * versa. 1-0 = 1 / 1-1 = 0. 
   */

  /*
   * Don't increment position by 1 each time the loop runs: it 
   * would look strange if each animation frame was in a new position. 
   * Change it every time runState becomes 0, making the position change 
   * after both icons are displayed. Because runState alternates between 
   * 0 and 1, the change will occur every other loop.
   */
  if (runState == 0){
    position = position + 1; //update position every other animation update
  }

  if (position == 16){ //reset position to 0 when you reach the screen's edge
    position = 0;
  } 
}

/*
 * Now your code is starting to resemble a game!
 * 
 * This program shows even more integration between variables- the
 * position variable only changes when the runState variable has a certain
 * value.
 * 
 * Try changing the delay to see how it effects the animation
 * 
 * In the next lesson, you'll add a jump feature based on a button press
 */ 
 
//Need help? http://www.letsstartcoding.com/help

//(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
