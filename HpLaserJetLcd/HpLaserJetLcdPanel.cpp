/*
  HpLaserJetLcdPanel Library - Implementation

  Originally created 2 April 2011

  This file is in the public domain.  
*/

#include "HpLaserJetLcdPanel.h"
#include "WProgram.h"

HpLaserJetLcdPanel::HpLaserJetLcdPanel(uint8_t selectPin, uint8_t clockPin, uint8_t dataWritePin, uint8_t dataReadPin) :
  HpLaserJetLcd(selectPin, clockPin, dataWritePin, dataReadPin)
{
  // Init LED's off.
  _currentLedValues = LEDS_OFF_BYTE; // Top 3 bits are for LEDs, inverted.
   
  // Init button state.
  _currentButtonState = ~0x1FF;
  _pressedState = 0;
  _releasedState = 0; 
  _lastReadMark = millis() - LAST_READ_MARK_OFFSET_FUDGE;
}

void HpLaserJetLcdPanel::begin(uint8_t cols, uint8_t lines) {
  HpLaserJetLcd::begin(cols, lines); 

  // Align our state and hardware state.
  flushLedBits();
  _currentButtonState = ~getDigitalInputs();
  _lastReadMark = millis() - LAST_READ_MARK_OFFSET_FUDGE;  
}

bool HpLaserJetLcdPanel::isButtonPressed(uint8_t buttonIndex)
{
  updateButtons();
  
  uint16_t bitMask = (1 << buttonIndex);
  return (_currentButtonState & bitMask) != 0;
}

bool HpLaserJetLcdPanel::wasButtonPressed(uint8_t buttonIndex)
{
  updateButtons();
  
  uint16_t bitMask = (1 << buttonIndex);
  uint16_t pressedStateTmp = _pressedState;
  _pressedState &= ~bitMask;
  return (pressedStateTmp & bitMask) != 0;
}

bool HpLaserJetLcdPanel::wasButtonReleased(uint8_t buttonIndex)
{
  updateButtons();
  
  uint16_t bitMask = (1 << buttonIndex);
  uint16_t releasedStateTmp = _releasedState;
  _releasedState &= ~bitMask;
  return (releasedStateTmp & bitMask) != 0;
}

uint8_t HpLaserJetLcdPanel::getLedState(int8_t ledIndex)
{
  uint8_t bitMask = 1 << ledIndex;
  return (bitMask & _currentLedValues) ? LOW : HIGH; // Led on/off is inverted.
}

void HpLaserJetLcdPanel::setLedState(uint8_t ledIndex, uint8_t level)
{
  uint8_t currentLedValuesTmp = _currentLedValues;
  uint8_t bitMask = 1 << ledIndex;
  if (level == LOW)
  {
    // Led on/off is inverted.
    currentLedValuesTmp |= bitMask;
  }
  else
  {
    // Led on/off is inverted.
    currentLedValuesTmp &= ~bitMask;
  }
  
  if (currentLedValuesTmp != _currentLedValues)
  {
    _currentLedValues = currentLedValuesTmp;
    flushLedBits();
  }    
}

void HpLaserJetLcdPanel::flushLedBits()
{
  setDigitalOutputs(_currentLedValues);
}

void HpLaserJetLcdPanel::updateButtons()
{
  if ((millis() - _lastReadMark) >= BUTTON_DEBOUNCE_DELAY_IN_MS)
  {
    // It has been long enough since the last time we read the button state.
    
    uint16_t newButtonState = ~getDigitalInputs();
    
    if (newButtonState != _currentButtonState)
    {
      // Button state has changed.
      // Figure out pressed and released states.
      uint16_t changedBits = newButtonState ^ _currentButtonState;
      _pressedState |= changedBits & newButtonState;
      _releasedState |= changedBits & _currentButtonState;
      
      // Save off new state.
      _currentButtonState = newButtonState;
      
      // Set _lastReadMark so we get the full debounce wait.
      _lastReadMark = millis();
    }
    else
    {
      // Set the _lastReadMark so we get minimum delay before next read.
      _lastReadMark = millis() - LAST_READ_MARK_OFFSET_FUDGE;
    }
  }
}

