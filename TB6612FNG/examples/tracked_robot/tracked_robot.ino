#include <TB6612FNG.h>

/*Pins for the left track*/
byte left_PWM_pin = 6;
byte left_FWD_pin = 5;
byte left_REV_pin = 7;

/*Pins for the right track*/
byte right_PWM_pin = 3;
byte right_FWD_pin = 4;
byte right_REV_pin = 2;

Motor *left_track;
Motor *right_track;

void setup()
{
 left_track = new Motor(6, 5, 7);
 right_track = new Motor(3, 4, 2);

    /*Full speed*/
 left_track->Change_Speed(255);
 right_track->Change_Speed(255);
}

void loop()
{
    /*Go forward for half a second*/
 left_track->Change_Direction(FORWARD);
 right_track->Change_Direction(FORWARD);

 left_track->Rotate();
 right_track->Rotate();

 delay(500);

    /*Go left for half a second*/
 left_track->Change_Direction(REVERSE);
 delay(500);

    /*Go right for half a second*/
 left_track->Change_Direction(FORWARD);
 right_track->Change_Direction(REVERSE);
 delay(500);

    /*Go back for half a second*/
 left_track->Change_Direction(REVERSE);
 delay(500);
}
