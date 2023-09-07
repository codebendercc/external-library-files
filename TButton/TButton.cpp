/*
 * button_state
 *
 * Have button press turn on/off
 */

#include <WProgram.h>

#include "TButton.h"

TButton::TButton(int pin1)
{
  // public
  bounce_delay = 10;
  check_interval = 10;

  // private
  pin = pin1;
  active_state = true; // default to 'on'
  old_value = LOW;

  // actions
  pinMode(pin, INPUT);
}

bool TButton::isActive(void)
{
  int button_value = digitalRead(pin);

  // trigger on change from LOW to HIGH
  if (button_value == HIGH && old_value == LOW)
  {
    active_state = !active_state;
    ::delay(bounce_delay);
  }
  
  old_value = button_value;
  
  return active_state;
}

/*
 *
 * Perform delay, but check for button press to avoid button presses
 * lost during normal delays.
 */
void TButton::delay(int msecs)
{
  while (msecs > 0)
  {
    // check button during long delays
    if (!isActive())
      return;

    // call standard library delay()
    ::delay(min(msecs, check_interval));
    msecs -= check_interval;
  }
}
