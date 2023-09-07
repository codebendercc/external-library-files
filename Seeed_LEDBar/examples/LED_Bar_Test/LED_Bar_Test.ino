// test for seeed match UP project
// JY.W @ 2016-01-04

#include "Seeed_LEDBar.h"

LEDBar ledBar(9, 8);  // Clock pin, Data pin

void setup()
{
  	Serial.begin(115200);
	Serial.println("Test for Seeed match UP project");
	Serial.println("LED Bar");
	
	ledBar.Init();
}

void loop()
{
	for(int i = 0; i <= 10; i++)
	{
		ledBar.SetLevel(i);
		delay(100);
	}
}
