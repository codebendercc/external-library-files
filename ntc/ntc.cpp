/*
  ntc.cpp - library for measuring with an ntc
  Created by Stefan Breitegger, June 2013
  Revision Feb 2014
  Released into the public domain.
*/
#include <Arduino.h>
#include "ntc.h"

void NTC::Init(byte entries, int RV, int R25, double B, double Tmin, double Tmax) {

  #if (entries > MAX_ENTRIES)
    #error "Too many entries given!"
  #endif

  #if defined(DYNAMIC_MEMORY)
    //init the 2d array
    //_listArray = new int *[entries];
    _listArray = (int **)malloc(entries*(sizeof(int *)));
    if (!_listArray) {
      return;
    }
    for( int i=0 ; i < entries ; i++ ) {
      //_listArray[i] = new int[2];
      _listArray[i] = (int *)malloc(sizeof(int));
      if (!_listArray[i]) {
        return;
      }
    }
  #endif

	this->_entries = entries;
	this->_RV = RV;
	this->_R25 = R25;
	this->_B = B;
	this->_Tmin = Tmin;
	this->_Tmax = Tmax;
	NTC::readVCC();
}

 void NTC::InitSteinhart(int RV, int R25, double A, double B, double C, double Tmin, double Tmax) {
  this->_RV = RV;
  this->_R25 = R25;
  this->_A = A;
  this->_B = B;
  this->_C = C;
 	this->_Tmin = Tmin;
	this->_Tmax = Tmax;
	NTC::readVCC();
 }

double NTC::GiveSteinhart(int adcval) {
	double R = 0; //current resistance
	double URV = 0; //current adc val scaled
	double temp = 0; //temp variable
	double T = 0;
	static unsigned long previousMillis;

  if (adcval != 1023 && adcval != 0) {  //we suppress hardware defects, eg. zero resistance NTC, zero resistance RV

    #if defined(MEASURE_AVCC)
    if ((unsigned long)(millis() - previousMillis) >= 60000) { //read every 1 minute the aref voltage
      NTC::readVCC();
      previousMillis = millis();
    }
    #endif

    URV = (_U/(long)ADC_COUNTS)*(long)adcval;  //calculate the voltage measured by the adc

    R=((long)_RV*(_U-URV))/(URV);  //resistance of the NTC

    temp = log(R);  //pre-calculate the natural logarithm

    T = (1.0/(_A + _B*temp + _C*temp*temp*temp)) - _TK;  //execute the final temperature calculation

    if (T < _Tmin || T > _Tmax) {  //temperature out of normal range
      T = _TK*-1; //return absolute zero point
    }
  }

  else {
    T = _TK*-1; //return absolute zero point
  }
  #if defined(DEBUG)
  Serial.print("_U: ");
  Serial.println(_U);
  Serial.print("URV: ");
  Serial.println(URV);
  Serial.print("R: ");
  Serial.println(R);
  #endif

	return(T);

}

void NTC::DoCalculation() {
	double u = 0; //output voltage
	double T = 0; //current temp in K
  double Tstep = 0; //at iterating temp steps
	double adcResolution = 0; //real resolution of the ADC
	unsigned int adcValue = 0; //measured value of the ADC

  //calculate the step width
	Tstep = (_Tmax - _Tmin) / (_entries-1);
  adcResolution = _U / ADC_COUNTS;


    //calculate the entries
    for (int i=0; i<_entries; i++) {
        if (i==0) {
            T = _Tmin;
        }
        else {
            T = _Tmin + (i)*Tstep;
        }

        u = (_RV*_U)/(_R25*pow(e,(_B/(T+_TK))-(_B/(_TN+_TK)))+_RV);
        adcValue = round(u / adcResolution);

        _listArray[i][0] = adcValue;
        _listArray[i][1] = round(T);
    }
}

void NTC::Print() {

	Serial.println("templist{adcvalues, temp}");
	Serial.print("int templist[");
  Serial.print(_entries);
  Serial.print("][");
  Serial.print(2);
  Serial.println("] {");

	for (int i=0; i<_entries; i++) {
		if (i<(_entries-1)) {
			Serial.print(_listArray[i][0]);
      Serial.print(", ");
      Serial.print(_listArray[i][1]);
      Serial.print(", //Element ");
      Serial.println(i);
		}
		else {
      Serial.print(_listArray[i][0]);
      Serial.print(", ");
      Serial.print(_listArray[i][1]);
      Serial.print("}; //Element ");
      Serial.println(i);
		}
  }
  Serial.println();
}

//interpolates the values of the array
double NTC::LinearInterpolate(int adcval) {
    int i = 0;
    double y10 = 0;
    double y20 = 0;
    double y11 = 0;
    double y21 = 0;
    if ((adcval >= _listArray[0][0]) && (adcval <= _listArray[_entries-1][0])) {
      for (i=0; i<_entries; i++) {
          if (_listArray[i][0] > adcval) {
              y20 = _listArray[i][0];
              y10 = _listArray[i-1][0];
              y21 = _listArray[i][1];
              y11 = _listArray[i-1][1];
              break;
          }
          if (_listArray[i][0] == adcval) {
              return(_listArray[i][1]);
              break;
          }
      }
      return(y11 + (y21 - y11) * ((double(adcval) - y10)/(y20 - y10)));
    }
    else {
      return _TK*-1;
    }
}

void NTC::readVCC() {
  int u = 0;

  // code taken from: http://code.google.com/p/tinkerit/wiki/SecretVoltmeter
  // Read 1.1V reference against AVcc
  // MUX1...3 -> read against 1.1V reference
  // REFS0 -> AV CC with external capacitor at AREF pin
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  //Start single conversion
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  u = ADCL;
  u |= ADCH<<8;
  /*
  1126.4 = 1024*1,1 = 2^10*VREF ->
    10 Bit resolution * 1,1V internal reference
  Because of the fact that the internal reference is depending on the VCC,
  it is possible to rescale the measurement from 0 to 5V:
  */
  _U = 1126.4 / (long)u; // Back-calculate AVcc in mV

}


//Destrucor, eleminating the 2d array

NTC::~NTC() {
    #if defined(DYNAMIC_MEMORY)
    for( int i = 0 ; i < _entries ; i++ ) {
        //delete [] _listArray[i] ;
        free(_listArray[i]);
    }
    //delete [] _listArray ;
    free(_listArray);
    #endif
}

