// test for seeed match UP project
// JY.W @ 2016-01-04

#include <Wire.h>
#include "Seeed_LCDRGBBacklight.h"

LCDRGBBacklight lcd;

void setup()
{
	Serial.begin(115200);
	Serial.println("Test for Seeed match UP project");
	Serial.println("LED Bar");
	
	lcd.Init();
	lcd.setRGB(50, 0, 50);
}

void loop()
{
	lcd.Print(0, "Match Up");
	lcd.Print(1, "Grove LCD RGB");
	delay(1000);
	lcd.Print(0, "Hello!");
	lcd.Print(1, "^_^......");
	delay(1000);
}
