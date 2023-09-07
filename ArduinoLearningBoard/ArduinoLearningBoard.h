/*
	ArduinoLearningBoard.h - Arduino Learning Board Library
	Created by Jeff Shapiro, July 2016
	http://ArduinoLearningBoard.com
*/
#ifndef ArduinoLearningBoard_h
#define ArduinoLearningBoard_h

#include "Arduino.h"

#if defined(USE_ALB_I2C_Scan)
	#include "ALB_I2C_Scan.h"
#endif

#if defined(USE_ALB_LCD_I2C)
	#include "ALB_LCD_I2C.h"
#endif

#if defined(USE_ALB_DHT11)
	#include "ALB_DHT11.h"
#endif

#if defined(USE_ALB_DS1302)
	#include "ALB_DS1302RTC.h"
#endif

#if defined(USE_ALB_Servo)
	#include "ALB_Servo.h"
#endif

#if defined(USE_ALB_Stepper)
	#include "ALB_AccelStepper.h"
#endif

#if defined(USE_ALB_RFID)
	#include "ALB_RFID.h"
#endif

#if defined(USE_ALB_MPU6050)
	#include "ALB_I2Cdev.h"
	#include "ALB_MPU6050_6Axis_MotionApps20.h"
	#include "ALB_Wire.h"
#endif

#if defined(USE_ALB_SevenSegment)
	#include "ALB_SevSeg.h"
#endif

#endif