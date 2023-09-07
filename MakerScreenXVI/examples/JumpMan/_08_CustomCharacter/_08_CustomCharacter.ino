/*
 * It's time to start making the character look more like a game character. 
 * As mentioned before, programming is a process of adding layers of 
 * features to get what you want. Thinking that way,you could fully  
 * develop the mechanisms for moving a character into the game and then 
 * later add on the graphical parts.
 * 
 * It's best to approach everything in stages so that 'going back' to add 
 * in something doesn't force you to re-write all the other code which 
 * failed to account for some change.
 * 
 * This is not a perfect process and planning a software project is hard!
 * Pick a feature, break it into tiny bits, then solve each bit and see
 * where the errors pop up.
 */ 
 
/* 
 * Here, you'll create a custom character for the screen which will look 
 * like a little person and learn how display it with LCD.write()
 */

#include "MakerScreenXVI.h"

MakerScreenXVI lcd;

/*
 * To make a custom character, you need to be able to tell the screen
 * what to draw. You do this with a new type of variable, an array.
 * An array is a variable which holds more than one number in a list
 * or grid.
 */

/*
 * Below is an array named man. It contains bytes, so it's a 'byte' type 
 * array. The [8] means there are 8 bytes in the array. 8 is the number
 * of pixels tall a custom character can be.
 * 
 * The purpose of this variable is to set which pixels we want to be 'on' 
 * and which should be 'off' in the custom character. LCD characters are 
 * 8x5 pixel arrays.If you look at the 1s and 0s below, you will see that 
 * they fit that pattern. Any place there are 1s, the pixel will be dark.
 * 
 * The 0b in front of the 0s and 1s tells the computer that the digits it
 * is about to receive are binary. You always need those there for a
 * custom character.
 * 
 * To visualize how this works, you can play with this website:
 * https://omerk.github.io/lcdchargen/
 */

byte man[8] = {
  0b00000,
  0b01100,
  0b01100,
  0b01100,
  0b01110,
  0b01100,
  0b01010,
  0b01010
};

void setup() {
  lcd.begin();
  
  lcd.backlightOn();

  /*
   * Use the .createChar() method to tell the LCD to make a new 
   * character. There are 8 empty 'slots' on the LCD for custom
   * characters, which will be indexed by a number 0-7. 
   */
  lcd.createChar(0, man); //char named man in slot 0
  /*
   * The first argument, 0, is the  'slot' number and you'll use '0' to
   * refer to this custom character later. The second is the name of the 
   * array specifying the pixels for the character, as created above.
   */
}

void loop() {

  lcd.clear();

  /*
   * Displaying a custom character is different from the previous text 
   * and variabledisplays. You tell the screen which 
   * custom graphic to draw, which you do with the 'slot' number assigned
   * above around line 70.
   */
  lcd.write(byte(0));//write custom char 0 (man) to the screen
  /*
   * You are using a different method to put up the custom character:
   * .write(), not .print(). The 'byte' wrapped around the 0 is an 
   * artifact of much deeper parts of the C++ coding language. 
   * Just note that it needs to be there for 0, but not other numbers
   * in lcd.write().
   */
  
  delay(100);//Delay reduces flicker of the character
}

/*
 * In this lesson, we learned how to create a new character, so that
 * we can make graphics for our game. Next, we're going to see how to
 * use custom characters to make the animation for the game.
 * 
 * Before moving on, we noted the strange 'byte' in the comand we 
 * used to put up the custom character. Without going too deep into
 * things relating to libraries, try the program with just 
 * lcd.write(0), to see how it works without it.
 */ 
 
 // Need help? http://www.letsstartcoding.com/help
  
 // (c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
