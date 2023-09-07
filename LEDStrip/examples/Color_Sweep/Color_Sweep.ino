/*
 * Sweep a color and brightness changing pixel back and forth
 * across the LED strip, without clearing previously drawn pixels
 */

#include <LEDStrip.h>

int numPixels = 15; //number of pixels on a strip

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin, clockPin);
 */
LEDStrip strip = LEDStrip(numPixels, 11, 12);

/*
 * A counter to watch for when to adjust the rates
 * of change of individual colors
 */
int color = 0;

/*
 * An incrementor to hold the position of the 'head'
 * of the colorbar
 */
int pixel = 2;

int Direction = -1; //A variable to hold the direction of travel

int brightness = 100; //A variable to hold the brightenss level

void setup() {
}

void loop() { 

  strip.setPixel(pixel, color, brightness);
    
  strip.draw();
  
  delay(1); //Stability delay

  pixel = pixel + Direction; //Increment the head's position

  //Change direction at either end
  if ((pixel == numPixels-1)||(pixel == 0)){
    Direction = -1*Direction;}

  color+=1; //Increment the color

  brightness+=1; //Increment brightness

  if (brightness >= 100){ //Loop the brightness
    brightness = 0;
  }
  
  if (color >= 300){ //Loop the color
    color = 0;
  }

}

//Change the rate at which color or brightness changes.

// (c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

