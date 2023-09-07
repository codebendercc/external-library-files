// test for seeed match UP project
// JY.W @ 2016-01-04

#include <Seeed_MiniI2CMotorDriver.h>

MiniI2CMotorDriver miniI2CMotorDriver;

void setup()
{
	Serial.begin(115200);
	Serial.println("Test for Seeed match UP project");
	Serial.println("Mini I2C Motor Driver");
}

void loop()
{
	Serial.println("Forward!");
	miniI2CMotorDriver.Driver(0, 63);
	miniI2CMotorDriver.Driver(1, 63);
	delay(3000);

	Serial.println("Stop!");
	miniI2CMotorDriver.Driver(0, 0);
	miniI2CMotorDriver.Driver(1, 0);
	delay(1000);

	Serial.println("Reverse!");
	miniI2CMotorDriver.Driver(0, -63);
	miniI2CMotorDriver.Driver(1, -63);
	delay(3000);

	Serial.println("Brake!");
	miniI2CMotorDriver.Driver(0, -1);
	miniI2CMotorDriver.Driver(1, -1);
	delay(1000);
}
