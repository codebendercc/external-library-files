#ifndef ALB_I2C_Scan_h
#define ALB_I2C_Scan_h

#include "Arduino.h"
#include "ALB_Wire.h"


class ALB_I2C_Scan
{
	public:
		ALB_I2C_Scan(); // This defines a constructor for the class ALB_I2CScan
		~ALB_I2C_Scan(); // This defines a destructor for the class ALB_I2CScan
		void scan();
	private:
};

#endif