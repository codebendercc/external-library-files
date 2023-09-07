/*
 * Now that you know how to make your own graphics for the screen,
 * you will create the animations for your game.
 * 
 * Think of these animations like a 2-D cartoon- you combine still images
 * to get the effect of movement.
 */

#include "MakerScreenXVI.h"

MakerScreenXVI lcd;

/* 
 * To make it look like the Jump Man is running, make two different
 * icons and alternate between them. Call them run0 and run1
 */
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

void setup() {
  lcd.begin();
  
  lcd.backlightOn();

  // Create both characters, giving them the ID numbers 0 and 1
  lcd.createChar(0, run0);
  lcd.createChar(1, run1);
}

void loop() {

  lcd.clear();
  /*
   * Animate the character by alternating between the graphics. 
   * Set the cursor to draw over top of the old graphic each time.
   */
  lcd.setCursor(8,1);//cursor in 9th column, second row

  lcd.write(byte(0));// Write run0 - partial stride
  
  delay(500); //half a second in run0 position

 /*
  * The cursor will automatically try to draw a new character next to 
  * an existing character. You need to move the cursor back one space to
  * write over the last character.
  */
  lcd.setCursor(8,1);//cursor in 9th column, second row

  lcd.write(1); //Write run1 - full stride

  delay(500); //half a second in run1 position
}

/*
 * Now you have an automatic animation running!
 * 
 * If you remember the original loop version of hello world, you
 * can see why you reset the cursor position before each 
 * .write()- if you'd only done it the first time, then you'd get the two
 * characters side by side. 
 * 
 * Try commenting out the second .setCursor() method by placing two slash
 * (//) marks in front of line 65.
 * 
 * Try changing the delays in this program. See how your animation changes?
 */ 
 
 // Need help? http://www.letsstartcoding.com/help
 
 //(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
