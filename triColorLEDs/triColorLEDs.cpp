// triColorLEDs.cpp
// Troy W. Weber
// Aug 18, 2012
// class to help with interfacing tri-color LEDs more easily

#include <Arduino.h>
#include "triColorLEDs.h"

#ifndef TRICOLORLEDS_CPP
#define TRICOLORLEDS_CPP
// constructor initialization of the class
triColorLED::triColorLED(int pin_red, int pin_green, int pin_blue, const int color[3], double brightness)
{
  pinMode(pin_red,OUTPUT);
  pinMode(pin_green,OUTPUT);
  pinMode(pin_blue,OUTPUT);
  pins[0] = pin_red;
  pins[1] = pin_green;
  pins[2] = pin_blue;
  for (int i = 0 ; i < 3 ; i++ )
    currentColor[i] = color[i];
  currentBrightness = brightness;
}
// destructor of the class
triColorLED::~triColorLED()
{
  delete pins;
  delete currentColor;
  delete &currentBrightness;
}
// setting the color and brightness of the LED; automatically turns the led on as well
void triColorLED::setLED(int color[3], double brightness)
{
  setColor(color);
  setBrightness(brightness);
  on();
  return;
}
// sets just the brightness
void triColorLED::setBrightness(double brightness)
{
  if ( currentBrightness < 0.0 )
    currentBrightness = 0.0;
  if ( currentBrightness > 1.0 )
    currentBrightness = 1.0;
  currentBrightness = brightness;
  return;
}
// sets just the color
void triColorLED::setColor(int color[3])
{
  for (int i = 0 ; i < 3 ; i++ )
    currentColor[i] = color[i];
  return;
}
// sets everything on without changing the color settings
void triColorLED::on(void)
{
  for ( int i = 0 ; i < 3 ; i++ )
    analogWrite(pins[i],currentBrightness*currentColor[i]);
  return;
}
// simply turns the output to off, while retaining color and brightness settings
void triColorLED::off(void)
{
  for (int i = 0 ; i < 3 ; i++ )
    analogWrite(pins[i],0);
  return;
}
// returns the current color code
int* triColorLED::color(void)
{
  return currentColor;
}
// returns the current brightness
double triColorLED::brightness(void)
{
  return currentBrightness;
}
#endif