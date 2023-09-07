/*
 Qtouch QT1103 1-wire serial example
 
 polls and prints the status of the capacitive pads.
 
 if you have the E1103 evaluation board, the pads are layed out in the 2x5 grid like:
 96574
 81203
 
 Created 23 November 2009
 By Dave Vondle

 */
#include <Qtouch1wire.h>

#define ONEWIREPIN 3

unsigned int value;

void setup() {
  
  OneWire.begin(ONEWIREPIN, 9600); // should be > 8000 according to datasheet
  Serial.begin(19200);
}

void loop() {
  delay(100);
  
  value = OneWire.requestRead();
  
  if(value>>8 & B00000001){
    Serial.print("0");
  }else{
    Serial.print(" ");
  }
  if(value>>8 & B00000010){
    Serial.print("1");
  }else{
    Serial.print(" ");
  }
  if(value>>8 & B00000100){
    Serial.print("2");
  }else{
    Serial.print(" ");
  }
  if(value>>8 & B00001000){
    Serial.print("3");
  }else{
    Serial.print(" ");
  }
  if(value>>8 & B00010000){
    Serial.print("4");
  }else{
    Serial.print(" ");
  }
  if(value>>8 & B00100000){
    Serial.print("5");
  }else{
    Serial.print(" ");
  }
  if(value & B00000001){
    Serial.print("6");
  }else{
    Serial.print(" ");
  }
  if(value & B00000010){
    Serial.print("7");
  }else{
    Serial.print(" ");
  }
  if(value & B00000100){
    Serial.print("8");
  }else{
    Serial.print(" ");
  }
  if(value & B00001000){
    Serial.print("9");
  }else{
    Serial.print(" ");
  }
  Serial.print("      ");
  Serial.println(value, BIN);  
  
}


