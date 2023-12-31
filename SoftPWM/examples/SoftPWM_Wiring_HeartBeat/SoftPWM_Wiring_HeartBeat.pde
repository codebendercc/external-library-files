#include <SoftPWM.h>

#define WLED 13 // Most boards have an onboard LED attached to pin 13, change this if necessary


void setup()
{
  SoftPWMBegin();

  // Sets the PWM value to 0 for the built-in LED (WLED).
  SoftPWMSet(WLED, 0);
  
  // Sets the default fade time for WLED to
  // 100 ms fade-up and 550 ms to fade-down.
  SoftPWMSetFadeTime(WLED, 100, 550);
}

void loop() 
{
  // Turn on WLED
  SoftPWMSet(WLED, 255);
  // Wait for the fade-up and some extra delay.
  delay(250);
  // Turn off WLED
  SoftPWMSet(WLED, 0);
  // Wait for the fade-down, and some extra delay.
  delay(650);
}

