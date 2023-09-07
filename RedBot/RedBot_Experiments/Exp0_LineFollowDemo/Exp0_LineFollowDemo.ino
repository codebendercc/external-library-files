/***********************************************************************
 * Exp0_LineFollowerMario -- RedBot Demo Code
 *
 * Quick Demo Code of line following, heartbeat on the LED, and a little
 * Mario tune on the buzzer.
 * 
 * Play around with the SPEED variable to change the default speed of 
 * the RedBot
 * 
 * This code reads the three line following sensors on A3, A6, and A7
 * and prints them out to the Serial Monitor. Upload this example to your
 * RedBot and open up the Serial Monitor by clicking the magnifying glass
 * in the upper-right hand corner.
 *
 * This is a real simple example of a line following algorithm. It has
 * a lot of room for improvement, but works fairly well for a curved track.
 * It does not handle right angles reliably -- maybe you can come up with a
 * better solution?
 *
 * This sketch was written by SparkFun Electronics,with lots of help from
 * the Arduino community. This code is completely free for any use.
 *
 * 18 Feb 2015 B. Huang
 ***********************************************************************/

#include <RedBot.h>
#include "tones.h"

RedBotSensor left = RedBotSensor(A3);   // initialize a left sensor object on A3
RedBotSensor center = RedBotSensor(A6); // initialize a center sensor object on A6
RedBotSensor right = RedBotSensor(A7);  // initialize a right sensor object on A7

// constants that are used in the code. LINETHRESHOLD is the level to detect 
// if the sensor is on the line or not. If the sensor value is greater than this
// the sensor is above a DARK line.
//
// SPEED sets the nominal speed

#define SPEED 150

#define LINETHRESHOLD 800  // adjust for different lighting / background situations

const int LEDPin = 13;
const int buzzerPin = 9;
const int buttonPin = 12;

RedBotMotors motors;
int leftSpeed;   // variable used to store the leftMotor speed
int rightSpeed;  // variable used to store the rightMotor speed

// timing related variables
long timeRef;
int interval;    // variable used for blinking pattern of LED w/o delays


void setup()
{
	pinMode(LEDPin, OUTPUT);
	pinMode(buzzerPin, OUTPUT);
	pinMode(buttonPin, INPUT_PULLUP);
	
	Serial.begin(9600);
	Serial.println("Welcome to experiment 0 - Line Following");
	Serial.println("------------------------------------------");
	Serial.println("Place your robot with the center line following sensor on a ");
	Serial.println("dark line over a light colored surface, and it should start following");
	Serial.println("the line. Adjust the LINETHRESHOLD and SPEED constants in code to");
	Serial.println("adjust the behavior of your robot.");
	Serial.println();
	Serial.println("If you have a buzzer on pin 9, you can play a little tune by");
	Serial.println("pressing the D12 push button.");

	delay(2000);
	Serial.println("IR Sensor Readings: ");
	delay(500);
}

void loop()
{
	// hearbeat using modulus instead of delays
	interval = millis() / 200;
	if ((interval %  2) == 0)
		digitalWrite(LEDPin, LOW);
	else 
		digitalWrite(LEDPin, HIGH);
	if ((interval %  3) == 0)
		digitalWrite(LEDPin, LOW);  // heart-beat pattern


	// print out line following sensor values
	Serial.print(left.read());
	Serial.print("\t");  // tab character
	Serial.print(center.read());
	Serial.print("\t");  // tab character
	Serial.print(right.read());
	Serial.println();
	
	// set drive speeds for left and right motors
	// if on the line drive left and right at the same speed (left is CCW / right is CW)
	if(center.read() > LINETHRESHOLD)
	{
		leftSpeed = -SPEED; 
		rightSpeed = SPEED;
	}
	
	// if the line is under the right sensor, adjust relative speeds to turn to the right
	else if(right.read() > LINETHRESHOLD)
	{
		leftSpeed = -(SPEED + 50);
		rightSpeed = SPEED - 50;
	}

	// if the line is under the left sensor, adjust relative speeds to turn to the left
	else if(left.read() > LINETHRESHOLD)
	{
		leftSpeed = -(SPEED - 50);
		rightSpeed = SPEED + 50;
	}
	
	// if all sensors are on black or up in the air, stop the motors.
	// otherwise, run motors given the control speeds above.
	if((left.read() > LINETHRESHOLD) && (center.read() > LINETHRESHOLD) && (right.read() > LINETHRESHOLD) )
	{
		motors.stop();
	}
	else
	{
		motors.leftMotor(leftSpeed);
		motors.rightMotor(rightSpeed);
		
	}
	delay(0);  // add a delay to decrease sensitivity.

	if(digitalRead(buttonPin) == LOW)
		playMario();
}

void playMario()
{ 
	// code modified from : http://www.princetronics.com/supermariothemesong/
	
    int size = sizeof(melody) / sizeof(int);
	for (int thisNote = 0; thisNote < size; thisNote++)
	{

		// to calculate the note duration, take one second
		// divided by the note type.
		//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
		int noteDuration = 800 / tempo[thisNote];

		tone(buzzerPin, melody[thisNote], noteDuration);

		// to distinguish the notes, set a minimum time between them.
		// the note's duration + 30% seems to work well:
		int pauseBetweenNotes = noteDuration * 1.30;
		delay(pauseBetweenNotes);

		// stop the tone playing:
		tone(buzzerPin, 0, noteDuration);
	}
}

