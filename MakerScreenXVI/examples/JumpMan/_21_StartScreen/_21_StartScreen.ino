/*
 * In this lesson, you will add a start screen that comes up 
 * when the game is turned on and after a player gets a 'game over'.
 * 
 * The start screen will be a little title card that goes up on the
 * LCD, and will wait for a button press to start playing. You will
 * implement this as a function, like drawing the crash, and drop
 * it in wherever it should pop up.
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

byte wall[8] = { //wall obstacle
  0b11111,
  0b10101,
  0b11111,
  0b10001,
  0b11111,
  0b10101,
  0b11111,
  0b10001
};

byte crash[8] = { //crashed JumpMan
  0b00000,
  0b00011,
  0b01011,
  0b00111,
  0b00011,
  0b00011,
  0b00001,
  0b00001
};

//The new icon for the jump part of the animation.
byte jump[8] = { //mid-jump icon
  0b00110,
  0b10110,
  0b01100,
  0b01111,
  0b01110,
  0b01011,
  0b10000,
  0b00000
};

int position = 0;  //holds the changing position of the cursor
bool runState = 0; //which running animation to draw (0 or 1).
int jumpState = 0; //0 ='pre-jump', 1 ='mid jump', 2='post jump'
bool buttonState = 0; //will equal 1 after press, 0 after jump completed
int wallPosition = 14; //wall can be placed from 0 to 15
int lives = 3; //starting number of lives

int animationDuration = 300;//ms between animations of JumpMan's legs
int runDuration = 600; //ms between moving JumpMan's position on the screen

//Timers to update for each 'level'
long animationTimer;
long runTimer;

void setup() {

  lcd.begin();
  
  lcd.backlightOn();
  
  lcd.createChar(0, run0); //partial stride character
  lcd.createChar(1, run1); //full stride character
  lcd.createChar(2, wall); //wall character in 'slot' 2 of custom characters
  lcd.createChar(3, crash); //splatted character against the wall
  lcd.createChar(4, jump);  //mid-jump icon

  pinMode(6,INPUT_PULLUP); //button to trigger jump

  /*
   * The first time you want to put up the start screen is in setup,
   * after all the prepatory work, but before the game. You 
   * want it to be before the timers get set, so the guy won't jump
   * ahead right away due to the delay. Jump to the bottom of the code
   * to see what the function looks like and what is in it.
   */
  startScreen();

  //Start both timers equal to millis() so that the difference between
  //millis and the variables is 0.
  animationTimer = millis();
  runTimer = millis();

}

void loop() {
 
  lcd.setCursor(0,0);
  lcd.write(4); //Show the jump icon alongside the lives counter
  lcd.print(lives);

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

  if (jumpState == 1){ //if jump has been activated..
    lcd.write(4); //draw JumpMan mid-air
  }
  else{
    lcd.write(runState); //draw the run0 or run1 animation
  }

  lcd.setCursor(wallPosition,1);
  lcd.write(2); //draw wall

  if ((digitalRead(6) == LOW)&&(jumpState == 0)){
    buttonState = 1; //1 means a press is recorded
  }
  
   //Button is unpressed and a jump has been completed
  if ((digitalRead(6) == HIGH)&&(jumpState == 2)){
    jumpState = 0;
  }

  if (millis() - animationTimer > animationDuration){
    runState = 1 - runState; //draw the next JumpMan animation
    animationTimer = millis();
    lcd.clear(); //clear the LCD to draw a new character
  }

  if (millis() - runTimer > runDuration){ //update position after runDuration
    position = position + 1;
    
    if (position == 16){
      wallPosition = random(3,16); //place wall in column 3 through 15
      position = 0;
    }

    //if button has been pressed and a jump hasn't been initiated...
    if ((buttonState == 1)&&(jumpState == 0)){
      jumpState = 1; //signal a jump
      buttonState = 0; //reset button to 'unpressed' in the code
    }
    else if (jumpState == 1){ //true if jumpState set to one in the last loop
      jumpState = 2; //jump is marked as complete
    }

    if ((position == wallPosition)&&(jumpState != 1)){//if a jump is missed...
            drawCrash();

    /*
     * Remove the speed reset. Reset the speed of the runner when the game 
     * resets after all lives are lost.
     */
      
      lives = lives - 1;//decrease lives by one with each crash

    /*
     * If the player has run out of lives, print a message to the screen
     * and reset the game.
     */
      if (lives == 0){ //if out of lives...
        // Clear the LCD and write "Game Over" to it
        lcd.clear();
        lcd.setCursor(4,0);
        lcd.print("Game Over");

        delay(2000);//wait two seconds to restart the game

        //Reset all state variables:
        lives = 3;
        position = 0;
        buttonState = 0;
        runState = 0;

        //Reset speeds to 'level one'
        runDuration = 600; 
        animationDuration = 300;

        /*
         * Show the start screen after the 'game over' is shown so that the 
         * next game won't start until the button is pressed again
         */
        startScreen();

      } 
    }

       else if (position == wallPosition){

      runDuration = runDuration - 50;//increase speed that JumpMan moves on screen

      /*
       * If runDuration gets too small, JumpMan will just be a blur on the
       * screen. Keep its value at a minimum of 100. 
       */
      if (runDuration < 100){
        runDuration = 100;
      }

      animationDuration = runDuration/2; //Move legs twice per position change
    }

    lcd.clear();
        
    runTimer = millis();
  }
}


void drawCrash(){
  lcd.clear();

  lcd.setCursor(position-1,1); //move cursor one position left of the collision
  lcd.write(3);//draw 'splatted' JumpMan

  lcd.setCursor(wallPosition,1);
  lcd.write(2); //draw the wall

  delay(1000);

  // Reset the variables to put the JumpMan back at the starting point
  position = 0;
  buttonState = 0;
  jumpState = 0;
}

/*
 * Defining the function for startScreen is just like drawCrash(). Unlike
 * the rest of the program, the order of the functions doesn't matter-
 * startScreen could come before drawCrash(). When the function is called,
 * the code jumps to it, runs it, and jumps back into place.
 */
void startScreen(){

  lcd.clear(); //clear all of the old characters

  lcd.setCursor(2,0); //place cursor near the middle of Maker Screen
  lcd.print("Press button 6");
  lcd.setCursor(4,1); //Move to the next line and complete message.
  lcd.print("to play!");

  /*
   * A 'while' loop works a bit like an 'if' loop- it checks to see if
   * the condition in parenthesis is true, and if it is, does the
   * code in the following block. A while loop is different from
   * 'if'because 'if' runs it's code block code once per loop, but 'while' 
   * checks its condition again after running. If the condition is still true,
   * it will run again, creating a loop of itself until the condition is no
   * longer true.
    */
  while (digitalRead(6) == HIGH){//While the button isn't pressed...
    /*
     * Inside the 'while' loop, delay 10 milliseconds and check the 'while'
     * again. The program will just sit here as long as the button is not 
     * pressed.
     */
    delay(10);
  }

  /*
   * As the player presses the button, the code waits for them to let it go
   * so that the start screen doesn't jump to the game with the button held, 
   * which would make JumpMan jump immediately. In this 'while', delay as 
   * before.
    */
  while (digitalRead(6) == LOW){//While the button is pressed...
    delay(10);
  }

  /*
   * Combining the 'while' loops creates a series of steps for the code to
   * complete before finishing the function. The button must be pressed and
   * then released to complete the startScreen(); function.
   */

  /*
   * Once the button is pressed and released, this function is 
   * complete and the code returns to the line of code just below the
   * function call (startScreen()).
    */
  lcd.clear();//Clear the 'intro message'from the screen.
}

/*
 * You saw a 'while loop in this lesson. While loops are very useful
 * whenever you want to keep doing something until a condition is met. You
 * need to pay close attention to how they work, because a loop like:
 * 
 * while(number<10){
 *    number = 9; 
 * }
 * will continue forever once it begins.
 * 
 */
  
 //Need help? http://www.letsstartcoding.com/help

 //(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
