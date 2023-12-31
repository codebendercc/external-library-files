/* DHT library */

#ifndef ALB_DHT11_H
#define ALB_DHT11_H

#include "Arduino.h"

class ALB_DHT11 {
  public:
   ALB_DHT11(uint8_t pin);
   void begin(void);
   float readTemperatureC();
   float readTemperatureF();
   float convertCtoF(float);
   float convertFtoC(float);
   float computeHeatIndexC(float temperature, float percentHumidity);
   float computeHeatIndexF(float temperature, float percentHumidity);
   float readHumidity(bool force=false);
   boolean read(bool force=false);

 private:
  uint8_t data[5];
  uint8_t _pin;
  uint8_t _bit, _port;
  uint32_t _lastreadtime, _maxcycles;
  bool _lastresult;
  uint32_t expectPulse(bool level);
};

class InterruptLock {
  public:
   InterruptLock() {
    noInterrupts();
   }
   ~InterruptLock() {
    interrupts();
   }

};

#endif
