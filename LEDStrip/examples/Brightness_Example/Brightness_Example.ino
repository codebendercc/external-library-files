/*
 * Fades the brightness of the entire LED strip up and down
 */

//Include the LEDStrip library to use the commands
//Don't have the library? Visit http://letsstartcoding.com/libraries
#include <LEDStrip.h>

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin (DI), clockPin (CI));
 */
LEDStrip strip = LEDStrip(15, 11, 12);

int brightness = 2; //The level of illumination

int brightnessChange = 1; //The rate of change in brightness

void setup() {
}

void loop() { 

  //Set the brightness of all pixels, color of 200 (green)
  strip.setPixel(strip.ALL,200,brightness);

  //Change direction at either limit (0% or 100%)
  if ((brightness <= 0)|(brightness >= 100)){
    brightnessChange = (-1)*brightnessChange;
  }

  //Change the brightness level
  brightness = brightness + brightnessChange;

  strip.draw(); //Draw the pixel

  delay(10); //Stability delay
}

//Change brightnessChange or the delay to make it fade faster or 
//  slower

// (c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

