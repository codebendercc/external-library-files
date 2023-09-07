/*
 * In this lesson, you'll add a delay to the previous code, which tells 
 * the microcontroller to pause for a certain amount of time. This will 
 * stop the flickering effect by leaving the words drawn on the
 * screen for longer before clearing them and re-writing.
 */

#include "MakerScreenXVI.h"

MakerScreenXVI lcd;

void setup() {

  lcd.begin();
  
  lcd.backlightOn();

}

void loop() {

  lcd.clear();

  lcd.print("Hello World!");

  /*
   * Delay tells the code to halt execution for a period of time. It is 
   * measured in milliseconds (ms), or thousandths of a second. This delay
   * is for 100 ms: 1/10th of a second.
   */
  delay(100);
}

/*
 * Controlling how fast your code executes is important. 
 * 
 * Because the processor is so fast, it can do things that humans cannot, 
 * but it does not operate with any natural delays like people do. 
 * To make it perform actions in order and for a given amount of time, 
 * you have to tell it when to pause. Imagine a song played at 100X speed-
 * it wouldn't sound very good. 
 * 
 * Delay is one method of doing so. It leaves the controller in whatever 
 * state it was in when you arrive at the command, and does nothing
 * for the duration of time you specify.
 * 
 * In this build, you'll see have different approaches for controlling 
 * timing that do not stop the processor from doing things while 
 * waiting for time to pass.
 */ 

 //Need help? http://www.letsstartcoding.com/help
 
 //(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
