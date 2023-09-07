// test for seeed match UP project
// JY.W @ 2016-01-04

#include "Seeed_AirQuality.h"

AirQuality airQuanlity(A0);

void setup()
{
	Serial.begin(115200);
	Serial.println("Test for Seeed match UP project");
	Serial.println("Air Quality Sensor");
}

void loop()
{	
	int value = airQuanlity.AirQualityRead();
	Serial.print("AirQuality Read is ");
	Serial.println(value);
	delay(1000);
}
