#include "MakerScreenXVI.h"
/*
 * MakerscreenXVI library, supporting the Let's Start Coding Makerscreen hardware,
 * derived from the Arduino LiquidCrystal library.
 */


#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

// Creating the LCD object
MakerScreenXVI::MakerScreenXVI()
{
  // These are pin values, which are specifically set permanently for
  // the makerscreen hardware- they should never change for Let's Start 
  // Coding devices

  // Control pins
  _rs_pin = 12;
  _rw_pin = 255;
  _enable_pin = 11;
  
  // Data Pins, the latter 4 are '0' because we do not use them
  _data_pins[0] = 10;
  _data_pins[1] = 9;
  _data_pins[2] = 8;
  _data_pins[3] = 7;
  _data_pins[4] = 0;
  _data_pins[5] = 0;
  _data_pins[6] = 0;
  _data_pins[7] = 0;
  
  // Variable holding whether the backlight is off or on
  backlightState = 0;

  // Set the control pins to be outputs
  pinMode(_rs_pin, OUTPUT);
  pinMode(_enable_pin, OUTPUT);
  pinMode(13, OUTPUT);
  
  // A variable used below, just a setting
  _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  
}

// Initialize the screen with begin()
void MakerScreenXVI::begin() {
  // Sets the mode of the screen
  _displayfunction |= LCD_2LINE;
  _numlines = 2;

  // Adjust the offsets for character placement
  setRowOffsets(0x00, 0x40, 0x00 + 16, 0x40 + 16);  

  // A 50 ms delay to let the Screen Warm up properly
  delayMicroseconds(50000); 

  // Set control pins to set screen to accept commands
  digitalWrite(_rs_pin, LOW);
  digitalWrite(_enable_pin, LOW);
  
  //put the LCD into 4 bit mode
  if (! (_displayfunction & LCD_8BITMODE)) {
  	// Make three tries to ensure the command goes through
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms
    
    // third go!
    write4bits(0x03); 
    delayMicroseconds(150);

    // finally, set to 4-bit interface
    write4bits(0x02); 
  }

  // Set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);  

  // Turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  display();

  // Clear and leftovers from the memory
  clear();

  // Initialize to default text direction
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

  // Tell screen to await data
  command(LCD_ENTRYMODESET | _displaymode);

}

// Function to map the offsets for the screen
void MakerScreenXVI::setRowOffsets(int row0, int row1, int row2, int row3)
{
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

// Turn on the backlight
void MakerScreenXVI::backlightOn(){
	backlightState = 1;
	digitalWrite(13,1);
}

// Turn off the backlight
void MakerScreenXVI::backlightOff(){
	backlightState = 0;
	digitalWrite(13,0);
}

// Change the current backlight state to the opposite state
void MakerScreenXVI::backlightToggle(){
	backlightState = 1 - backlightState;
	digitalWrite(13,backlightState);
}

// Display data on a bargraph
void MakerScreenXVI::bargraph(int value, int lowLimit, int highLimit){
	// Use map to convert scaled value to 0-16 on screen
	int scaled = map(value,lowLimit,highLimit,0,17);
	setCursor(0,1);
	// Draw blocks for the bars
	for (int i = 0; i < scaled; i++){
		send(255, HIGH);
	}
	for (int i = scaled; i < 17; i++){
		send(' ', HIGH);
	}
}

// Wipe the screen clear of all data
void MakerScreenXVI::clear(){
  command(LCD_CLEARDISPLAY); 
  delayMicroseconds(2000); 
}


// Place the cursor at a specific line and position
void MakerScreenXVI::setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/0
  }
  
  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void MakerScreenXVI::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void MakerScreenXVI::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void MakerScreenXVI::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void MakerScreenXVI::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Scroll the display without changing the data in memory
void MakerScreenXVI::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void MakerScreenXVI::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// 'right justify' text from the cursor
void MakerScreenXVI::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

//'left justify' text from the cursor
void MakerScreenXVI::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Makes custom characters in open memory slots from 0-7
void MakerScreenXVI::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7;
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */

inline void MakerScreenXVI::command(uint8_t value) {
  send(value, LOW);
}

inline size_t MakerScreenXVI::write(uint8_t value) {
  send(value, HIGH);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void MakerScreenXVI::send(uint8_t value, uint8_t mode) {
  digitalWrite(_rs_pin, mode);
  
  write4bits(value>>4);
  write4bits(value);

}

void MakerScreenXVI::pulseEnable(void) {
  digitalWrite(_enable_pin, LOW);
  delayMicroseconds(1);    
  digitalWrite(_enable_pin, HIGH);
  delayMicroseconds(1);    // enable pulse must be >450ns
  digitalWrite(_enable_pin, LOW);
  delayMicroseconds(100);   // commands need > 37us to settle
}

void MakerScreenXVI::write4bits(uint8_t value) {
  for (int i = 0; i < 4; i++) {
    pinMode(_data_pins[i], OUTPUT);
    digitalWrite(_data_pins[i], (value >> i) & 0x01);
  }

  pulseEnable();
}

