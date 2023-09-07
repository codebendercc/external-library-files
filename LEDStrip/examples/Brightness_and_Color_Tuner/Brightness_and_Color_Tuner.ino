/*
 * LED color and brightness tuner- color changes while one button is 
 * held, and brightness while the other is held.
 */

//Include the LEDStrip library to use the commands
//Don't have the library? Visit http://letsstartcoding.com/libraries
#include <LEDStrip.h>

int numPixels = 15; //number of pixels on a strip

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin (DI), clockPin(CI));
 */
LEDStrip strip = LEDStrip(numPixels, 11, 12);

int color = 0; //Variable to hold the color of the strip

int brightness = 50; //Current brightness level

void setup() {  

  //Buttons
  pinMode(A5,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);

}

void loop() {

  if (digitalRead(A5) == LOW){
    color = (color + 1)%300; //Increase color, looping 0 - 299
    delay(20);
  }
  
  if (digitalRead(2) == LOW){
    brightness = (brightness + 1)%100; //Increase color, looping 0 - 99
    delay(20);
  }

  strip.setPixel(strip.ALL,color,brightness); //Set all pixels to current color
  
  strip.draw(); //Draw the strip

}
