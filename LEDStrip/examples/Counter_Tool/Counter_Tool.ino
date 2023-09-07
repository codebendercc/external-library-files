/*
 * A counter using the LED strip, which displays the number of button
 * presses, as illuminated LEDs on the strip, ones place from one 
 * direction, tens place from the other. Resets after 69.
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

int count = 0; //Registered button presses

void setup() {
  
  //Button
  pinMode(A5,INPUT_PULLUP);

}

void loop() {

  int button = digitalRead(A5); //Check the button state

  if (button == LOW){ //If button is pressed
    delay(10); //Wait 10 ms to debounce
    //Wait until the button is released to proceed, to register
    //  each press only once
    while (digitalRead(A5) == LOW){
      delay(1); //Wait while button is held
    }
    count++; //After button let go, increment counter
    if (count == 70){ //When counter hits 70
      count = 1; //reset to 1
    }
    strip.clear(); //Clear previously drawn pixels
  }

  //For the 1s place, divide the count by 10 and draw the
  //  remainder (count%10)
  for (int i = 0; i < count%10; i++){
    //Color values range from 0 - 300
    strip.setPixel(i,200); //Draw out each pixel in green (200)
  }

  //For the 10s place, divide count by 10. Count is an integer,
  //  so it ignores decimal remainders (17/10 = 1.7 -> 1)
  for (int i = 0; i < count/10;i++){
    //Draw each pixel in red (100), from the end of the strip
    //numPixels-1 is the far end of the strip
    strip.setPixel(numPixels-1 - i,100);
  }

  strip.draw();//Display what was written to the strip
}

//Change the %10 and /10 in the if statements and see how high you
//  can count

// (c) 2015 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense

