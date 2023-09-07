/*
 * Display a bar graph on Maker Screen showing the value read from
 * a turn knob. This example can be used for any type of sensor or input.
 * 
 * Make sure you have the Maker Screen library to do this program.
 * www.letsstartcoding.com/libraries
 */
 
//Include the MakerScreenXVI library to use the commands
#include <MakerScreenXVI.h>

//Create an object named lcd in the MakerScreenXVI library
MakerScreenXVI lcd;

int columns = 0; //The number of bars to fill on the display

void setup() {
  lcd.begin();

  lcd.clear(); //clear all existing info from the screen
  
  pinMode(A3,INPUT); //Middle pin of turn knob. Outsides should go to + and -
  }

void loop() {
  int knobPosition = analogRead(A3); //set the knob's position as a variable
  
  //Print the measured analog value on line 0
  lcd.setCursor(0,0); //Set cursor to upper left
  lcd.print(knobPosition);//Value of the variable
  lcd.print("    "); //Erase any remaining characters by 'printing' blanks

  lcd.bargraph(knobPosition,0,1023);//show knobPosition on a scale of 0-1023
  
  delay(100); //Delay to reduce flicker
}

//Replace the 255 in lcd.write with a different number or variable to 
//  change the appearance of the blocks.

// Need help? http://www.letsstartcoding.com/help

//(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

