#include "ALB_I2C_Scan.h"

ALB_I2C_Scan::ALB_I2C_Scan() { }
ALB_I2C_Scan::~ALB_I2C_Scan() { }

void ALB_I2C_Scan::scan()
{
	// Check if Serial port is already defined and opened
	if (!Serial)
	{
		Serial.begin(9600); // Open Serial Port
	}

	Serial.println ("I2C scanner. Scanning ...");
	byte count = 0;

	Wire.begin();
	for (byte i = 1; i < 120; i++)
	{
		Wire.beginTransmission (i);
		if (Wire.endTransmission () == 0)
		{
			Serial.print ("Found address: ");
			Serial.print (i, DEC);
			Serial.print (" (0x");
			Serial.print (i, HEX);
			Serial.println (")");
			count++;
			delay (1);
		} // end of good response
	} // end of for loop
	Serial.print ("Found ");
	Serial.print (count, DEC);
	Serial.println (" device(s).");
}