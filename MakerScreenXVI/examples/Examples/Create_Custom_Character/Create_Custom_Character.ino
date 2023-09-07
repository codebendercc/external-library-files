//Include the MakerScreenXVI library to use the methods designed for it.
#include <MakerScreenXVI.h>

//Create an object in the LCD library named lcd. You can name it almost anything
MakerScreenXVI lcd;

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

//name the 'slot' where the smiley is stored on screen. Up to 8 custom characters
byte smileyID = 1; //naming the custom characters makes code easier to read
//value of smileyID should match the value in createChar() below.

void setup() {
  //Always type name.begin() in setup to set the screen's default settings
  lcd.begin();
  
  // create a new character in the empty 'slot 1' of the lcd screen
  lcd.createChar(1, smiley);
}

void loop() {
  //set the cursor in the upper left of the screen
  lcd.setCursor(0,0); //change these to see where the smiley is placed.
  lcd.write(smileyID); //.write() the name of the character's 'slot'

}

// Need help? http://www.letsstartcoding.com/help

// (c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

