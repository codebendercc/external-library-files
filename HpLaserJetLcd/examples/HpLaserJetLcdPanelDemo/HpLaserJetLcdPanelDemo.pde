/*
  HpLaserJetLcd libraries - Demo of Panel class
 
 Demonstrates the use an LCD control/display panel from a
 HP LaserJet printer.
 
 This sketch:

 * Sets all pixels and flashes the LEDs for approximately 1 second.
 * Prints text to the display and flahses a heart beat.
 * Sets LEDs on/off based on button presses.
 * Sets the LCD backlight on and off based on button presses.
 
 See the web link below for latest wiring information.
 
 Originally created 2 April 2011
 
 This example code is in the public domain.

 http://arduino.cc/playground/Main/HpLaserJetLcd
*/

#include <HpLaserJetLcdPanel.h> // LCD and button/LED library.

HpLaserJetLcdPanel panel(3, 5, 7, 8);

// Out custom characters.
byte openHeart[8] = {
  0b00000,
  0b01010,
  0b10101,
  0b10001,
  0b01010,
  0b00100,
  0b00000, 
  0b00000
};

byte filledHeart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

// Does a flashing sequence with the LEDs.
void StartupLedSequence()
{
  static const uint8_t LED_ON_TIME_IN_MS = 75;
 
  for (uint8_t i=0; i < 5; i++)
  {
    panel.setLedState(1, HIGH);
    delay(LED_ON_TIME_IN_MS);
    panel.setLedState(1, LOW);
  
    panel.setLedState(2, HIGH);
    delay(LED_ON_TIME_IN_MS);
    panel.setLedState(2, LOW);
  
    panel.setLedState(0, HIGH);
    delay(LED_ON_TIME_IN_MS);
    panel.setLedState(0, LOW);
  }
  
  delay(LED_ON_TIME_IN_MS);
    
  for (uint8_t i=0; i < 2; i++)
  {
    delay(LED_ON_TIME_IN_MS);
    panel.setLedState(0, HIGH);
    panel.setLedState(1, HIGH);
    panel.setLedState(2, HIGH);
    delay(LED_ON_TIME_IN_MS * (i*2+1));
    panel.setLedState(0, LOW);
    panel.setLedState(1, LOW);
    panel.setLedState(2, LOW);
  }
}

// We call this each frame to process button presses and update LED's accordingly.
void UpdateButtonsAndLeds()
{
  // Left rocking buttons pressed toggles.
  if (panel.wasButtonPressed(0))
  {
    // Toggle LED0.
    panel.setLedState(0, (panel.getLedState(0) == LOW) ? HIGH : LOW);
  }
  
  if (panel.wasButtonPressed(2))
  {
    // Toggle LED1.
    panel.setLedState(1, (panel.getLedState(1) == LOW) ? HIGH : LOW);
  }

  if (panel.wasButtonPressed(4))
  {
    // Toggle LED2.
    panel.setLedState(2, (panel.getLedState(2) == LOW) ? HIGH : LOW);
  }

  // Right rocking buttons currently down sets LED.
  if (panel.isButtonPressed(1))
  {
    // Set LED0.
    panel.setLedState(0, HIGH);
  }
  
  if (panel.isButtonPressed(3))
  {
    // Set LED1.
    panel.setLedState(1, HIGH);
  }
  
  if (panel.isButtonPressed(5))
  {
    // Set LED2.
    panel.setLedState(2, HIGH);
  }
  
  // Right rocking buttons released clears LED state.
  if (panel.wasButtonReleased(1))
  {
    // Clear LED0.
    panel.setLedState(0, LOW);
  }
  
  if (panel.wasButtonReleased(3))
  {
    // Clear LED1.
    panel.setLedState(1, LOW);
  }
  
  if (panel.wasButtonReleased(5))
  {
    // Clear LED2.
    panel.setLedState(2, LOW);
  }

  // Large white button toggles all LEDS.  
  if (panel.wasButtonPressed(6))
  {
    // Toggle all LEDs.
    panel.setLedState(0, (panel.getLedState(0) == LOW) ? HIGH : LOW);
    panel.setLedState(1, (panel.getLedState(1) == LOW) ? HIGH : LOW);
    panel.setLedState(2, (panel.getLedState(2) == LOW) ? HIGH : LOW);
  }
  
  // Turn backlight on/off.
  if (panel.wasButtonPressed(7))
  {
    panel.backLight();
  }
  
  if (panel.wasButtonPressed(8))
  {
    panel.noBackLight();
  }
}

void setup() {

  // Prepare the VFD library for use.
  panel.begin(16, 2);

  // Create open  as custom character 0. Note that this display supports 16 custom characters.
  panel.createChar(0, openHeart);
  
  // Create filled heart as custom character 1.
  panel.createChar(1, filledHeart);
    
  // Print some text to the display.
  panel.home();
  panel.print("   Panel Demo");

  // LED flashing sequence.
  StartupLedSequence();
}

void loop() {
  
  UpdateButtonsAndLeds();
  
  // Draw heart beat.
  panel.setCursor(15, 1);
  panel.write(((millis() % 1000) < 250) ? 1 : 0);
}

