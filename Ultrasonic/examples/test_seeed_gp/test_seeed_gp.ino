// test for seeed graphical programming project
// loovee @ 2015-8-4

#include "Ultrasonic.h"

Ultrasonic ultrasonic;
const int pin = 2;          // ultrasonic connect to D2

void setup()
{
    Serial.begin(115200);
    Serial.println("test for Seeed Graphical programming project");
    Serial.println("Ultrasonic");
}

void loop()
{
    int dist_cm = ultrasonic.UltrasonicRangerRead(pin);         // read data from ultrasonic ranger
    Serial.println(dist_cm);                                    // display it
    delay(100);
}