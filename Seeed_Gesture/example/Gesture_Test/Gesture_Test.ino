// test for seeed match UP project
// JY.W @ 2016-01-04

#include <Wire.h>
#include "Seeed_Gesture.h"

#define GES_NONE_EVENT				0
#define GES_RIGHT_EVENT				1
#define GES_LEFT_EVENT				2
#define GES_UP_EVENT				3
#define GES_DOWN_EVENT				4
#define GES_FORWARD_EVENT			5
#define GES_BACKWARD_EVENT			6
#define GES_CLOCKWISE_EVENT			7
#define GES_COUNT_CLOCKWISE_EVENT	8
#define GES_WAVE_EVENT				9

Gesture gesture;

void setup()
{
	Serial.begin(115200);
	Serial.println("Test for Seeed match UP project");
	Serial.println("Gesture Sensor");
	gesture.Init();
}

void loop()
{
	int data;
	
	data = gesture.GestureRead();
	if(data)
	{
		switch(data)
		{
			case GES_RIGHT_EVENT:
				Serial.println("Get gesture right event.");
			break;
			
			case GES_LEFT_EVENT:
				Serial.println("Get gesture left event.");
			break;
			
			case GES_UP_EVENT:
				Serial.println("Get gesture up event.");
			break;
			
			case GES_DOWN_EVENT:
				Serial.println("Get gesture down event.");
			break;
			
			case GES_FORWARD_EVENT:
				Serial.println("Get gesture foeward event.");
			break;
			
			case GES_BACKWARD_EVENT:
				Serial.println("Get gesture backward event.");
			break;
			
			case GES_CLOCKWISE_EVENT:
				Serial.println("Get gesture clockwise event.");
			break;
			
			case GES_COUNT_CLOCKWISE_EVENT:
				Serial.println("Get gesture count clockwise event.");
			break;
			
			case GES_WAVE_EVENT:
				Serial.println("Get gesture wave event.");
			break;
			
			default:
			break;
		}
	}
	delay(100);
}

