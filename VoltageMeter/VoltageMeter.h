/*
 * VoltageMeter.h
 * 
 * A simple utility class to read voltage. Uses a simple voltage divider
 * and the internal analog reference.
 *
 * (c) 2008 Sofian Audry | info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef VOLTAGE_METER_INC
#define VOLTAGE_METER_INC

#include "WProgram.h"
#include "analog_reference.h"

/*

Circuit schematics
------------------

            R1             R2
  Vin ----/\/\/\----*----/\/\/\---- GND
                    |
                    | Vout
                    |
               ANALOG PIN

Vin  : input voltage (the voltage we try to meter)
Vmax : the maximum value of the input voltage
Vout : the output voltage (the Vin remapped to 0 .. 1.1V)

Choose R1 and R2 CAREFULLY according to the following rule:
R1 = R2 x 1.1 / (Vmax - 1.1)
R2 = R1 x (Vmax - 1.1) / 1.1

A typical configuration is R1 = 1k and R2 = 4k. It assumes Vmax = 5.5V.

*IMPORTANT NOTICES*
1) It is recommended to test the circuit before, especially if Vmax > 6V,
   to make sure the voltage at the analog pin (Vout) does not get over 5V.
2) Since this system relies on the internal reference, you should NOT
   connect any external voltage source to the AREF pin.
3) ATmega168 datasheet has the following notice "Note that VREF is a high 
   impedance source, and only a capacitive load should be connected in a 
   system." which means in other words that "when VREF is used as an output, 
   you may not draw current from it. You may connect a capacitor to it to 
   eliminate any ripple that might be caused by noise on adjacent circuits."
   (this precision was mentionned by Peter Pryor, thanks).

Example Arduino code
------------------------------
#include "VoltageMeter.h"

#define VOLTAGE_AIN 0

VoltageMeter meter(VOLTAGE_AIN, 5.5);

void setup() {
  Serial.begin(9600);
}

void loop() {
  float voltage = meter.voltage();
  Serial.print((int)(voltage*1000), DEC); Serial.println(" mV");
}
  
*/
class VoltageMeter {
  byte _pin;
  float _vMax;
  
public:
  VoltageMeter(byte pin, float vMax)
    : _pin(pin), _vMax(vMax) 
  {}
  
  int analogReadVoltage(int nSamples = 1) const {
    byte currentReference = getAnalogReference();
    setAnalogReference(INTERNAL);
    float sum = 0;
    for (int i=0; i< nSamples; i++)
      sum += analogRead(_pin);
    setAnalogReference(currentReference);
    return (sum / nSamples);
  }
  
  float voltage(int nSamples = 1) const {
    return analogReadVoltage(nSamples) / 1023.0f * _vMax;
  }
};

#endif