/*
 * In this lesson, you will re-write the program from the 
 * previous lesson to use a timer instead of delay. This allows you to
 * check for the button press more frequently and make sure presses are
 * recorded.
 * 
 * Using delay() is helpful for projects that aren't based on precise
 * timing, but they can cause issues like missing a button press. 
 * 
 * You will create a button state variable to track when the button has 
 * been pressed. The button press will change the variable value. Later in
 * the program, the variable value is checked. This all happens so quickly
 * that it will appear instant, but keep in mind the button press doesn't
 * trigger a jump directly, it triggers a variable change that triggers a 
 * jump. If the press triggered a jump directly, the man would jump up and
 * back down before position moved him over on the screen.
 * 
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

int position = 0;  //holds the changing position of the cursor
bool runState = 0; //which running animation to draw (0 or 1).
int jumpState = 0; //0 ='pre-jump', 1 ='mid jump', 2='post jump'

/*
 * buttonState is changed by a button press. You may not be holding the 
 * button at the exact time the code checks for a button press, so 
 * buttonState will change to 1 when you press the button and hold that
 * value. When buttonState is checked by the code, it will 'remember'
 * your button press.
 */
bool buttonState = 0; //will equal 1 after press, 0 after jump completed

/*
 * A timer variable tracks how much time has passed since the last time 
 * you updated the position variable.
 */
long runTimer; //used for tracking position updates
/*
 * This variable is a 'long' type, because the function
 * that gets the time, millis(), is measured in milliseconds, so the 
 * numbers get very large very quickly. 1 minute is 60000 ms.
 * The 'long' variable type can hold values over 2 million.
 */

void setup() {

  lcd.begin();
  
  lcd.backlightOn();

  lcd.createChar(0, run0);
  lcd.createChar(1, run1);

  pinMode(6,INPUT_PULLUP);

  
  runTimer = millis(); //set runTimer = to the built-in millis() timer

  /*
   * millis() is a built-in counter. It is always counting up when an 
   * Arduino program is running. You can compare this counter to a 
   * variable and see how much time has passed. 
   * For example, when you start a program, millis() equals 0. 
   * After 1 second, it equals 1000 no matter what is happening in the 
   * program (delays included). If you create a variable named "timer"
   * and declare it as timer = millis(), then the difference between timer
   * and millis is 0 (they are the same number). 
   * After one second, millis has increased by 1000. "timer" has not. If 
   * you write code that checks the value of millis()-timer, the difference
   * will be 1000. 
   * 
   * This is a way to check how much time has passed without pausing the 
   * program to do it. 
   */ 
}

void loop() {
  /* 
   * Loop is split informally into two parts from here onward. One part
   * happens as quickly as possible. The second part only happens when
   * it is true that there is a difference of 600 or greater between 
   * millis() and the variable runTimer: every 600 milliseconds.
   */
  
  /*
   * First in loop, outside the timer check section, are
   * all the drawing parts, since you want them to update regularly
   * as you change the runState and position variables every now
   * and again. The clearing of the LCD is now moved to places where
   * you change what will be drawn, so that the flicker from 
   * clearing the screen is minimal.
   */
   
  //Set the cursor depending on the state of the character
  if (jumpState == 0){
    lcd.setCursor(position,1);
  }
  else if (jumpState == 1){
    lcd.setCursor(position,0);
  }
  else if (jumpState == 2){
    lcd.setCursor(position,1);
  }

  lcd.write(runState);//draw the run0 or run1 animation

  if ((digitalRead(6) == LOW)&&(jumpState == 0)){
    /*
     * If the button has been pressed, 'flag' that with the buttonState
     * variable. 
     */
    buttonState = 1;//1 means a press is recorded
  }
  
  //Button is unpressed and a jump has been completed.
  if ((digitalRead(6) == HIGH)&&(jumpState == 2)){
    jumpState = 0;
  }

  /*
   * If it has been more than 600 ms since the animation changed, then 
   * perform the code inside this 'if' block.The rest of the program from 
   * here down is all within the 'if' statement. If millis()-runTimer is 
   * not more than 600,this code is skipped and the code at the top of 
   * the loop starts running again.
   */
  if (millis() - runTimer > 600){ //Every 600 milliseconds, run this...
    
    position = position + 1; //move character one space to the right
    runState = 1 - runState; //change which animation will be drawn next

    if (position == 16){ //reset position to 0 when you reach the screen's edge
      position = 0;
    }
    
    /*
     * If the button was pressed, buttonState = 1. If a jump has not yet
     * started, jumpState is 0. This is why you 'flagged' the button 
     * above- so that you don't have to be holding the button just as 
     * this check happens.
     */
    if ((buttonState == 1)&&(jumpState == 0)){
      jumpState = 1;

      buttonState = 0; //reset buttonState after starting the jump
    }

    /*
     * Use an else if here, because if you set jumpState to 1 above,
     * then let this code run, it will be advanced to 2 before
     * anything can happen when the above if block is triggered!
      */
    else if (jumpState == 1){ //true if jumpState set to one in the last loop
      jumpState = 2; //jump is marked as complete
    }
    
    lcd.clear(); //clear the LCD to draw a new character

    /*
     * Reset the runTimer variable to the current millis() value 
     * to keep track of when you should increase the position. If
     * you do not do this update, then the next time loop is run 
     * through, the millis()-runTimer check will still think you
     * are measuring from a smaller value of runTimer and will execute on
     * each loop. The first position change would work and after that the 
     * runner would zip across the screen!
      */
    runTimer = millis();//update runTimer to current millis() value.
  }
}

/*
 * This lesson has turned out to be quite extensive! It's very important 
 * for code which needs to update regularly not to be constrained by the 
 * pauses caused by delay().
 *  
 * The microcontroller can't do more than one thing at a time; it's 
 * designed to run one instruction after another.  
 * 
 * By creating a large 'if' statement that only happens every 600
 * milliseconds, you created a second, less frequent set of checks while
 * you maintained the speed you need for very responsive code, like 
 * catching a button press every single time.
 * 
 * Another new concept: using a new state to record when the button got 
 * pressed. That allows you to note that the button was pressed, but not
 * take action on it until later in the code, when you were able to update
 * position.
 * 
 * Creating and using all these state variables is an important 
 * part of writing code. They allow you yo keep track of the virtual world
 * that you build. They are like taking a note for later, then going to
 * check it- very helpful . 
 */
 
 //Need help? http://www.letsstartcoding.com/help
 
 //(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense 
