/*
 * Now that you've got the wall set as a variable, it's possible to
 * make use of the added flexibility. The most straightforward way
 * to do that is to make the wall change position every new 'level'.
 * 
 * To make this happen, you'll add in an additional piece of code 
 * when JumpMan is reset at the edge of the screen to move the wall.
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

  lcd.setCursor(wallPosition,1);
  lcd.write(2); //draw wall

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
   
    if (position == 16){ 
      /*
       * This is the reset for when JumpMan is at the edge of the
       * screen, so it is the most appropriate place to put the 
       * code for moving the wall. At the same time he starts a new 
       * level, the wall will be placed somewhere new.
       * 
       * The function 'random' is built in to the Arduino software, so you 
       * don't have to make it in your code. It takes two arguments as a 
       * range to choose from and returns a number between those arguments. 
       * The range 3-16 ensures that the wall won't be set too close to the 
       * left side of the screen or at some location not on the screen.
       * Random excludes the highest number from the range, so (3,16) is 3-15.
       */
      wallPosition = random(3,16); //place wall in column 3 through 15
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

    if ((position == wallPosition)&&(jumpState != 1)){//check for a crash
         drawCrash();
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
 * In this lesson, you took advantage of a feature (that the wall's
 * position was made into a variable in the code), and used it
 * to add an element in the game. 
 * 
 * The ability to make one change in one location is a benefit of variables. 
 */
  
 //Need help? http://www.letsstartcoding.com/help

 //(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

