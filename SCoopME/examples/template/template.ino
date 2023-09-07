/****************************************************************************
 * BEST VIEWED WITH NOTEPAD ++ or an editor with colored syntax             *
 ****************************************************************************
 * this is a template for the new "light" front end revisited SCOOP library *
 ****************************************************************************
 * Author: fabrice oudert                                                   *
 * Creation date: 31 may 2013                                               *
 ***************************************************************************/

#include "SCoopME.h"


struct task1 : SCoopTask< task1 > {	// default stack (150bytes) and quantum time (400us)
static void setup() { }
static void loop() { 
 // user code goes here. by default a yield() is executed at the end of this loop.
 // code can be blocking as long as it calls yield() or sleep(xx) time to time
} 
} task1;


struct task2 : SCoopTask< task2, 100 > {	// allocate 100bytes for stack
static void loop() { 						// example without setup()
  // user code here
} 
} task2;


struct task3: SCoopTask< task3, 100, 150 > {// allocate 100bytes for stack and 150us
static void loop() { 						// example without setup()
  // user code here
} 
} task3;


struct timer1 : SCoopTimer< timer1, 100 > { // every 100ms
static void run() { 
  // user code go here. code must NOT be blocking in timers, and must NOT use yield()
}
} timer1;


SCtimerMs myTimer;		// exemple of an event based on a timer value
struct myTrigger1 {		// user must define a structure with "read" and "confirm"
static bool read() { return (myTimer > 1000); };
static void confirm(uint8_t status) { myTimer.add(1000); }
};

struct myEvent : SCoopEvent< myEvent, myTrigger1, RISING > {
static void run() { 
  // user code goes here and will be executed when myTriger1.read() is true
}
} myEvent;


SCtimerMs timer;					// a basic uint16 timer in millisecond

void setup()
{ timer=0; 
  // user setup here below. tasks will be automatically setup() by next call to yield.
  
}

void loop() {
  
  yield();  // orchestrate everything.
  
  if (timer>=1000) { timer=0;
    // this code is ran every second
  } 
}