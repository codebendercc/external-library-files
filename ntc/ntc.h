/*
  ntc.h - library for measuring with an ntc
  Created by Stefan Breitegger, June 2013
  Revision Feb 2014
  Released into the public domain.

  schematic diagram:

      AVCC+
        |
        |
        -
       | |
   NTC | |
        -
        |
        |-----AVR analog input
        |
        -
       | |
   RV  | |
        -
        |
        |
       GND
  2 supported algorithms:
   -Beta method with table lookup and linear interpolation (less cpu usage, RAM usage higher - depending of the array size).
    Static/dynamic memory can be selected
   -Steinhart live calculation (measures every function call, cpu usage =~ 3 x beta method, RAM usage lower).

  Both of them support reading of the reference voltage. The Steinhart implementation reads by default every
  minute the AVCC.

*/
#include <Arduino.h>
//using namespace std;

//taken from: https://github.com/openenergymonitor/EmonLib/blob/master/EmonLib.h
// to enable 12-bit ADC resolution on Arduino Due,
// include the following line in main sketch inside setup() function:
// analogReadResolution(ADC_BITS);
// otherwise will default to 10 bits, as in regular Arduino-based boards.
#if defined(__arm__)
#define ADC_BITS 12
#else
#define ADC_BITS 10
#endif

#define ADC_COUNTS (1<<ADC_BITS)

#define DYNAMIC_MEMORY  //use dynamic memory allocation
#define MAX_ENTRIES 32  //array max. elements
#define MEASURE_AVCC  //re-measure the AVCC every minute

//#define DEBUG

class NTC {

public:
  /* Attributes */


  /* Methods */
  void Init(byte entries = 0, int RV = 0, int R25 = 0, double B = 0, double Tmin = 0, double Tmax = 0);
  void InitSteinhart(int RV = 0, int R25 = 0, double A = 0, double B = 0, double C = 0, double Tmin = 0, double Tmax = 0);
  void DoCalculation();
  double GiveSteinhart(int adcval);
  void Print();
  double LinearInterpolate(int adcval);
  void readVCC();

  /* Destructor */
  ~NTC();

private:
  /* Attributes */

  #if defined(DYNAMIC_MEMORY)
  int **_listArray;
  #else
  int _listArray[MAX_ENTRIES][2];
  #endif
	static const double _TK = 273.15; //Kelvin Temp.
	static const int _TN = 25; //Nominal temp in K
	static const double e = 2.71828182846;

  byte _entries;
  int _RV; //pull down resistor
  int _R25; //resitance ntc at 25¡ãC
	double _U; //source voltage
	double _A; //A-value NTC Steinhart
	double _B; //B-value NTC beta and Steinhart
	double _C; //C-value NTC Steinhart
	double _Tmin; //minimal temp of your interest
	double _Tmax; //maximal temp of your interest

    /* Methods */
};
