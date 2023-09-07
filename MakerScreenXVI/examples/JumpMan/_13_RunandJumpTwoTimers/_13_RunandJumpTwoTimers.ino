/*
 * In this lesson, you will add in  a second timer check to control the 
 * animation of JumpMan's legs. In the previous program, the animation 
 * update used the same timer as the position so that each time the
 * position was updated, the animation icon changed. With this program,
 * you can control their speeds independently.
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
bool buttonState = 0; //will equal 1 after press, 0 after jump completed

/*
 * A new timer variable can still be compared with millis(), since millis()
 * is constantly growing regardless of what happens in the program.
  */
long animationTimer; //tracks animation (custom character) updates
long runTimer; //tracks position updates

void setup() {

  lcd.begin();
  
  lcd.backlightOn();

  lcd.createChar(0, run0);//put run0 array character into 'slot' 0
  lcd.createChar(1, run1);//put run1 array character into 'slot' 1

  pinMode(6,INPUT_PULLUP); //button to trigger jump

  //Start both timers equal to millis() so that the difference between
  //millis and the variables is 0. 
  animationTimer = millis();
  runTimer = millis();

}

void loop() {
    //Set the cursor depending on the state of the character
  if (jumpState == 0){
    lcd.setCursor(position,1);}
  else if (jumpState == 1){
    lcd.setCursor(position,0);}
  else if (jumpState == 2){
    lcd.setCursor(position,1);}

  lcd.write(runState);//draw the run0 or run1 animation

  if ((digitalRead(6) == LOW)&&(jumpState == 0)){
    buttonState = 1; //1 means a press is recorded
  }
  
  //Button is unpressed and a jump has been completed
  if ((digitalRead(6) == HIGH)&&(jumpState == 2)){
    jumpState = 0;
  }

  /*
   * Implement the same kind of check you used with the runTimer check, 
   * but with the animationTimer variable. Make the time between updates 
   * 300 ms- half the time between position updates, so that the animation 
   * goes through both (full and partial stride) icons in each position.
   */
  if (millis() - animationTimer > 300){ //Every 300 milliseconds, run this...
    runState = 1 - runState;
    animationTimer = millis();

    lcd.clear(); //clear the LCD to draw a new character
  }

  if (millis() - runTimer > 600){//Every 600 milliseconds, run this...
    position = position + 1; //move character one space to the right

    if (position == 16){//reset position to 0 when you reach the screen's edge
      position = 0;
    }
    //if button has been pressed and a jump hasn't been initiated...
    if ((buttonState == 1)&&(jumpState == 0)){ 
      jumpState = 1; 
      buttonState = 0; //reset button to 'unpressed' in the code
    }
    else if (jumpState == 1){ //true if jumpState set to one in the last loop
      jumpState = 2; //jump is marked as complete
    }

    lcd.clear();//clear the LCD to draw a new character
        
    runTimer = millis();//update runTimer to current millis() value.
  }
}

/*
 * In this lesson, you added another timer so that animation is separate 
 * from position, which allows the JumpMan's legs to move at a different
 * pace than his position on the screen.
 * 
 * In the last program you had two 'sets of checks'. Now you have 3: 
 * One that happens as quickly as the Maker Board can do it, one that
 * happens every 300 milliseconds, and one that happens every 
 * 600 milliseconds. 
 * 
 * A pattern you'll notice is a 'reset' after an 'if' statement based on
 * a timer. Each time you complete the action associated with a timer, 
 * reset it. Without that, the (if millis()- animationTimer) statement 
 * will always be true.
 * 
 * Comment out the animationTimer = millis(); line in loop by placing
 * two slashes (//) in front of it. Upload the code to see the effect of
 * not resetting your timers.
 * 
 * Try changing the number in (millis()- animationTimer) to something
 * smaller. Do you find a number you think looks better?
 */ 

 // Need help? http://www.letsstartcoding.com/help

 // (c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
