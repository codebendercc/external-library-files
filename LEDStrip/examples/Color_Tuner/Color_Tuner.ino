/*
 * LED color tuner- color changes while the button is held
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

void setup() {  

  //Button
  pinMode(A5,INPUT_PULLUP);

}

void loop() {

  //If the button is pressed
  if (digitalRead(A5) == LOW){
    //Increase color, looping 0-299, color value is the remainder of
    // (color+1) divided by 300
    color = (color + 1)%300;
  }

  strip.setPixel(strip.ALL,color); //Set all pixels to current color
  
  strip.draw(); //Draw the strip

  delay(10); //Visibility delay
} 

//Use serial.begin and serial.println to send the color variable 
//  value to the current color to the serial monitor

// (c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

