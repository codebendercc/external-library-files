/*
 * lights_dim
 *
 * Increase LED brightness, slowing increase as brightness increases
 * and reverse.
 */

#include <TButton.h>

TButton tbutton(11);

const int ledPin = 9;

int addDelay = 0;
int increment = 1;


// The setup() method runs once, when the sketch starts

void setup()
{
  pinMode(ledPin, OUTPUT);
}

// the loop() method runs over and over again,
// as long as the Arduino has power

void loop()                     
{
  if (!tbutton.isActive())
    return;

  analogWrite(ledPin, addDelay);
  tbutton.delay(addDelay);

  addDelay += increment * random(1, 10);

  if (addDelay < 0 || addDelay > 255)
  {
    increment *= -1;
    addDelay = constrain(addDelay, 0, 255);
    tbutton.delay(1000);
  }
}
