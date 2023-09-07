/*
 * MTBDEMO.ino
 * A simple demo for Grove - Mini Track ball v1.0
 * Description:  Get the track data on removable, include up, down, left, right, click.
 *
 * Copyright (c) 2015 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Ruibin Wu
 * Modified Time: Nov 2015
 */

#include "Wire.h"
#include "Seeed_MiniTrackBall.h"


MiniTrackBall miniTrackBall;

void setup() 
{ 
	Serial.begin(115200);
	Serial.println("Test for Seeed match UP project");
	Serial.println("Mini Track Ball Sensor");
	Wire.begin();
}

void loop() 
{
	if(miniTrackBall.Available())
	{		
		int motion_up_data = miniTrackBall.miniTrackBallRead(1);
		int motion_down_data = miniTrackBall.miniTrackBallRead(2);
		int motion_left_data = miniTrackBall.miniTrackBallRead(3);
		int motion_right_data = miniTrackBall.miniTrackBallRead(4);
		int motion_confirm_data = miniTrackBall.miniTrackBallRead(5);

		Serial.print("Motion data ");
		Serial.print("up ");
		Serial.print(motion_up_data, DEC);
		Serial.print(", ");

		Serial.print("down ");
		Serial.print(motion_down_data, DEC);
		Serial.print(", ");

		Serial.print("left ");
		Serial.print(motion_left_data, DEC);
		Serial.print(", ");

		Serial.print("right ");
		Serial.print(motion_right_data, DEC);
		Serial.print(", ");

		Serial.print("confirm ");
		Serial.print(motion_confirm_data, DEC);
		Serial.println(".");
	}
	delay(100);
}

