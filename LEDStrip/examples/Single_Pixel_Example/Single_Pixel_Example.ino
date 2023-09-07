/*
 * Using a single LED on the LED strip
 */

#include <LEDStrip.h>

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin (DI), clockPin(CI));
 */
LEDStrip strip = LEDStrip(numPixels, 11, 12);

void setup() {
}

void loop() {
  
  //Set pixel 7 to a color between 0 and 300. Brightess is %: 0 to 100
  //The brightness number is optional. You can leave it out
  strip.setPixel(7, 200, 75);

  strip.draw(); //Draw the pixel
}

//Change the pixel, color, and brightness values
//Use more than line of code to set other pixels as colors.

// (c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

