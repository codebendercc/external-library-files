// test for seeed match UP project
// JY.W @ 2016-01-04

#include <Wire.h>
#include <Seeed_I2CTouchSensor.h>

I2CTouch i2cTouch;

void setup() 
{    
	Serial.begin(115200);
	Serial.println("Test for Seeed match UP project");
	Serial.println("I2C Touch Sensor");
	
	i2cTouch.Init();  
}

void loop()
{
	for(int i=0;i<12;i++)
	{
		if(i2cTouch.I2CTouchRead(i))
		{
			Serial.print("pin ");
			Serial.print(i);
			Serial.println(" was  touched");
		}
	}
	delay(200);
}  
