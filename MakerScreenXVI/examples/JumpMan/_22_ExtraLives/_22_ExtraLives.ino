/* 
 * Here you'll add a new game element, a heart, which will add a life to 
 * the player's total if they get collide with it in a jump. 
 * Hearts will show up randomly.
 * 
 * The decision of whether a heart will show up will be based on another
 * 'state variable' that will be set to 0 or 1 at each game level.
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

// Make an icon for the heart object, just like the others
byte heart[8] = {//heart for extra life
  0b00000,
  0b01010,
  0b10101,
  0b10001,
  0b01010,
  0b00100,
  0b00000,
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

bool heartState = 0;//0 = no heart available, 1 = heart available

int heartPosition; //which column is the heart in?

void setup() {

  lcd.begin();
  
  lcd.backlightOn();
  
  lcd.createChar(0, run0); //partial stride character
  lcd.createChar(1, run1); //full stride character
  lcd.createChar(2, wall); //wall character in 'slot' 2 of custom characters
  lcd.createChar(3, crash); //splatted character against the wall
  lcd.createChar(4, jump);  //mid-jump icon
  lcd.createChar(5, heart); //heart for extra lives

  pinMode(6,INPUT_PULLUP); //button to trigger jump

  startScreen();//run function for welcome screen

  //Start both timers equal to millis() so that the difference between
  //millis and the variables is 0.
  animationTimer = millis();
  runTimer = millis();

  /*
   * randomSeed() is a built-in function for Arduino that takes a 
   * reading from a pin and uses that reading to create a random number.
   * Without it, Maker Board will use the same pattern of 'random' 
   * numbers. randomSeed solves that problem. 
   */
  randomSeed(analogRead(A0));//take a reading from A0 to get a random number
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
  
  // Draw the heart right after drawing the wall, but only
  // if heartState is 1- if there is currently one available.
  if (heartState == 1){
    lcd.setCursor(heartPosition,0);
    lcd.write(5); //heart icon
  }
  
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

  if (millis() - runTimer > runDuration){//update position after runDuration
    position = position + 1;

    /*
     * In the 'if' block that resets the runner to the left of the 
     * screen, use random() to decide if a heart will be available
      */
    if (position == 16){
      heartState = 0; //by default there is no heart available
      
      if (random(0,10) < 3){//30% of the time...
        heartState = 1; //signal that a heart will be available
        heartPosition = random(3,16); // Place the heart randomly
      }
      
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
    
    /*
     * Just like checking for a collision with a wall, check for 
     * a collision with a heart if one is available.
     */
    if ((position == heartPosition)&&(jumpState == 1)){
      if (heartState == 1){
        lives = lives + 1; //add a life
        heartState = 0; //reset the heart variable
      }
    }
    
    if ((position == wallPosition)&&(jumpState != 1)){//if a jump is missed...
            drawCrash();
      
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

      else if (position == wallPosition){//if the jump is successful...
      runDuration = runDuration - 50; //speed each position up by 50 ms
      
      if (runDuration < 100){//keep runDuration at a minimum of 100
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

void startScreen(){

  lcd.clear();

  lcd.setCursor(2,0);//place cursor near the middle of Maker Screen
  lcd.print("Press button 6");
  lcd.setCursor(4,1);//Move to the next line and complete the message
  lcd.print("to play!");

  while (digitalRead(6) == HIGH){//while the button isn't pressed...
    delay(10); //wait 10 ms and check the 'while' again
  }
  
  while (digitalRead(6) == LOW){ //while the button is pressed...
    delay(10); //wait 10 ms and check the 'while' again
  }
  
  //After both 'while' loops have completed, clear the screen and complete
  //the function
  
  lcd.clear();
  
}

/*
 * Now the player can prolong their life by grabbing extra hearts. You may
 * see a pattern evolving here. To add a feature to the game, you are finding
 * the exact spot where something should occur: a collision (or not) and using
 * that moment in time to change a variable in the code. When all of these
 * moments come together, you have a dynamic game with lots of moving parts.
 */ 
 
 //Need help? http://www.letsstartcoding.com/help

 //(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

