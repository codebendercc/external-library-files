/*
 * In this lesson, you'll add a points counter which keeps track of how many 
 * consecutive successful jumps have been made.
 */
 
#include "MakerScreenXVI.h"

MakerScreenXVI lcd;

byte run0[8] = {//legs in partial stride
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

byte wall[8] = {//wall obstacle
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
int wallPosition = 14; //wall can be placed from 0 to 15

/*
 * Create a variable to hold the number of points scored, which is the number 
 * of consecutive jumps made without running into the wall.
 */
int points = 0; //jumps without hitting a wall

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

  /*
   * Before drawing JumpMan or the wall, write the 
   * current point total at the top left of the screen. Write it
   * first so that if the man jumps up into where the total is
   * put, he will be written over top of it.
    */
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
      points = 0; //remove all points if you crash
      drawCrash();
    }

      //If you've reached the wallPosition and haven't crashed, you must have
      //survived the leap. Add a point
    else if (position == wallPosition){
       points = points + 1;
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
 * Since you've written good, flexible code, adding in a points counter was 
 * fairly simple. All you had to do was find the right place where
 * the state of the program signaled a completed jump.
 * 
 * It was also simple to add in the display of the counter. The only
 * clever part was making sure the counter was put in before the 
 * character, so that the character would be drawn on top, if they
 * ended up in the same position on the screen.
 */

  
 //Need help? http://www.letsstartcoding.com/help
  
 //(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
