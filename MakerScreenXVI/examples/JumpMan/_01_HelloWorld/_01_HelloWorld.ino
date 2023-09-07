/*
 * Programming is a process of problem solving. You have a goal in mind 
 * and you break it into manageable steps to achieve that goal.
 * 
 * The goal you're after is the Jump Man game, so a good starting 
 * point is using the screen it's played on. This lesson starts with a
 * message printed on your Maker Screen
 */

//Include the MakerScreenXVI library so you can control Maker Screen.
#include "MakerScreenXVI.h"
/*
 * A library is a collection of tools that are prepared in advance 
 * to make programming easier. This one is for the Maker Screen 16x2 LCD 
 * screen. You'll use methods and commands built into the library to 
 * control it.Each method will be explain as it's introduced.
 */

 /*
  * Once the library is included the code, you need to be able to use
  * the resources it provides. To do that, create what's called
  * an object. An object is a specific, named version of the thing you
  * want to use the library's methods on. 
  */
MakerScreenXVI lcd;
/*
 * 'MakerScreenXVI' is the type of thing you'll use and 'lcd' is 
 * the name of the screen object connected to the hardware. 
 * You can name an object almost anything.
 */


/*
 * Arduino is designed with two specific places for code to run: 
 * void setup() and  void loop(). Everything bewtween the { and the } 
 * of void setup() is run one time. It's where you put code that starts 
 * devices or initializes settings.
 */
void setup() {
  // In setup, the screen is made ready to take commands,
  // which is accomplished by the 'begin' method:
  lcd.begin();
  /* 
   * Methods are how you use objects, they tell the object you named to 
   * do something. Here, you tell the object named lcd to set its 
   * settings to default.
   */

  /* The backlightOn() method turns on the LED embedded in the screen. 
   * Place two slashes (//) in front of this line to 'comment it out'.
   * Re-upload the code and see what it looks like without backlight.
   */
  lcd.backlightOn();

  /*
   * Once the screen is on and lit up, send it some text.Use the print() 
   * method. Note that characters must be within double quotes " "
    */
  lcd.print("Hello world!");
  /*
   * This method has something between the parenthesis: the text you 
   * want to show on the lcd.  Data given to a method this way are called
   * 'arguments'.They provide directions to an object through a method. 
   * The command 'print this on the screen' wouldn't make much sense 
   * without telling Maker Screen what to print!
   */
}  //This brace is the end of the void setup() section

/*
 * After setup() is loop(). Code written between the { and } of 
 * void loop() executes over and over as long as the Maker Board is 
 * powered on. Most code you write will be in loop, as you usually want
 * continuous operation. Loop can run millions of times per second!
 */
 
void loop() {
  /* 
   * No code is needed in loop for this program. The program has no 
   * changing parts. 
   */
}

/*
 * That's the end of your first program! To recap:
 * - You included a library so that you could use the screen 
 * - Created an object called 'lcd' to represent your screen 
 * - Activated Maker Screen and turned on the backlight 
 * - Sent Maker Screen some text to display
 * 
 * Here you typed lcd.print() in void setup so that it only wrote to 
 * the screen once. After you've run this example, 
 * try copying that line into loop() between the { and } and uploading it. 
 * Watch the screen closely for different behaviors
 * 
 * Try replacing 'world' in the .print method with your name or type a
 * whole new message. Re-upload your code to see the effects.
 */
  
  // Need help? http://www.letsstartcoding.com/help
   
  // (c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

