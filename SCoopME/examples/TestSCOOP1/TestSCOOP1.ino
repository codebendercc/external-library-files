

#include "SCoopME.h"
#include "GPIOPIN.h"

extern volatile uint16_t s;


//GPIO_PIN<PORT_D, 6 > LED;
DIGITAL_PIN< 13  > LED1;

volatile uint16_t count1;
volatile uint16_t count2;
volatile uint16_t count3;

struct Task1 : SCoopTask< Task1, 100> {

  //static void setup() {  }
  
  static void loop() { count1++; sleep(100);  }

} myTask1;

struct myTask2 : SCoopTask< myTask2, 100> {	// 100 bytes stack. inside at least during 500us

  static void setup() {  }
  
  static void loop()  {  count2++; sleepSync(10);   }

} myTask2;

struct myTask3 : SCoopTask< myTask3, 70> {	// 100 bytes stack. inside at least during 500us

  static void setup() {  }
  
  static void loop()  {  count3++; sleepSync(10);   }

} myTask3;

/*
struct myEvent : SCoopEvent< myEvent > {
  static void setup() { s=0x4444; s=(uint16_t)&status; }
  static void run()   { LED2.toggle(); }
} myEvent;



*/
struct myTimer : SCoopTimer< myTimer, 500 > {	//  every 500ms
  static void setup() { Serial.begin(57600); }
  static void run() { Serial.print("1:");Serial.println(count1); LED1.toggle(); 
  Serial.print("stak1 ");Serial.println(myTask1.stackLeft());
  Serial.print("stak2 ");Serial.println(myTask2.stackLeft()); 
  }
} myTimer;
/*

struct myTimer2 : SCoopTimer< myTimer2, 100 > {	// every 100ms
  static void run()   { count1++; }
} myTimer2;
*/
struct myTimer3 : SCoopTimer< myTimer3, 1000 > {	// every 100ms
  static void run()   { count2++; Serial.print("2:");Serial.println(count2); }
} myTimer3;



void setup() { 
LED1.output(); 
LED1=1; 
Serial.println("HELLO");
}

void * SChedulerss() {
  //myTimer
return NULL;
}

void loop() { 
  yield();
  if (count1>=30) { count1=0; Serial.print("reset "); Serial.println((unsigned)SCoopGetSP());}
  } 