// test for seeed match UP project
// JY.W @ 2016-01-20

#include <Wire.h>
#include <Seeed_OLEDDisplay.h>

OLEDDisplay oledDisplay;

void setup()
{
	Serial.begin(115200);
	Serial.println("Test for Seeed match UP project");
	Serial.println("OLED Display");
	
	oledDisplay.Init();
	
	for(char i=0; i < 12 ; i++)
	{
		oledDisplay.setGrayLevel(i);
		oledDisplay.setTextXY(i, 0, "Hello World");
	}
}

void loop()
{
  
}


