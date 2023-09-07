/*
H48C.cpp - Library for H48C 3-axis sensor by parallax
Ported from kiilo killo@killo.org
by Dan McClain daniel.e.mcclain@gmail.com
Released into the public domain.
*/

#include "Wprogram.h"
#include "H48C.h"

H48C::H48C(int CSpin, int CLKpin, int DIOpin){
	pinMode(DIOpin, OUTPUT);
	pinMode(CLKpin, OUTPUT);
	pinMode(CSpin, OUTPUT);
	_DIOpin = DIOpin;
	_CSpin = CSpin;
	_CLKpin = CLKpin;
	digitalWrite(_CSpin, LOW);
	digitalWrite(_CLKpin, LOW);
	delayMicroseconds(1);
	digitalWrite(_CSpin, HIGH);
	digitalWrite(_CLKpin, HIGH);
}

void H48C::startBIT()
{
	pinMode(_DIOpin, OUTPUT);
	digitalWrite(_CSpin, LOW);
	digitalWrite(_CLKpin, LOW);
	delayMicroseconds(1);
	digitalWrite(_DIOpin, HIGH);
	digitalWrite(_CLKpin, HIGH);
	delayMicroseconds(1);	
}

void H48C::endBIT()
{
	digitalWrite(_CSpin, HIGH);
	digitalWrite(_CLKpin, HIGH);
}

void H48C::sampleIT()
{
	digitalWrite(_CLKpin, LOW);
	delayMicroseconds(1);
	digitalWrite(_CLKpin, HIGH);
	delayMicroseconds(1);
	pinMode(_DIOpin, INPUT);
	digitalWrite(_CLKpin, LOW);
	delayMicroseconds(1);
	digitalWrite(_CLKpin, HIGH);		
}

byte H48C::shiftINNIBBLE()
{
	byte resultnibble;
	resultnibble = 0;
	
	for(int i = 3; i >= 0; i--)
	{
		digitalWrite(_CLKpin, LOW);
		delayMicroseconds(1);
		if(digitalRead(_DIOpin) == HIGH)
		{
			resultnibble += 1 << i;
		}
		else
		{
			resultnibble += 0 << i;
		}
		digitalWrite(_CLKpin, HIGH);
	}
	return resultnibble;	
}

void H48C::shiftOUTNIBBLE(byte dataout)
{
	for(int i = 3; i >= 0; i--)
	{
		digitalWrite(_CLKpin, LOW);
		if ((dataout & (1 << i)) == (1 << i))
		{
			digitalWrite(_DIOpin, HIGH);
		}
		else
		{
			digitalWrite(_DIOpin, LOW);
		}
		digitalWrite(_CLKpin, HIGH);
		delayMicroseconds(1);
	}
}

int H48C::getVALUE(byte Axis)
{
	_Result = 0;
	startBIT();
	shiftOUTNIBBLE(Axis);
	sampleIT();
	_Result = 2048 - ((shiftINNIBBLE() << 8) + (shiftINNIBBLE() << 4) + shiftINNIBBLE());
	endBIT();
	return _Result;
}

int H48C::getX()
{
	_X = getVALUE(B1000);
	return _X;
}

int H48C::getY()
{
	_Y = getVALUE(B1001);
	return _Y;
}

int H48C::getZ()
{
	_Z = getVALUE(B1010);
	return _Z;
}
