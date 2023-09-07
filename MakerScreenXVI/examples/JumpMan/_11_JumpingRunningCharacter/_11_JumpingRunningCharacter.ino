/*
 * In this lesson you'll make your character jump with a button press. 
 * Another state variable will tracks whether or not the character is 
 * supposed to be jumping.
 * 
 * Unlike the runState variable, the jumpState variable will have three 
 * states: 0, 1, and 2, which will refer to the default 'waiting to jump' 
 * state, the 'in the jump' state, and the 'after the jump' state. The 
 * 'after the jump' state exists to make sure the button has been 
 * released. Without it, holding down the button would make the character 
 * fly!
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

int position = 0; //holds the changing position of the cursor
bool runState = 0; //which running animation to draw (0 or 1).

/*
 * The jumping state variable is set it to 0. This variable cannot be 
 * a bool like runState because it will go beyond 0 or 1. 
 */
int jumpState = 0; //0 ='pre-jump', 1 ='mid jump', 2='post jump'

void setup() {

  lcd.begin();
  
  lcd.backlightOn();

  lcd.createChar(0, run0);//put run0 array character into 'slot' 0
  lcd.createChar(1, run1);//put run1 array character into 'slot' 1

  pinMode(6,INPUT_PULLUP); //button for character jump
}

void loop() {

  lcd.clear();

  // If the jumpState = 0 ,put the cursor at 'position' on second row
  if (jumpState == 0){
    lcd.setCursor(position,1);
  }

  /*
   * If jumpState is 1, character is in the jump. Set the cursor to draw 
   * at the 'position' on the top line. This check and the one below it 
   * use 'else if'. An 'else if' is only checked if the preceeding 'if' 
   * is not true. You can have multiple 'else ifs' in a sequence and each 
   * is checked only if all those before it are false.
   */
  else if (jumpState == 1){//if jumpState isn't 0, check if it's 1...
    lcd.setCursor(position,0); //if so, set man to 'position' on top row

    // Advance jumpState to 2 to indicate that the jump has been made
    jumpState = 2;
  }

  //If jump has been completed, return cursor to position, bottom row
  else if (jumpState == 2){
    lcd.setCursor(position,1);
  } //end of the three if-elseif-elseif options 

  lcd.write(runState);//draw the run0 or run1 animation

  delay(300); //how quickly the animation changes is based here

  runState = 1 - runState; //change which animation will be drawn next

  /*
   * If digitalRead is LOW, the button is pressed. When the button is 
   * pressed and jumpState = 0, change the jumpState to 1. Nothing will 
   * happen immediately,but the variable will hold that value and be 
   * referenced later.
   */
  if ((digitalRead(6) == LOW)&&(jumpState == 0)){
    /*
     * && is called a 'Boolean and' and it combines checks. When you see 
     * (A)&&(B), that check is true if A is true AND B is true. This 
     * checks if both the button is pressed AND the jumpState variable is
     * set to 0, and executes the following code only if both are true.
     */
    jumpState = 1;
  }
  
  /*
   * Similarly, check here if the jump has been completed and the button 
   * is released. If so, reset jumpStateto 0. This prevents the character 
   * from staying in the air if you hold the button down.
   */
  if ((digitalRead(6) == HIGH)&&(jumpState == 2)){
    jumpState = 0;
  }

  if (runState == 0){
    position = position + 1;//update position every other animation update
  }

  if (position == 16){ //reset position to 0 when you reach the screen's edge
    position = 0;
  }
}

/*
 * In this lesson, you made the character jump with a button press.
 * A variable (jumpState) controls this jump. The three values that you
 * use for this variable could be called 'states'. These three have to
 * interact so that your game can't be cheated by holding a button.
 * 
 * When testing the code from this lesson, you might notice that
 * the guy sometimes doesn't jump if you push and release the button too 
 * quickly. This is caused by the use of delay.
 * 
 * When you put in a delay, the controller sits idle while it
 * waits for the time to pass. In this lesson, the program can only catch 
 * the button press if you hold the button down long enough to get 
 * through they delay and back to a digitalRead(). 
 * Keep in mind the loop can running thousands of tmes per second without
 * any delays, so even a half-second delay slows down the loop immensely.
 * 
 * In the next lesson, you will rework the code to remove delay.
 */
  
 //Need help? http://www.letsstartcoding.com/help
 
 //(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

