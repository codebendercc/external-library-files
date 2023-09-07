#ifndef SimpleSoftwareServo_h
#define SimpleSoftwareServo_h

#include "Arduino.h"

#include <stdint.h>

class SimpleSoftwareServo
{
  private:
    uint8_t pin;
    uint8_t angle;       // in degrees (0-180)
	uint16_t pulse;      // in us
    uint16_t min_pulse;       // minimum pulse, us
    uint16_t max_pulse;       // maximum pulse, us
    class SimpleSoftwareServo *next;
    static SimpleSoftwareServo* first;
  public:
    SimpleSoftwareServo();
    uint8_t attach(uint8_t pin);     // attach to a pin, sets pinMode, won't position the servo until a subsequent write() happens
    uint8_t attach(uint8_t pin, uint16_t newMin, uint16_t newMax); // as above but also sets min and max values for writes.
    void detach();
    void write(uint8_t);         // specify the angle in degrees, 0 to 180
    uint8_t read();
    uint8_t attached();
    void setMinimumPulse(uint16_t);  // pulse length for 0 degrees in microseconds, 540us default
    void setMaximumPulse(uint16_t);  // pulse length for 180 degrees in microseconds, 2400us default
    static void refresh();    // must be called at least every 50ms or so to keep servo alive; every time you call it, a pulse is generated, and up to 20ms will pass
	static const uint16_t refresh_interval = 20000; // in us
};

#endif
