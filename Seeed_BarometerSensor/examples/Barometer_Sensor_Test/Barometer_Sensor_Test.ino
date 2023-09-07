// test for seeed match UP project
// JY.W @ 2016-01-20

#include "Seeed_BarometerSensor.h"
#include <Wire.h>

Barometer barometer;


void setup()
{
	Serial.begin(115200);
	Serial.println("Test for Seeed match UP project");
	Serial.println("Barometer Sensor");
	barometer.Init();
}

void loop()
{
	float temperature = barometer.BarometerRead(0);
	float pressure = barometer.BarometerRead(1);
	float atm = barometer.BarometerRead(2);
	float altitude = barometer.BarometerRead(3);

	Serial.print("Temperature: ");
	Serial.print(temperature, 2);
	Serial.println(" deg C");

	Serial.print("Pressure: ");
	Serial.print(pressure, 0);
	Serial.println(" Pa");

	Serial.print("Ralated Atmosphere: ");
	Serial.println(atm, 4);

	Serial.print("Altitude: ");
	Serial.print(altitude, 2);
	Serial.println(" m");

	Serial.println();

	delay(1000); //wait a second and get values again.
}



