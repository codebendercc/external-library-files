/* 
H48C.h - Arduino Library for getting X,Y,Z data from Parallax H48C module
*/
#ifndef H48C_h
#define H48C_h

#include "WProgram.h"

class H48C
{
	public:
		H48C(int CSpin, int CLKpin, int DIOpin);
		int getX();
		int getY();
		int getZ();
		void startBIT();
		void sampleIT();
		int  getVALUE(byte command);
		void shiftOUTNIBBLE(byte dataoutnibble);
		byte shiftINNIBBLE();
		void endBIT();
		int _DIOpin;
		int _CLKpin;
		int _CSpin;
		int _Result;
		int _X;
		int _Y;
		int _Z;
	private:
};

#endif