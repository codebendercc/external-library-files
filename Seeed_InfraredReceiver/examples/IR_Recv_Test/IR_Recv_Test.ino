// test for seeed match UP project
// JY.W @ 2016-01-04

#include "Seeed_InfraredReceiver.h"

#define BIT_LEN         0
#define BIT_START_H     1
#define BIT_START_L     2
#define BIT_DATA_H      3
#define BIT_DATA_L      4
#define BIT_DATA_LEN    5
#define BIT_DATA        6

InfraredReceiver infraredReceiver(4);

void setup()
{
	Serial.begin(115200);
	Serial.println("Test for Seeed match UP project");
	Serial.println("Infrared Receiver");
	infraredReceiver.Init();
}

void loop()
{
	unsigned char *data = infraredReceiver.InfraredReceiverRead(); 

	if(data) // get IR data
	{	
		int length = data[0];
		
		Serial.println("+------------------------------------------------------+");
		Serial.print("DATA: ");
		for(int i=0; i<length; i++)
		{
			Serial.print("0x");
			Serial.print(data[i + 1], HEX);
			Serial.print("\t");
		}
		Serial.println();

		Serial.print("DATA: ");
		for(int i=0; i<length; i++)
		{
			Serial.print(data[i + 1], DEC);
			Serial.print("\t");
		}
		Serial.println();
		Serial.println("+------------------------------------------------------+\r\n\r\n");
	}
}
