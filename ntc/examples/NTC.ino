/*
Honeywell resistor values:
T{¡ãC}       res. {Ohm}
¨C20.00	220680
25.00   20000
100.00  1110
*/


#include "ntc.h"

NTC ain0;
NTC ain3;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup(){
  int oldFreeRam = 0;

  Serial.begin(115200);

  // reserve 200 bytes for the inputString:
  inputString.reserve(10);

  oldFreeRam = freeRam();

  //The parameters could be generated with the site:
  //http://www.thinksrs.com/downloads/programs/Therm%20Calc/NTCCalibrator/NTCcalculator.htm

  //init Steinhart for Honeywell NTC20k sensors ;-)
  ain0.InitSteinhart(20000, 20000, 1.160215431e-3, 2.118982508e-4, 0.9808183064e-7, -50, 150);

  Serial.print(F("Ram used for Steinhart:"));
  Serial.println(oldFreeRam - freeRam());
  Serial.println();

  oldFreeRam = freeRam();
  //INIT(array steps,pull down resistor val, U, R25,B-value,Tmin, Tmax,ADC bits)
  //Parameters for Honeywell NTC20k : B=4027.07
  //Parameters for Vishay NTCASCWE3102J: R25=1k, B=3528
  ain3.Init(6,20000,20000,4027.07,-50,150);
  ain3.DoCalculation();
  ain3.Print();

  Serial.print(F("Ram used for interpolation:"));
  Serial.println(oldFreeRam - freeRam());

  Serial.print(F("free RAM:"));
  Serial.println(freeRam());

  Serial.println();
}

void loop(){

  static unsigned long cycleTime;
  static unsigned long previousMillis;

  //int sTemp = analogRead(A0);
  static int sTemp = analogRead(A0);
  double TS = 0;
  double TI = 0;

  if ((unsigned long)(millis() - previousMillis) >= 2000) {

    //calculate the program's cylce time begin
    cycle_time(cycleTime);

    //Use this for Steinhart calculation
    TS = ain0.GiveSteinhart(sTemp);

    //calculate the program's cylce time end
    cycle_time(cycleTime);
    Serial.print(F("Cycle time Steinhart [us]: "));
    Serial.println(cycleTime);

    //calculate the program's cylce time begin
    cycle_time(cycleTime);

    //Use this for interpolation
    TI = ain3.LinearInterpolate(sTemp);

    //calculate the program's cylce time end
    cycle_time(cycleTime);
    Serial.print(F("Cycle time interpolated [us]: "));
    Serial.println(cycleTime);

    Serial.print(F("ADC value: "));
    Serial.println(sTemp);

    Serial.print(F("Steinhart T: "));
    Serial.println(TS);

    Serial.print(F("Interpolated T: "));
    Serial.println(TI);

    Serial.println();


    // print the string when a newline arrives:
    if (stringComplete) {
      sTemp = inputString.toInt();

      // clear the string:
      inputString = "";
      stringComplete = false;
    }

    previousMillis = millis();
  }

}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void cycle_time(unsigned long &cycleTime) {
  unsigned long currentMicros = micros();
  static unsigned long previousMicros;
  static boolean isrun;

  if (isrun) {
    cycleTime = currentMicros - previousMicros;
    isrun = LOW;
  }
  else {
    previousMicros = micros();
    isrun = HIGH;
  }
}

//used from: http://www.controllerprojects.com/2011/05/23/determining-sram-usage-on-arduino/
int freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
