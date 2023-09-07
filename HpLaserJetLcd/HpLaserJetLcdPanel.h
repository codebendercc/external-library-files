/*
  HpLaserJetLcdPanel Library - Declaration file
  
  Originally created 2 April 2011

  This file is in the public domain.  
*/

#ifndef HpLaserJetLcdPanel_h
#define HpLaserJetLcdPanel_h

#include "HpLaserJetLcd.h"

// This class provides an interface to the buttons on a HP LaserJet control/LEC/LCD panel.
// Basically, it is the HpLaserJetLcd class with helper functionality for the buttons and LEDs.
class HpLaserJetLcdPanel : public HpLaserJetLcd {
public:

  HpLaserJetLcdPanel(uint8_t selectPin, uint8_t clockPin, uint8_t dataWritePin, uint8_t dataReadPin = DO_NOT_USE_PIN);

  void begin(uint8_t cols = 16, uint8_t rows = 2);
  
  // All button reads are debounced.
  bool isButtonPressed(uint8_t buttonIndex); 
  bool wasButtonPressed(uint8_t buttonIndex); // Latched, clears state for button when read.
  bool wasButtonReleased(uint8_t buttonIndex); // Latched, clears state for button when read.
  
  uint8_t getLedState(int8_t ledIndex);
  void setLedState(uint8_t ledIndex, uint8_t level);

private:

  static const uint8_t LEDS_OFF_BYTE = 0xFF; // LEDs are active low.

  static const uint8_t MIN_BUTTON_READ_DELAY_IN_MS = 1; // Limit rate of polling to minimize CPU time, etc.
  static const uint8_t BUTTON_DEBOUNCE_DELAY_IN_MS = 20;
  static const uint8_t LAST_READ_MARK_OFFSET_FUDGE = BUTTON_DEBOUNCE_DELAY_IN_MS - MIN_BUTTON_READ_DELAY_IN_MS;

  // LED state.
  uint8_t _currentLedValues;

  // Button reading state.
  unsigned long _lastReadMark;
  uint16_t _currentButtonState;
  uint16_t _pressedState;
  uint16_t _releasedState;
  
  void flushLedBits();
  void updateButtons();
};
  
#endif // HpLaserJetLcdPanel_h

