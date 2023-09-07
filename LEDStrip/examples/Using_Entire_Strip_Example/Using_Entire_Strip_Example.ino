/*
 *Shows the use of the all() function to illuminate all pixels on  
 *  the LED strip at once
 */

#include <LEDStrip.h>

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin (DI), clockPin(CI));
 */
LEDStrip strip = LEDStrip(15, 11, 12);


void setup() {
}

void loop() {

  //Set all pixels to the color value 200. Colors range from 0-300
  strip.setPixel(strip.ALL,200); 

  strip.draw(); //Draw the .setPixel values to the strip
}

// (c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

