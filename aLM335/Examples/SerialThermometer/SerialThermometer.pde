#include <aLM335.h>

/*  This example code will initialize an instance of the LM335 class using two
    non-default variables, and then, once per second, print over the serial
	line the values it receives.
*/

void setup() {
 
  Serial.begin(9600);
  
}

void loop() { 

  // We're using the two variable form of initialization, which defines both
  // the pin and the reference voltage.

  aLM335 tempsensor (0, 5);
  
  // Print the values the sensor generates in Kelvin, Celsius, and Fahrenheit

  Serial.println(tempsensor.getKelvin());
  Serial.println(tempsensor.getCelsius());
  Serial.println(tempsensor.getFahrenheit());

  // Wait a second, then do it again

  delay (1000);
  
}
