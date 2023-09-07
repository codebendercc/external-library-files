/*
  HpLaserJetLcdPanel Library - Declaration file

  Originally created 1 April 2011
 
  Based on Arduino LiquidCrystal source.

  All changes to the LiquidCrystal source are in the public domain.  
*/

#ifndef HP_LASER_JET_LCD_H
#define HP_LASER_JET_LCD_H

#include <inttypes.h>
#include "Print.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_EXTENSION_FUNCTION_SET 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80
#define LCD_EXTENSION_SETSEGRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_REGISTER_EXTENSION 0x04
#define LCD_BLINK_ENABLE 0x02
#define LCD_LOW_POWER_MODE 0x01

class HpLaserJetLcd : public Print {
public:
  
  static const uint8_t DO_NOT_USE_PIN = 0xFF; // Can be specified for dataReadPin.
  
  HpLaserJetLcd(uint8_t selectPin, uint8_t clockPin, uint8_t dataWritePin, uint8_t dataReadPin = DO_NOT_USE_PIN);
    
  void begin(uint8_t cols = 16, uint8_t rows = 2);

  void clear();
  void home();

  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();

  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t); 
  virtual void write(uint8_t);
  void command(uint8_t);
  
  // Commands new over what is in LiquidCrystal.
  void noBackLight();
  void backLight(); // Default.
  void noExtendedCharacters(); // Default.
  void extendedCharacters();
  void extendedCommand(uint8_t);

  uint16_t getDigitalInputs();
  void setDigitalOutputs(uint8_t outputBits);
  
private:

  static const unsigned long DELAY_SCALE = 1;

  uint16_t sendLcdByte(uint8_t value, uint8_t mode, bool readBack = false);

  uint8_t _selectPin;
  uint8_t _clockPin;
  uint8_t _dataWritePin;
  uint8_t _dataReadPin;

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;

  uint8_t _digitalSwitchMask;

  uint8_t _numlines;
  uint8_t _currline;
  
  bool _useExtendedCharacters;
};

#endif
