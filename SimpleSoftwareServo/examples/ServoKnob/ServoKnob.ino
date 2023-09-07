#include <SimpleSoftwareServo.h>

/*
 * Move a servo based on input from a knob.  Also set brightness of an LED proportional to servo angle.
 */

SimpleSoftwareServo servo1;

int servo1_pin = 2;
int led_pin = 0;
int pot_pin = 2;  // analog input pin (different numbering scheme from digital pins)

void setup() {
  pinMode(led_pin, OUTPUT);
  servo1.attach(servo1_pin);
}

void loop() {
  int val = analogRead(pot_pin);
  servo1.write(map(val,0,1023,0,180));
  analogWrite(led_pin,map(val,0,1023,0,255));
  delay(15); // optional, since refresh() will delay as needed for the next pulse
  SimpleSoftwareServo::refresh();
}

