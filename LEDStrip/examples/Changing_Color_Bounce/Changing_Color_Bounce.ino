/*
 * Bouncing a pixel along the LED strip
 */

#include <LEDStrip.h>

int numPixels = 15; //number of pixels on a strip

int color = 200; //Variable to hold the pixel color

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin, clockPin);
 */
LEDStrip strip = LEDStrip(numPixels, 11, 12);

int pixel = 0; //Position of the pixel on the strip

int Direction = 1; //Direction of travel of the pixel

void setup() {

}

void loop() {

  strip.setPixel(pixel,-1); //Clear the old pixel
  
  pixel = pixel + Direction; //Move the pixel

  strip.setPixel(pixel,color); //Write the new pixel

  strip.draw(); //Draw what we wrote to the strip


  //If at either end of the strip
  if ((pixel == 0)||(pixel == numPixels)){
    Direction = -1*Direction; //Direction will be 1 or -1
    //Change color by 77, %300 to keep within 0-299 range
    color = (color + 77)%300;
  }

  delay(100); //Timing delay controls pixel speed
}

//Change the if statement to make the pixel bounce between different
//  limits

// (c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense


