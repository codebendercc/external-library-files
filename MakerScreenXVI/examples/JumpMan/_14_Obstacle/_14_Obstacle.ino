/*
 * This lesson is about adding in a new feature to the game- an obstacle
 * for JumpMan to jump over. This wall will become the basis for levels,
 * points, lives, and the rest of the game. 
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


//A new graphic for the wall element using createChar
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

//A graphic for JumpMan when he runs into the wall-a splatted character
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

int position = 0;  //holds the changing position of the cursor
bool runState = 0; //which running animation to draw (0 or 1).
int jumpState = 0; //0 ='pre-jump', 1 ='mid jump', 2='post jump'
bool buttonState = 0; //will equal 1 after press, 0 after jump completed

long animationTimer; //tracks animation (custom character) updates
long runTimer; //tracks position updates

void setup() {

  lcd.begin();
  
  lcd.backlightOn();

  lcd.createChar(0, run0); //partial stride character
  lcd.createChar(1, run1); //full stride character
  lcd.createChar(2, wall); //wall character in 'slot' 2 of custom characters
  lcd.createChar(3, crash); //splatted character against the wall

  pinMode(6,INPUT_PULLUP); //button to trigger jump

  //Start both timers equal to millis() so that the difference between
  //millis and the variables is 0. 
  animationTimer = millis();
  runTimer = millis();

}

void loop() {
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

  /*
   * After you've set the position for JumpMan and drawn
   * him, set the cursor to the bottom right of the screen and
   * draw the wall. Because of where it is positioned, you'll draw the 
   * wall thousands of times a second- as fast as Maker Board can. 
    */
  lcd.setCursor(14,1); //cursor near right side, second row
  lcd.write(2);//char in slot 2 is the wall

  if ((digitalRead(6) == LOW)&&(jumpState == 0)){
    buttonState = 1; //1 means a press is recorded
  }

  //Button is unpressed and a jump has been completed
  if ((digitalRead(6) == HIGH)&&(jumpState == 2)){
    jumpState = 0;
  }

  if (millis() - animationTimer > 300){//Update animation every 300 ms
    runState = 1 - runState; //draw the next JumpMan animation
    animationTimer = millis();
    lcd.clear(); //clear the LCD to draw a new character
  }

  if (millis() - runTimer > 600){ //Every 600 milliseconds, run this...
    position = position + 1; //move character one space to the right
    
    if (position == 16){//reset position to 0 when you reach the screen's edge
      position = 0;
    }

    //if button has been pressed and a jump hasn't been initiated...
    if ((buttonState == 1)&&(jumpState == 0)){
      jumpState = 1;
      buttonState = 0; //reset button to 'unpressed' in the code
    }
    else if (jumpState == 1){//true if jumpState set to one in the last loop
      jumpState = 2; //jump is marked as complete
    }

    /*
     * Create a check to see if JumpMan cleared the wall.
     * If the 'position' = 14 (the location of the wall) and the 
     * jumpState is NOT 1 (mid-jump) then JumpMan has hit the wall.
     */

    /*
     * This 'if' statement introduces a new type of booleam operator:
     * the 'not' operator. You use this operator as an exclamation in 
     * front of the equals sign when you want to test 'not equal to'.
     * 
     * The logic feels reversed, but the 'if' statement is asking "is it 
     * true that jumpState is NOT equal to one?". If that is true AND the
     * position variable equals 14, the 'if' statement executes.
     */
    if ((position == 14)&&(jumpState != 1)){
        drawCrash(); //function for drawing a crash
    }
    /* 
     * drawCrash() is a function- a block of code that always executes
     * together. This function has one purpose: draw the crash event. 
     * Rather than writing every line of code here that makes that happen,
     * you can create a function in your code and name it. Whenever you
     * 'call the function', that group of code all executes together.
     * 
     * Functions exist outside of void setup and void loop, but when you
     * call them, the code still runs in the place where you called the 
     * function. When it's complete, the code loop continues from just
     * below the function call. See this function around line 188.
     */
     
    lcd.clear();
        
    runTimer = millis();
  }
}

/*
 * When you make a function, you name it. This function is named drawCrash. 
 * Void is the type- it means the function does not produce any data, it
 * just runs its code. You can also send information to the function with\ 
 * arguments, like you do with methods. drawCrash doesn't need any  
 * arguments, it just needs to be told 'run'.
 * 
 * Everything between the { and } of drawCrash is the function.
 */
  
void drawCrash(){ //draw JumpMan crashed into a wall
  
  lcd.clear(); //clear all the characters on the screen

  lcd.setCursor(13,1);//set cursor next to the wall
  
  lcd.write(3); //draw the 'crashed' icon from custom character slot 3

  //Move the cursor to the location of the wall and draw it again. 
  lcd.setCursor(14,1); //wall location
  lcd.write(2); //draw the wall
  
  delay(1000); //show the crash for one second

  // Reset the variables to put the JumpMan back at the starting point
  position = 0;
  buttonState = 0;
  jumpState = 0;
}

/*
 * This lesson showed how to use a function to insert a block of code 
 * somewhere. Functions are useful when your code performs the same 
 * set of actions over and over in the program. Rather than write the same 
 * series of steps over and over, put all of the steps into a function
 * and call the function when you need those steps to run.
 * 
 * Functions are also useful for inserting complete blocks of code 
 * into pre-existing programs, as you did here. Adding in new blocks
 * can be difficult when you have many of code lines to work 
 * through, and functions make it convenient to simplify your code.
 * 
 * Try typing drawCrash(); just below the line position = position + 1.
 * The game won't be very fun, but you'll see how you can activate a 
 * whole series of events with the function.
 * 
 * This is starting to look like a real game- the character is moving 
 * around and can jump and there is an obstacle to avoid. Now that you 
 * have the basic machanics down, you can start adding in features like 
 * points and difficulty or improve the look of the game.
 */ 
 
 // Need help? http://www.letsstartcoding.com/help
  
 // (c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
 
