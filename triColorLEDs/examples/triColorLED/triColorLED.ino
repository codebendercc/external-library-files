// triColorLED.ino
// Troy W. Weber
// August 19, 2012
// this is an example of how to use the triColorLEDs.h/.cpp library
//  with a tri-color LED. I used an LED with a common cathode, and wired
//  the anodes to the pins documented below in the code. There are
//  a couple simple value manipulation functions included in this
//  .ino file. Visit me at http://sites.google.com/site/troywweber
#include <triColorLEDs.h>

#define RED_1   3  // PWM, check LED specs for max red amperage ( mine<50mA )
#define GREEN_1 6  // PWM, check LED specs for max green amperage ( mine<30mA )
#define BLUE_1  5  // PWM, check LED specs for max blue amperage ( mine<30mA )
#define PAUSE   1000

int colors[][3] = {WHITE,RED,YELLOW,GREEN,TORQUOISE,BLUE,PURPLE};
int color_value = 1;
double bright_value = 1;

triColorLED LED1(RED_1, GREEN_1, BLUE_1, colors[color_value], 1.0);
double inc = 1.0/16;

void cycleInt( int* value, int increment, int minValue, int maxValue )
{
  (*value) += increment;
  if ( *value > maxValue )
    *value = minValue;
  if ( *value < minValue )
    *value = maxValue;
}

void cycleDouble( double* value, double increment, double minValue, double maxValue )
{
  (*value) += increment;
  if ( *value > maxValue )
    *value = minValue;
  if ( *value < minValue )
    *value = maxValue;
}

void setup()
{
  Serial.begin(9600);  // open up the serial dialogue if you want debugging messages
  return;
}

void loop()
{
  Serial.println("LED 1 ON");
  LED1.on();
  delay(PAUSE);
  Serial.println("LED 1 OFF");
  LED1.off();
  delay(PAUSE/4);
  Serial.print("cycling color_value from\t");
  Serial.print(color_value,DEC);
  Serial.print(" to ");
  cycleInt(&color_value,1,0,6);
  Serial.println(color_value,DEC);
  LED1.setColor(colors[color_value]);      // cycles through the array of pre-defined colors
  Serial.print("changing brightness to\t");
  cycleDouble(&bright_value,-0.1,0,1);     // cycles down from full brightness to 1/10th brightness
  Serial.println(bright_value,DEC);
  LED1.setBrightness(bright_value);
  return;
}
