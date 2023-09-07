// test for seeed match UP project
// JY.W @ 2016-01-04

#include "Seeed_WS2812RGBLED.h"

WS2812RGBLED wsRGBLED(4);

void setup()
{
	wsRGBLED.Init();
}

void loop()
{
	int i;
	for(i=0;i<13;i++)wsRGBLED.setPixelColor(i, 50, 0, 0);
	delay(1000);
	for(i=0;i<13;i++)wsRGBLED.setPixelColor(i, 0, 50, 0);
	delay(1000);
	for(i=0;i<13;i++)wsRGBLED.setPixelColor(i, 0, 0, 50);
	delay(1000);
	for(i=0;i<13;i++)wsRGBLED.setPixelColor(i, 50, 50, 0);
	delay(1000);
	for(i=0;i<13;i++)wsRGBLED.setPixelColor(i, 0, 50, 50);
	delay(1000);
	for(i=0;i<13;i++)wsRGBLED.setPixelColor(i, 50, 0, 50);
	delay(1000);
}