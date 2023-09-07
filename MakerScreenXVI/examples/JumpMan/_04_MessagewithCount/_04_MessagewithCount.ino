/*
 * In this project, you'll add a new feature: a counter
 * that ticks up once a second and displays the count on the screen.
 */

#include "MakerScreenXVI.h"

MakerScreenXVI lcd;

/* Here you create a variable- an important code concept. The Maker Board
 * can keep track of numbers for you, like it will with this counter. You
 * need to name that number so that you can refer to it in code. As an
 * example, think of the word 'temperature'. Temperature can have a value 
 * of 32, 65, 85, or 950, depending on where you are in the world. You use
 * the word 'temperature' to represent a number of degrees.
 */
int counter = 0;
/*
 * Here you create a new variable called  'counter' and set its value to 
 * '0'. You decide the nameof a variable, but it's good to give variables
 * clear, descriptive names like this. 
 * The 'int' part tells the program that the variable is of the integer 
 * type, meaning it can have whole number values like -10, 21, 3, 0, 89, 
 * but not any decimals like 3.2, 9.8 or -22.6. 
 * You only have to name the type once- when you 'declare the variable'. 
 * When you refer to this variable later, just use the name 'counter'
  */

void setup() {
  lcd.begin();
  
  lcd.backlightOn();

}

void loop() {

  lcd.clear();

  // Add a space after 'World!' so the numbers aren't smashed against it.
  lcd.print("Hello World! ");

  /*
   * This is where you show the value of the counter. This print is going
   * to show the value of counter, not the word "counter", so you don't
   * need the quotation marks around it.
   */
  lcd.print(counter);

  delay(1000); //pauses the program for 1 second after printing counter

  /*
   * Increase the counter each second, so after
   * the 1 second delay, add one to the 'counter' variable.
   * This line says "counter has a new value of counter + 1". If you just
   * typed 'counter + 1', that value wouldn't have a name. 
   */
  counter = counter + 1;
 
}

/*
 * In this project, you saw how the different pieces of the code can
 * interact. The variable can be used as an argument for the .print() method
 * In future projects, you'll see that variables replace numbers often. 
 * That practice allows for more flexibility in yur program.
 * 
 * Try changing the variable reassignment line to just:
 * counter + 1 
 * and see how it effects the display
 * 
 * Try putting quotes around "counter" in the lcd.print(). How does the
 * computer interpret that lcd.print() now?
 */
  
 // Need help? http://www.letsstartcoding.com/help
 
 // (c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
