#include "TB6612FNG.h"

Motor::Motor(byte pin1, byte pin2, byte pin3)
{
 PWM_pin = pin1;
 FWD_pin = pin2;
 REV_pin = pin3;
 pinMode(PWM_pin, OUTPUT);
 pinMode(FWD_pin, OUTPUT);
 pinMode(REV_pin, OUTPUT);
 rotating = false;
 direction = FORWARD;
 speed = 255;
 speed_regulation = 255;
}
void Motor::Rotate()
{
 if (direction == FORWARD)
 {
  digitalWrite(FWD_pin, HIGH);
  digitalWrite(REV_pin, LOW);
  analogWrite(PWM_pin, speed);
 } else
 {
  digitalWrite(FWD_pin, LOW);
  digitalWrite(REV_pin, HIGH);
  analogWrite(PWM_pin, speed);
 }
 rotating = true;
}
void Motor::Stop()
{
 digitalWrite(FWD_pin, LOW);
 digitalWrite(REV_pin, LOW);
 analogWrite(PWM_pin, 0);
 rotating = false;
}
void Motor::ChangeSpeed(byte n_spd)
{
 speed = map(n_spd, 0, 255, 0, speed_regulation);
 if (rotating)
  Rotate();
}
void Motor::ChangeDirection(boolean n_dir)
{
 direction = n_dir;
 if (rotating)
  Rotate();
}
void Motor::ChangeSpeedRegulation(byte n_spd_reg)
{
 speed_regulation = n_spd_reg;
 if (rotating)
  Rotate();
}
byte Motor::GetSpeedRegulation()
{
 return speed_regulation;
}
