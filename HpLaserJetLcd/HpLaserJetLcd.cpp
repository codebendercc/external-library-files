/*
  HpLaserJetLcdPanel Library - Implementation

  Originally created 1 April 2011
 
  Based on Arduino LiquidCrystal source.

  All changes to the LiquidCrystal source are in the public domain.  
*/
#include "HpLaserJetLcd.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "WProgram.h"

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// HpLaserJetLcd constructor is called).


HpLaserJetLcd::HpLaserJetLcd(uint8_t selectPin, uint8_t clockPin, uint8_t dataWritePin, uint8_t dataReadPin)
{
  _selectPin = selectPin;
  _clockPin = clockPin;
  _dataWritePin = dataWritePin;
  _dataReadPin = dataReadPin;

  digitalWrite(selectPin, LOW);
  pinMode(selectPin, OUTPUT);
  
  digitalWrite(clockPin, LOW);
  pinMode(clockPin, OUTPUT);
  
  digitalWrite(dataWritePin, LOW);
  pinMode(dataWritePin, OUTPUT);
  
  if (dataReadPin != DO_NOT_USE_PIN)
  {
    pinMode(dataReadPin, INPUT);
    digitalWrite(dataReadPin, LOW);
  }
    
  _displayfunction = LCD_4BITMODE | LCD_1LINE;
  _digitalSwitchMask = 0b000001; // Default to backlight on.

  _useExtendedCharacters = false;
  
  begin(16, 2);  
}

void HpLaserJetLcd::begin(uint8_t cols, uint8_t lines) {
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;
  _currline = 0;

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
  delayMicroseconds(50000); 
  
  //put the LCD into 4 bit mode

  // this is according to the hitachi HD44780 datasheet
  // page 45 figure 23

  // Send function set command sequence
  command(LCD_FUNCTIONSET | _displayfunction);
  delayMicroseconds(4500 * DELAY_SCALE);  // wait more than 4.1ms

  // second try
  command(LCD_FUNCTIONSET | _displayfunction);
  delayMicroseconds(150 * DELAY_SCALE);

  // third go
  command(LCD_FUNCTIONSET | _displayfunction);

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);  

  // Need to pop in and out of extended function for contrast to be correct.
  // Not sure why.
  command(LCD_FUNCTIONSET | _displayfunction | LCD_REGISTER_EXTENSION);
  command(LCD_FUNCTIONSET | _displayfunction);  

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);  
  
  // Clear extension function set.
  extendedCommand(LCD_EXTENSION_FUNCTION_SET | 0);
  
  // Zero any extended addresses.
  extendedCommand(LCD_EXTENSION_SETSEGRAMADDR | 0);
}

/********** high level commands, for the user! */
void HpLaserJetLcd::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000 * DELAY_SCALE);  // this command takes a long time!
}

void HpLaserJetLcd::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000 * DELAY_SCALE);  // this command takes a long time!
}

void HpLaserJetLcd::setCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row > _numlines ) {
    row = _numlines-1;    // we count rows starting w/0
  }
  
  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void HpLaserJetLcd::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void HpLaserJetLcd::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void HpLaserJetLcd::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void HpLaserJetLcd::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void HpLaserJetLcd::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void HpLaserJetLcd::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void HpLaserJetLcd::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void HpLaserJetLcd::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void HpLaserJetLcd::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void HpLaserJetLcd::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void HpLaserJetLcd::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void HpLaserJetLcd::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void HpLaserJetLcd::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

void HpLaserJetLcd::noBackLight()
{
  _digitalSwitchMask &= ~1;

  // Send a NOP command which will also set the bits.
  command(0);
}

void HpLaserJetLcd::backLight()
{
  _digitalSwitchMask |= 1;

  // Send a NOP command which will also set the bits.
  command(0);
}

void HpLaserJetLcd::noExtendedCharacters()
{
  _useExtendedCharacters = false;  
}

void HpLaserJetLcd::extendedCharacters()
{
  _useExtendedCharacters = true;  
}

/*********** mid level commands, for sending data/cmds */

inline void HpLaserJetLcd::command(uint8_t value) {
  sendLcdByte(value, LOW);
}

inline void HpLaserJetLcd::write(uint8_t value) {
  
  if (!_useExtendedCharacters)
  {
    if ((value>='a') && (value<='z'))
    {
      value -= 'a' - 'A';
    }
  }
  
  sendLcdByte(value, HIGH);
}

void HpLaserJetLcd::extendedCommand(uint8_t value)
{
  command(LCD_FUNCTIONSET | _displayfunction | LCD_REGISTER_EXTENSION);
  command(value);
  command(LCD_FUNCTIONSET | _displayfunction);
}

uint16_t HpLaserJetLcd::getDigitalInputs()
{
  // We send in dummy NOP command twice, once so we can toggle the latch
  // and latch the inputs, and a second time to read the input values
  // back out.
  
  sendLcdByte(0, LOW);
  uint16_t inputValues = sendLcdByte(0, LOW, true); 

  return inputValues >> (16-9); // Only want 9 of the 16 bits received.
}

void HpLaserJetLcd::setDigitalOutputs(uint8_t outputBits)
{
  _digitalSwitchMask &= ~0b00111000;
  _digitalSwitchMask |= (outputBits & 0b111)<<3;

  // Send a NOP command which will also set the bits.
  command(0);
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
uint16_t HpLaserJetLcd::sendLcdByte(uint8_t value, uint8_t mode, bool readBack)
{
  // Data is clocked MSB to LSB.

  uint16_t dataRead = 0;

  uint16_t wordValue = (mode) ? 0b00000000 : 0b10000000; // rsBit.
  wordValue |= (uint16_t)_digitalSwitchMask;
  wordValue |= ((uint16_t)(~value)<<8);

  for (uint16_t mask = (1<<15); mask; mask >>= 1)
  { 
    // We don't always read back since the performace cost of doing
    // that seems to mess up certain LCD operations.
    if (readBack && (_dataReadPin!=DO_NOT_USE_PIN))
    {
      dataRead <<= 1;
      if (digitalRead(_dataReadPin)!=LOW)
      {
        dataRead |= 1;
      }
    }
      
    // Set output bit.
    digitalWrite(_dataWritePin, (wordValue & mask) ? HIGH : LOW);    
    
    // Clock data.
    digitalWrite(_clockPin, HIGH);
    digitalWrite(_clockPin, LOW);
  }
  
  delayMicroseconds(1);    
  digitalWrite(_selectPin, HIGH);
  delayMicroseconds(1);    
  digitalWrite(_selectPin, LOW);
  delayMicroseconds(50 * DELAY_SCALE);   // commands need > 37us to settle

  return dataRead;
}

