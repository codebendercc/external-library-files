#include <SimpleSoftwareServo.h>

SimpleSoftwareServo *SimpleSoftwareServo::first;

#define NO_ANGLE (0xff)

SimpleSoftwareServo::SimpleSoftwareServo() : pin(0),angle(NO_ANGLE),pulse(0),min_pulse(544),max_pulse(2400),next(0)
{}

void SimpleSoftwareServo::setMinimumPulse(uint16_t t)
{
	min_pulse = t;
}

void SimpleSoftwareServo::setMaximumPulse(uint16_t t)
{
	max_pulse = t;
}

uint8_t SimpleSoftwareServo::attach(uint8_t pinArg)
{
	pin = pinArg;
	angle = NO_ANGLE;
	pulse = 0;
	next = first;
	first = this;
	pinMode(pin,OUTPUT);
	digitalWrite(pin,LOW);
	return 1;
}

uint8_t SimpleSoftwareServo::attach(uint8_t pin, uint16_t newMin, uint16_t newMax)
{
	min_pulse = newMin;
	max_pulse = newMax;
	return attach(pin);
}

void SimpleSoftwareServo::detach()
{
	for ( SimpleSoftwareServo **p = &first; *p != 0; p = &((*p)->next) ) {
		if ( *p == this) {
			*p = this->next;
			this->next = 0;
			return;
		}
	}
}

void SimpleSoftwareServo::write(uint8_t angleArg)
{
	if (angleArg < 0) angleArg = 0;
	if (angleArg > 180) angleArg = 180;
	angle = angleArg;
	pulse = map(angle,0,180,min_pulse,max_pulse);
}

uint8_t SimpleSoftwareServo::read()
{
	return angle;
}

uint8_t SimpleSoftwareServo::attached()
{
	for ( SimpleSoftwareServo *p = first; p != 0; p = p->next ) {
	if ( p == this) return 1;
	}
	return 0;
}

void SimpleSoftwareServo::refresh()
{
	static uint16_t last_refresh = 0;
	uint8_t count = 0;
	uint8_t i = 0;
	SimpleSoftwareServo *p;
	
    for (p = first; p != 0; p = p->next ) if (p->pulse) count++;
    if (count == 0) return;
	
    // gather all the SimpleSoftwareServos in an array
    SimpleSoftwareServo *s[count];
    for ( p = first; p != 0; p = p->next ) {
		if (p->pulse) {
			s[i++] = p;
		}
	}

	// insertion-sort the servos by pulse time, ascending
	for (i=1; i<count; i++) {
		uint8_t h = i;
		while (h>0  &&  s[h]->pulse < s[h-1]->pulse) {
			p      = s[h];
			s[h]   = s[h-1];
			s[h-1] = p;
			h--;
		}
	}
	
	// wait until we need to fire the next pulse: 20ms - (max pulse this time) - (time since last pulse ended)
	delayMicroseconds(refresh_interval - s[count-1]->pulse - (micros() - last_refresh));

	// turn up all the servo pins and note the time
	uint16_t pulse_start = micros();
    for (i = 0; i < count; i++) digitalWrite(s[i]->pin, HIGH);
	
	// turn them off one at a time in order
    for (i = 0; i < count; i++) {
		int16_t d = s[i]->pulse - (micros() - pulse_start); // signed so we can detect underflow, which happens when two pulse end times are close together
		if (d>0) delayMicroseconds(d);
		digitalWrite( s[i]->pin,0);
    }

	// note the time for the next pass
	last_refresh = micros();

}
