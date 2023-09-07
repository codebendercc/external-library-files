#include <SimpleSoftwareServo.h>

/*
 * Sweeps a servo back and forth.
 */

SimpleSoftwareServo servo1;

int servo1_pin = 2;

void setup() {
  servo1.attach(servo1_pin);
}

void loop() {
  int pos;
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    servo1.write(pos);              // tell servo to go to position in variable 'pos' 
    SimpleSoftwareServo::refresh(); // delays ~20ms
  } 
  for(pos = 180; pos>=1; pos -= 1)     // goes from 180 degrees to 0 degrees 
  {                                
    servo1.write(pos);              // tell servo to go to position in variable 'pos' 
    SimpleSoftwareServo::refresh(); // delays ~20ms
  } 
}

