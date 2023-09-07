/*
 * Display messages that you type into your Serial monitor on the 
 * Maker Screen. 
 * 
 * Instructions: Maker Screen must remain plugged into your USB to use
 * this program. Upload the program to Maker Screen. Then press the Serial
 * Monitor button (or magnifying glass in Arduino IDE). A new window will
 * open. In the line at the top of that window, you can type messages and
 * send them by pressing "Send" or Enter.
 * 
 * If you have a garbled message or black character on the screen, check
 * the 'baud rate' in the serial monitor. It should be a drop-down menu.
 * Select 9600.
 * 
 * You may have to close/re-open the serial monitor after each upload
 * 
 * Make sure you have the Maker Screen library to do this program.
 * www.letsstartcoding.com/libraries
 */

 
//Include the MakerScreenXVI library to use the methods
#include <MakerScreenXVI.h>

//Create an object named lcd in the MakerScreenXVI library
MakerScreenXVI lcd;

void setup() {
  lcd.begin(); //Default settings for the LCD turned on with this method
  
  Serial.begin(9600);//Start the serial port. Don't change the 9600 speed
}

void loop(){
  
  if (Serial.available()) { //If new characters have been sent over serial...
    delay(100); //delay for the message to 'arrive' to the screen
    lcd.clear(); // clear the screen of existing characters

    //while there are characters 'waiting' to be read'...
    while (Serial.available() > 0) {
      lcd.write(Serial.read());  //display each character to the LCD
    }
  }
}

// Need help? http://www.letsstartcoding.com/help

//(c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

