/*
 * Now that you have an obstacle to jump over, you can make the location 
 * of the wall a variable. By doing so, your game is more flexible and 
 * fun!
 * 
 * It is common that programmers will start out with a basic, less
 * complicated version of a feature, like a 'hard coded' wall, and then
 * add more to it, like a little version of the process you're using to 
 * make this game. Building things one step at a time means you can focus 
 * on each part as you make it and solve problems one at a time. 
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

byte run1[8] = {//legs in full stride
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

int position = 0;  //holds the changing position of the cursor
bool runState = 0; //which running animation to draw (0 or 1).
int jumpState = 0; //0 ='pre-jump', 1 ='mid jump', 2='post jump'
bool buttonState = 0; //will equal 1 after press, 0 after jump completed

//Integer for the position of the wall
int wallPosition = 14; //wall can be placed from 0 to 15

long animationTimer; //tracks animation (custom character) updates
long runTimer;  //tracks position updates

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

  lcd.write(runState); //draw the run0 or run1 animation

  /* 
   * Draw the wall at its variable position. In the previous lesson, 
   * wallPosition was the number '14'. Now it's a variable that holds
   * a range of numbers from 0 to 15.
   */
  lcd.setCursor(wallPosition,1);
  lcd.write(2); //Draw wall

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
      jumpState = 1; //signal a jump
      buttonState = 0; //reset button to 'unpressed' in the code
    }
    else if (jumpState == 1){//true if jumpState set to one in the last loop
      jumpState = 2; //jump is marked as complete
    }

    /*
     * Again you will update the 'hard coded' wall position of 14 to the
     * variable wallPosition. One of the strengths of variables is that 
     * they can be set at the top of the code so that you don't have to
     * hunt through an entire program to find hard coded numbers that
     * you want to change. It is easy to miss one change and get strange
     * behavior.
     */
    if ((position == wallPosition)&&(jumpState != 1)){//check for a crash
         drawCrash();
    }

    lcd.clear();
        
    runTimer = millis();
  }
}


void drawCrash(){ //draw JumpMan crashed into a wall

  /*
   * You also need to update the drawing to reflect the position.
   * You can use wallPosition anywhere we want because you created 
   * it up top, above setup and loop. Variables like that are 
   * called global variables, and can be used in any function you
   * create. You can also create variables inside functions, but 
   * they pop into existance for that function, and then disappear
   * when you leave it.
   * That is also why yoy can change position, buttonState, and 
   * jumpState in this function. If you create a variable inside
   * loop() or setup(), or in a function, it only exists inside that
   * function and can't be used outside of it.
   * 
  /* 
   * You use some existing variables in your function drawCrash(). This is
   * possible only because you declared those variables outside of void setup
   * and void loop. 
   * 
   * Variables that exist outside of all functions are called global scope 
   * variables and any function can use them. 
   * 
   * Variables that exist only inside a function are called local scope 
   * variables. 
   * 
   * Going in-depth with scope is beyond this program, but it's an important
   * topic that you will learn more about as you write more code.
   */

  lcd.clear(); //clear all the characters on the screen

  //Move JumpMan back one position from the wall he hit and draw him splatted
  lcd.setCursor((position-1),1);
  lcd.write(3); //draw the 'splatted' JumpMan

  lcd.setCursor(wallPosition,1); 
  lcd.write(2); //draw the wall
  
  delay(1000);

  // Reset the variables to put the JumpMan back at the starting point
  position = 0;
  buttonState = 0;
  jumpState = 0;
}

/*
 * Now that you've made the placement of the wall variable, you can
 * update the variable and the wall will move. Update the variable around
 * line 70 and re upload the code to see the effect.
 * 
 * 
 * The wall in the last lesson was 'hard coded': the execution of the code is 
 * very specific and non-adaptive. It may be simpler when you code it, 
 * but changing it is difficult.
 * 
 * Try cutting the "int position = 0;" line and pasting it inside the void
 * loop() anywhere between the first { and last }. Try to upload your code.
 * You will see that the computer encounters an error when the function tries
 * to use the position variable.
 */
  
 //Need help? http://www.letsstartcoding.com/help

 //(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

