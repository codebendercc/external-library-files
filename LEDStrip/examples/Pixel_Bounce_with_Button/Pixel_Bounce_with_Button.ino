/*
 * Bouncing a pixel along the strip when the button is pressed 
 */

#include <LEDStrip.h>

int numPixels = 15; //number of pixels on a strip

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin, clockPin);
 */
LEDStrip strip = LEDStrip(numPixels, 11, 12);

int pixel = 0; //Position of the pixel on the strip

int Direction = 1; //Direction of travel of the pixel

void setup() {
  pinMode(A5,INPUT_PULLUP);
}

void loop() {
  
  //If the button is pressed
  if (digitalRead(A5) == LOW){
    strip.setPixel(pixel,-1); //Erase the previous pixel
    pixel = pixel + Direction; //Move the pixel
  }
  
  strip.setPixel(pixel,200); //Write the pixel

  strip.draw(); //Draw what we wrote to the strip
  
  //If at either end of the strip
  if ((pixel == 0)||(pixel == numPixels-1)){
    Direction = -1*Direction; //Direction will be 1 or -1
  }

  delay(100); //Timing delay controls pixel speed
}

//Change the if statement to make the pixel bounce between different
//  limits

// (c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense


