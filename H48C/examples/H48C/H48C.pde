/*
H48C - Example for H48C 3-axis sensor by parallax
Ported from kiilo killo@killo.org
by Dan McClain daniel.e.mcclain@gmail.com
Released into the public domain.
*/
#include <H48C.h>

//pin setup
int CSpin = 9;
int CLKpin = 10;
int DIOpin = 11;

//variable setup

//object setup
H48C h48c(CSpin, CLKpin, DIOpin);


void setup(){
  Serial.begin(115200);
}

void loop(){
  Serial.print("X: ");
  Serial.print(h48c.getX());
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(h48c.getY());
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(h48c.getZ());
  Serial.println();
  delay(1000);
}


