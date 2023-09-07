/*
 * Illustrates the usage of all functions in the LEDStrip library,
 * with necessary arguments
 */

#include <LEDStrip.h>

int numPixels = 15; //number of pixels on a strip

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin, clockPin);
 */
LEDStrip strip = LEDStrip(numPixels, dataPin, clockPin);

void setup() {
}

void loop() {
  
  strip.clear(); //Turn all LEDs off

  /*
   * Set a pixel to the given color. The brightness will remain
   * at the level it was previously (the default of 50% if you
   * have not altered the level)
   */
  strip.setPixel(pixel, color);

  /*
   * Set a pixel to the given color and brightness. You can
   * set the brightness to the start-up level using 
   * strip.defaultBrightness
   */
  strip.setPixel(pixel, color, brightness);

  /*
   * Set all pixel to the given color. Their brightnesses will 
   * remain at the level they were previously (the default of 50%
   * if you have not altered their levels)
   */
  strip.setPixel(ALL, color);

    /*
   * Set all pixels to the given color and brightness. You can
   * set the brightness to the start-up level using 
   * strip.defaultBrightness
   */
  strip.setPixel(ALL, color, brightness);

  strip.draw(); //Display the pixels

}
