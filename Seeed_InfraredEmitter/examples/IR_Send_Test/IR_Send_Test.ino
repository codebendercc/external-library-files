// test for seeed match UP project
// JY.W @ 2016-01-04

#include "Seeed_InfraredEmitter.h"

InfraredEmitter infraredEmitter(3);
unsigned char dtaSend[20];


void dtaInit()
{	
	dtaSend[0]     = 128;			// data that will sent
	dtaSend[1]     = 127;
	dtaSend[2]     = 192;
	dtaSend[3]     = 63;
	dtaSend[4]     = 192;
	dtaSend[5]     = 63;
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Test for Seeed match UP project");
	Serial.println("Infrared Emitter");
	infraredEmitter.Init();
	dtaInit();
}

void loop()
{
	infraredEmitter.InfraredEmitterWrite(dtaSend);
	delay(2000);
}
