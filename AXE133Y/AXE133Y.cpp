
// Please see AXE133Y.h for licence info

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "AXE133Y.h"
#include "inttypes.h"

#define bitWidth 417 //microseconds; Equates to 2400 baud
#define firmwareRevision "001"

AXE133Y::AXE133Y(uint8_t pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void AXE133Y::writeByte(uint8_t foo)
{
  digitalWrite(_pin, HIGH);  //start bit starts here
  delayMicroseconds(bitWidth);
  //shift a byte out to the hardware pin
  for (byte mask = 0x01; mask; mask <<= 1) {
    if (foo & mask) digitalWrite(_pin, LOW);
    else digitalWrite(_pin, HIGH); 
    delayMicroseconds(bitWidth);
  } 
  digitalWrite(_pin, LOW);  //end bit starts here
  delayMicroseconds(bitWidth);
  delayMicroseconds(300); //this is the inter-byte delay
}

 void AXE133Y::splash(String message)
{
  String splash = String("  AXE133Y V") + String(firmwareRevision);
  clearScreen();
  print(splash);
  cursorHome(2);
  print(message);
}

void AXE133Y::displayShow(bool show)
{
  writeByte(254);
  if(show) writeByte(12);
  else writeByte(8);
}

void AXE133Y::cursorShow(bool show)
{
  writeByte(254);
  if(show) writeByte(14);
  else writeByte(12);
}


void AXE133Y::cursorBlink(bool blink)
{
  writeByte(254);
  if(blink)writeByte(15);
  else writeByte(14);
}

void AXE133Y::clearScreen()
{
  writeByte(254);
  writeByte(1);  //clear
  delay(2); //wait for display to finish clearing
  writeByte(254);
  writeByte(128); //home line 1
  delay(2); //wait for command to finish
  cursorShow(0); //turn cursor off
}

void AXE133Y::cursorHome(uint8_t line)
{
 writeByte(254);
 if(line == 1) writeByte(128);
 else writeByte(192);
 delay(2); //wait for command to finish
}

void AXE133Y::cursorLeft(uint8_t moves)
{
 for (uint8_t i = 0; i<moves ; i++) 
  {
  writeByte(254);
  writeByte(16);
  }
}

void AXE133Y::cursorRight(uint8_t moves)
{
 for (uint8_t i = 0; i<moves ; i++) 
  {
  writeByte(254);
  writeByte(20);
  }
}

void AXE133Y::cursorPosition(uint8_t line, uint8_t position)
{
  writeByte(254);
  if(line ==1) position += 128;
  else position += 192;
  writeByte(position);
}

void AXE133Y::printMessage(uint8_t messageNumber)
{
  writeByte(253);
  delayMicroseconds(140); //don't take this 140us delay out
  writeByte(messageNumber);
}

void AXE133Y::print(String message)
{
  for(uint8_t i = 0; i< message.length(); i++) writeByte(message[i]);
}

void AXE133Y::outputWrite(uint8_t newState)
{
  writeByte(255);
  delayMicroseconds(250); //don't take this 250us delay out
  writeByte(newState);
}

void AXE133Y::backspace(uint8_t moves)
{
for (int i = 0; i< moves; i++)
 {
  writeByte(254);
  writeByte(16); //cursor left
  writeByte(' ');
  writeByte(254);
  writeByte(16); //cursor left
 }
}  

void AXE133Y::printFloat(float foo, uint8_t minimumLength, uint8_t decimalPlaces)
{
  char stringBuffer[16];
  String floatString;
  dtostrf(foo,minimumLength,decimalPlaces,stringBuffer);
  floatString = String(stringBuffer);
  print(floatString);
}

