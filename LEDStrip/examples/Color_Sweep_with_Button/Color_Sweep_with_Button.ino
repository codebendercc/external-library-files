/*
 * Sweep through colors over the LED strip while a button is held
 */

#include <LEDStrip.h>

int numPixels = 15; //number of pixels on a strip

/*
 * Make the LED strip object LEDStrip strip
 *  = LEDStrip(numPixels, dataPin(DI), clockPin(CI));
 */
LEDStrip strip = LEDStrip(numPixels, 11, 12);

int color = 0; //Variable to hold the pixel color

int pixel = 2; //Location of the 'head' of the color bar

int Direction = -1; //A variable to hold the direction of travel

void setup() {
  pinMode(A5,INPUT_PULLUP); //Button
}

void loop() { 
  
  //Set the head pixel to the current color 
  //Color from -1 to 300, with -1 turning the LED off
  strip.setPixel(pixel, color);
    
  strip.draw(); //Draw the pixel
  
  delay(1); //Stability delay

  //If the button is pressed
  if (digitalRead(A5) == LOW){
    pixel = pixel + Direction; //Increment the head's position

    //Change direction at either end
    if ((pixel == numPixels)||(pixel == 0)){
      Direction = -1*Direction;}

    color+=1; //Increment the color
  
    if (color >= 300){ //Loop the color
      color = 0;}
  }
   
}

//Change the rate at which the color changes

// (c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

