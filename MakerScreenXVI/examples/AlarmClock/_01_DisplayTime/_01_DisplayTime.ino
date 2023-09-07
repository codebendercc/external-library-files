/*
 * In these lessons, you are going to build a digital alarm clock.
 * The starting point for this goal is displaying the time
 * on the screen. 
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
MakerScreenXVI lcd; //create an object named lcd for MakerScreenXVI
/*
 * In this case, 'MakerScreenXVI' is the type of thing you're going
 * to use, and 'lcd' is the name of the screen connected to the
 * hardware. You can name an object almost anything.
 */

// Create three variables for time: hours, minutes, and seconds
int seconds = 20;
int minutes = 40;
int hours = 10;

// Create a central 'timer' variable. It is a 'long' type.
long timer;
/*
 * This variable has a different 'type' from the other variables you
 * have seen, which were 'int's- it is a 'long'. Different variables
 * have different types so they can store different sorts of numbers.
 * 'long' means that it can store very big numbers, which is
 * important, because millis(), which you will use with timer, has
 * very large numbers as outputs
 */

/*
 * Arduino is designed with two specific places for code to run: 
 * void setup() and  void loop(). Everything bewtween the { and the } 
 * of void setup() is run one time. It's where you put code that starts 
 * devices or initializes settings.
  */
void setup() {
  
  lcd.begin(); //.begin() method sets the screen up to accept commands
  
  /* 
   *  Methods are how you use objects, they tell the instance to do
   * something. In this case, telling the object named lcd to 
   * initialize itself and become ready to accept input.
   */
    

  /* The backlightOn() method turns on the LED embedded in the screen. 
   * Place two slashes (//) in front of this line to 'comment it out'.
   * Re-upload the code and see what it looks like without backlight.
   */
  lcd.backlightOn();

  /*
   * This next line is something new. What you do is take 'timer' and
   * set it to the value that millis() measures. Millis() is a 
   * function in arduino that tells you how many milliseconds
   * have passed since the board was powered on. Set timer equal to it
   * so you can measure the difference between millis() now and later.
    */
  timer = millis();
}

/*
 * After setup() is loop(). Code written between the { and } of 
 * void loop() executes over and over as long as the Maker Board is 
 * powered on. Most code you write will be in loop, as you usually want
 * continuous operation. Loop can run millions of times per second!
 */
 
void loop() {
  /*
   * This next bit of code is another new one. It is what is called
   * an 'if' block. What it does is check the expression contained 
   * in the parentheses after 'if' and finds out if it is true. In
   * the below, the check is whether millis(), the current time in
   * milliseconds since the board started, minus timer, the time
   * when you last set that variable, is greater than 1000. Then, 
   * the code contained within the {} is executed only if the 
   * check was true.
    */
  if (millis() - timer >= 1000){
    // If the time since the last check was more than a second,
    // increase 'seconds'
    seconds = seconds + 1;
    // Reset the timer, to keep track of when the last 
    // second was added
    timer = millis();
  }
  /* 
   * The above chunk of code forms the basis of othe whole clock, by
   * keeping track of seconds at the real time level.
    */
    
  /*
   * Next, check if enough seconds have passed that can mark
   * a whole minute gone by. The >= symbol says to do what is in the
   * {} if seconds is at or past 60.
    */
  if (seconds >= 60){
    /*
     * If so, take off a minute's worth of seconds. You do this
     * this way incase extra time has passed (if there are 61
     * seconds, don't want to throw away that extra second!)
      */
    seconds = seconds - 60;
    minutes = minutes + 1;
    }

  // Do the same sort of thing for minutes and hours that
  // you did for seconds and minutes:
  if (minutes >= 60){
    minutes = minutes - 60;  // Reduce 'minutes' by 60
    hours = hours + 1; // Increase hours by 1
  }
  /*
   * Once you've taken care of counting hours, minutes, and seconds,
   * you can put them on the display.
   */ 

  lcd.clear(); //Clear the display of previous information
  lcd.print(hours); //write the value of the 'hours' variable
  lcd.print(":"); //insert a colon
  
  // For minutes less than 10, you want to show a 0 before the actual
  // number, so use another if statement
  if (minutes < 10){
    lcd.print("0");
  }
  
  lcd.print(minutes);//print the value of the 'minutes' variable
  lcd.print(":");
  
  // Print the seconds. Second should include leading 0s like minutes
  if (seconds < 10){
    lcd.print("0");
  }
  lcd.print(seconds);//print the value of the 'seconds' variable

  /*
   * Then, once all that is on the screen, put in a short delay.
   * You do this because the loop() code executes as fast as possible,
   * so clearing and re-writing the time would look very flickery
   * without a little time for the display to stay on
    */
  delay(300);
  // 300 ms is 30% of a second, so it won't interfere with the
  // clock, since the minimum check you make is for 1000 ms
}

/*
 * You saw many new things in this lesson: 
 * new variable types so that you can use millis(), and 
 * millis() itself! You learned about using 'if'
 * statements to make checks and run specific code under certain
 * circumstances, and about how the conditional statements used in
 * if blocks work.
 * 
 * This project looks like a very basic clock with its segments of 
 * seconds, minutes, and hours, but it's really a timer measuring its 
 * own run time.
 * 
 * In the next lesson,you will alter this code so that it 
 * tracks hours properly like a clock, cycling through the 12 hour
 * cycle with AM/PM.
 */

// Need help? http://www.letsstartcoding.com/help

// (c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

