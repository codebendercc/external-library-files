/*
A library to controll the TB6612FNG Toshiba H-bridge
-----------------------------------------------------------------

The pins must be connected accordingly (see the datasheet of the H-bridge).
    -the PWM pins must support PWM on the Arduino
    -the enable pin on the H-bridge should be connected either to 5V
     or to a pin on the Arduino, which is set to HIGH, for the H-bridge to function

For support or advice (if I am able to advise), contact me: shrajtofle@gmail.com.
I hope this library will help you in your projects!
*/
#include <arduino.h>

#define FORWARD true
#define REVERSE false

class Motor
{
 private:
   byte PWM_pin;
   byte FWD_pin;
   byte REV_pin;
   boolean rotating;
   boolean direction;
   byte speed;
   byte speed_regulation;
 public:
   Motor(byte pin1, byte pin2, byte pin3);
   void Rotate();
   void Stop();
   void ChangeSpeed(byte n_spd);
   void ChangeDirection(boolean n_dir);
   void ChangeSpeedRegulation(byte n_spd_reg);
   byte GetSpeedRegulation();
};
