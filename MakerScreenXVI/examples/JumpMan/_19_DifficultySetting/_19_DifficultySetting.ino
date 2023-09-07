/*
 * Here, you'll add a difficulty setting which changes over time to make the 
 * game more difficult.
 * 
 * Changing the speed means editing how quickly the position update occurs. 
 * To create a speed-changing effect, you will change how often position and
 * animation update, again replacing a number with a variable.
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
int points = 0; //jumps without hitting a wall

/*
 * Create two new variables to hold the durations of the animation
 * cycle length and the position cycle length. These being variables
 * will let you change the period of time between updates as the game
 * progresses. These variables will be checked against millis() and they are
 * measured in milliseconds.
  */
int animationDuration = 300; //ms between animations of JumpMan's legs
int runDuration = 600; //ms between moving JumpMan's position on the screen

long animationTimer; //tracks animation (custom character) updates
long runTimer;  //tracks position updates

void setup() {

  lcd.begin();
  
  lcd.backlightOn();

  lcd.createChar(0, run0); //partial stride character
  lcd.createChar(1, run1); //full stride character
  lcd.createChar(2, wall); //wall character in 'slot' 2 of custom characters
  lcd.createChar(3, crash); //splatted character against the wall
  lcd.createChar(4, jump);  //mid-jump icon

  pinMode(6,INPUT_PULLUP); //button to trigger jump

  //Start both timers equal to millis() so that the difference between
  //millis and the variables is 0. 
  animationTimer = millis();
  runTimer = millis();

}

void loop() {
  
  lcd.setCursor(0,0); //upper left of the screen
  lcd.print("Pt: ");
  lcd.print(points);

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

  // In the timer block for the animation, replace the 300 from
  // before with the variable animationDuration.
  if (millis() - animationTimer > animationDuration){
    runState = 1 - runState; //draw the next JumpMan animation
    animationTimer = millis();
    lcd.clear(); //clear the LCD to draw a new character
  }

  //Replace the 600 ms timer from before with the runDuration variable
  if (millis() - runTimer > runDuration){
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

    // If the jump is missed, reset the duration variables and points
    if ((position == wallPosition)&&(jumpState != 1)){

      points = 0;
      drawCrash();
      runDuration = 600; //reset to 'level one' speed
      animationDuration = 300; //reset to 'level one' speed
      points = points + 1;
    }

    /*
     * Increase the speed  and add a point whenever there is a successful 
     * jump. Change the '...duration' variables here to get smaller with each 
     * successful jump.
     */
    else if (position == wallPosition){
       /*
       * Decrease runDuration by 50 ms for each successful jump.
       * Decrease because the duration is the wait between updates- the lower 
       * the wait, the faster the change.
       */
      runDuration = runDuration - 50;

      /*
       * If runDuration gets too small, JumpMan will just be a blur on the
       * screen. Keep its value at a minimum of 100. 
       */
      if (runDuration < 100){
        runDuration = 100;
      }

      /*
       * To give the appearance that JumpMan is running faster, adjust the
       * animationDuration so that there are two leg animations per change
       * in position on the screen.
       */
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
 * Again you have made your game more fun and flexible by using a changing 
 * variable value in place of constant numbers- now this is starting to look
 * like a fun game!
 * 
 */

 //Need help? http://www.letsstartcoding.com/help
 
 //(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
