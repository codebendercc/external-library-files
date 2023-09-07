/****************************************************************************
 * BEST VIEWED WITH NOTEPAD ++ or an editor with colored syntax             *
 ****************************************************************************
 * this is a new "light" front end revisited for SCOOP library              *
 * it uses small objects and templates to create a dynamique code           *
 * declaration is made easier by utilisation of structure and policy        *
 * classes, see examples !!!!! as the pdf document is not yet updated       *
 * https://code.google.com/p/arduino-scoop-cooperative-scheduler-arm-avr/   *
****************************************************************************
 * Author: fabrice oudert                                                   *
 * Creation date: 30 may 2013                                               *
 ***************************************************************************/

#ifndef _SCOOPME_H
#define _SCOOPME_H

#include <Arduino.h>

#if defined (__AVR__)

  #define SCoop_AVR 1
  typedef uint16_t SCmicros_t;	// define the size for any timers in microsecond. can NOT be changed on AVR
  typedef uint16_t SCmillis_t;	// define the size for any timers in milliseconds. can be changed to uint32_t
  extern SCmicros_t SCoopMicros16(void);
  inline SCmicros_t SCoopMicros() { return SCoopMicros16(); }
  inline SCmillis_t SCoopMillis() { return millis(); }	// standard arduino millis
  
  typedef uint8_t  SCoopStack_t; 
  const SCoopStack_t SCoopStackPattern = 0xA5;
  extern void SCoopSwitch(SCoopStack_t * & newSP, SCoopStack_t * & oldSP);
  inline SCoopStack_t* SCoopGetSP() { return (SCoopStack_t*)SP; }
  
  #define SCoopDefaultQuantum   400		// allow 400us minimum per task. can change between 100us and 10 000us
  #define SCoopDefaultStackSize 150    // to be experimented by user.


#elif defined (__MSP430MCU__)
  
  #define SCoop_MSP 1
  typedef uint16_t SCmicros_t;
  typedef uint16_t SCmillis_t;	// define the size for any timers in milliseconds. can be changed to uint32_t
  inline SCmicros_t SCoopMicros() { return micros(); }	// standard Energia micros
  inline SCmillis_t SCoopMillis() { return millis(); }	// standard Energia millis

  typedef uint16_t SCoopStack_t   __attribute__ ((aligned (2)));
  const SCoopStack_t SCoopStackPattern = 0xA55A;
  extern void SCoopSwitch(SCoopStack_t * & newSP, SCoopStack_t * & oldSP);
  inline __attribute__ ((always_inline)) SCoopStack_t* SCoopGetSP() { 
    register SCoopStack_t* val; asm ("MOV.W  r1,%[temp]" : [temp] "=r" (val)); return val; }

  #define SCoopDefaultQuantum   400    // allow 400us minimum per task. can change between 100us and 10 000us
  #define SCoopDefaultStackSize 100    // to be experimented...
  

#elif defined(__MK20DX128__)  || defined (__SAM3X8E__) // || defined(__arm__)

  #define SCoop_ARM 1
  typedef uint32_t   SCmicros_t
  typedef uint32_t   SCmillis_t;	// define the size for any timers in milliseconds.
  inline SCmicros_t SCoopMicros() { return micros(); }
  inline SCmillis_t SCoopMillis() { return millis(); }	// standard arduino millis

  typedef uint64_t   SCoopStack_t __attribute__ ((aligned (8)));
  const SCoopStack_t SCoopStackPattern = 0xA596695A;
  extern void SCoopSwitch(SCoopStack_t * & newSP, SCoopStack_t * & oldSP);
  inline __attribute__ ((always_inline)) SCoopStack_t* SCoopGetSP() {
    register SCoopStack_t* val; asm ("mov  %[temp],sp" : [temp] "=r" (val)); return val; }

  #define SCoopDefaultQuantum   200    //allow 400us minimum per task. can change between 50us and 10 000us
  #define SCoopDefaultStackSize 256    // should be a multiple of 8

#endif


// encapsulate the next block code within noInterrupt() and interrupts() // credits to Dean Camera
#ifndef ASM_ATOMIC
void    inline __SCoopInterrupts(const  uint8_t *__s) { interrupts(); }
uint8_t inline __SCoopNoInterrupts(void)              { noInterrupts(); return 1; }
#define ASM_ATOMIC for ( uint8_t __temp __attribute__((__cleanup__(__SCoopInterrupts))) = __SCoopNoInterrupts(); __temp  ; __temp = 0 )
#endif


// a standard type for indirect call of methods
typedef void * (*SCfunc_t)(void);	// returning a void ptr
typedef void   (*SCvoid_t)(void);	// returning nothing

// a basic object to abstract calls to millis()
struct SCtimerMs {   // in milliseconds
  SCmillis_t timer;	// internal variable
  //SCtimerMs()           	{ reset(); }
  SCmillis_t get()          { return  SCoopMillis() - timer; }
  void set(SCmillis_t val)  { timer = SCoopMillis() + val; }	
  void reset()  	    	{ set(0); }	
  void add(SCmillis_t val)  { timer -= val; }	// it s not a bug : substract to add time
  void sub(SCmillis_t val)  { timer += val; }
  operator SCmillis_t ()    { return get(); }
  SCtimerMs& operator=  (const SCmillis_t rhs) { set(rhs); return *this; }
};

// a basic object to abstract calls to micros()
struct SCtimerUs {   // in microseconds
  SCmicros_t timer;	// internal variable
  //SCtimerMs()           	{ reset(); }
  SCmicros_t get()          { return  SCoopMicros() - timer; }
  void set(SCmicros_t val)  { timer = SCoopMicros() + val; }	
  void reset()  	    	{ set(0); }	
  void add(SCmicros_t val)  { timer -= val; }
  void sub(SCmicros_t val)  { timer += val; }
  operator SCmicros_t () 	{ return get(); }
  SCtimerUs& operator=  (const SCmicros_t rhs) { set(rhs); return *this; }
};


// states of task/events
const uint8_t SCNEW   = 0;		// not initialized yet
const uint8_t SCSET   = 1;		// setup() has been called
const uint8_t SCRUN   = 2;		// inside the run()
const uint8_t SCSLEEP = 4;		// inside run but waiting something with sleep
const uint8_t SCPAUSE = 8;		// flagged as paused
const uint8_t SCTRIG  = 16;		// memorise previous state in events


// does nothing
struct SCoopNull { typedef SCoopNull SCoopNull_t; };
     
struct SCoopFalse {   
  static uint8_t read() { return 0; } };

struct SCoopTrue {   
  static uint8_t read() { return 1; } };

extern SCvoid_t SCoopCheckIn;    	// used to call all checkins
  
template<class CHILD = SCoopNull>

struct SCoop  {
    
  static uint8_t status;	
  
  static uint8_t getStatus() { return status; }

  static SCvoid_t prevCheckIn;
  
  static void cascadeCheckIn() {
     if (prevCheckIn) prevCheckIn();	// cascade checkins
	 CHILD::checkIn(); }  

   SCoop() {	// register the item "checkIn()" function
	prevCheckIn  = SCoopCheckIn;
	SCoopCheckIn = (SCvoid_t)CHILD::cascadeCheckIn; };
  
  static void pause()  { status |= SCPAUSE;  }
  
  static void resume() { status &= ~SCPAUSE; }
  
  static uint8_t paused() { return ((status & SCPAUSE)!=SCNEW); }
  
  // use to launch setup() then run() from user object
  static void checkIn() {
	if (status == SCNEW) {
	    CHILD::setup();			// launch the child setup, or the default one 
	    status = SCSET; 		// flag the object as "set"
	} else 
	    CHILD::run();			// execute the child "run" method which MUST exist in the child			
	}  
};

//specialization of each needed fields, to avoid linker error for "undefined reference"
template< class CHILD > uint8_t  SCoop< CHILD >::status;
template< class CHILD > SCvoid_t SCoop< CHILD >::prevCheckIn;


struct SCoopReadConfirm {
  static bool read()  { return false; }
  static void confirm(uint8_t status)   { };
};

///////////// EVENTS /////////

// class for creating events. run is launched if the TRIG::read() is raising from false to true
template< class CHILD, 
          class TRIG  = SCoopReadConfirm,
          uint8_t MODE = RISING >	// accept CHANGE or FALLING

struct SCoopEvent : SCoop< SCoopEvent< CHILD, TRIG, MODE > > {

  // use the inherited "enter" methode to launch setup() and run()
  using   SCoop< SCoopEvent< CHILD, TRIG, MODE > >::status;
  using   SCoop< SCoopEvent< CHILD, TRIG, MODE > >::pause;
  using   SCoop< SCoopEvent< CHILD, TRIG, MODE > >::paused;
  using   SCoop< SCoopEvent< CHILD, TRIG, MODE > >::resume;
  
  
  static void setup() {					// call user setup() only if it exists
    if (setup != CHILD::setup) 			// this avoid stupid recursion hang
	  CHILD::setup(); }

  
  static void run() {
    if (MODE==RISING) {	// compiler will select the right code depending on parameter
	  
	  if (TRIG::read() != 0) {
		if ((status & SCTRIG)==SCNEW) {	// just raising
	        status |= SCTRIG;			// memorize status
			TRIG::confirm(1);
			if (!paused())
			  CHILD::run(); }			// this is how we launch user events
	  } else { status &= ~SCTRIG; }
	
	} else 
	if (MODE==FALLING){
	  
	  if (TRIG::read() == 0 ) {
		if ((status & SCTRIG)) {		// just falling
	        status &= ~SCTRIG;			// memorize status
			TRIG::confirm(0);
			if (!paused())
				CHILD::run(); }			// this is how we launch user events
	  } else { status |= SCTRIG; } 
	
	} else {	// mode CHANGE

	  if ( (TRIG::read()!=0) ^ ((status & SCTRIG)!=SCNEW) ) {
         status ^= SCTRIG;				// change old status
		 TRIG::confirm((status & SCTRIG)!=0);
		 if (!paused())
			CHILD::run(); }				// this is how we launch user events
	} };

	
	static void yield() { // force generating an error if user try to launch yield() !
     enum  { error = 1/ (yield != CHILD::yield) }; }
};

///////////// TIMERS /////////

// class for creating "timers"
template< class CHILD, 
          unsigned TIME,
		  class SCTIME = SCtimerMs >

struct SCoopTimer : SCoop< SCoopTimer< CHILD, TIME, SCTIME > > {

  static SCTIME timer;
  static SCTIME thresold; 
  
  using   SCoop< SCoopTimer< CHILD, TIME, SCTIME > >::status;
  using   SCoop< SCoopTimer< CHILD, TIME, SCTIME > >::pause;
  using   SCoop< SCoopTimer< CHILD, TIME, SCTIME > >::paused;
  using   SCoop< SCoopTimer< CHILD, TIME, SCTIME > >::resume;
  // use the inherited "checkIn" methode to launch setup() and run()

  
  static void setPeriod(unsigned time) {		// setup timer thresold
    thresold.timer = time; }
	

  unsigned getPeriod() {
    return thresold.timer; }
  

  static void setup() {					// call user setup() only if it exists
    setPeriod(TIME);					// set the period
	if (setup != CHILD::setup) 			// this avoid stupid recursion if user doesnt declare "setup()"
	  CHILD::setup();
	timer = 0; }

  
  static void run() { 	// this code will be generated for each timer as everything is static
    if (thresold.timer) {				// check if thresold has been defined
	  if (timer >= thresold.timer) {	// then verify if time is over
	      timer.sub(thresold.timer);	// update timer for next check
          if (!paused())				// if not paused
			 CHILD::run(); } }			// launch user method which has to be define in user code
  }

    
  static void yield() { // force generating and error if user try to launch yield() !
     enum  { error = 1/ (yield != CHILD::yield) }; }
  
};
//specialization of the timer, to avoid linker error
template<class CHILD, unsigned TIME, class SCTIME > SCTIME SCoopTimer< CHILD, TIME, SCTIME >::timer;
template<class CHILD, unsigned TIME, class SCTIME > SCTIME SCoopTimer< CHILD, TIME, SCTIME >::thresold;


///////////// TASKS /////////


inline void SCoopStackFill(SCoopStack_t * startp, SCoopStack_t * endp) 
{ if (startp) while (startp < endp) *startp++ = SCoopStackPattern; };      


inline unsigned SCoopStackSearch(SCoopStack_t * startp, SCoopStack_t * endp) 
{ SCoopStack_t * ptr = startp;
  while (ptr < endp) if (*ptr++ != SCoopStackPattern) break;
  return ((unsigned)ptr-(unsigned)startp-1); 
};


// specific variable and methods from SCoopME.cpp 
extern volatile uint8_t SCoopAtomic;	// this block the yield mechanism if value != 0
extern void *  SCheduler();				// from SCoopME.cpp. can be overloaded by user
extern void yield();					// the new yield which can be used "every where"
extern void sleep(unsigned time);		// a generic sleep method, calling yield
extern void sleepSync(unsigned time);	// same , with not jitter (sync on previous value)

struct SCtaskObject;
extern SCtaskObject * SCtaskTcb;		// point on the current task context, or NULL if in Loop or timers or events
extern SCtaskObject SCmainTcb;			// this record contain the main sketch task informations


// object containing task variable (tcb) and non static primitive, to minimize code size
struct SCtaskObject {
  
  SCoopStack_t * pStack;		// this is basically a copy of the SP register after/before switching
  SCoopStack_t * * nextStack;	// potentially point to the next task to switch-in otherwise point on tcb.pStack
  
  SCtimerMs timerSleep;			// just used by sleep(xxx)
  
  SCtimerMs timerSleepSync;		// used for sleepSync(xxx). permanent/static variabe
  
  SCtimerUs timerYield;			// time checker for task quantum in microseconds

  // this is the key piece of cake of this library . cheers.
  void init(SCoopStack_t * stackBegin, SCoopStack_t * stackEnd, void(*run)()) {

	timerSleepSync = 0;
	timerYield = 0;

    register SCoopStack_t* pEnd;
	register SCoopStack_t* pSource;

	noInterrupts();                         // de activate interrupt so we can access the stack content for further copy/paste
	SCoopSwitch(SCmainTcb.pStack,SCmainTcb.pStack); // simulate switching but with current context : back to same place !                                               
                                     
	if (pStack) { SCtaskTcb=this; run(); }	// this will be executed only when we comeback here with the first scoopswitch		
					
    pStack  = stackEnd ;					// prepare task stack to the top of the space provided
    pEnd    = SCmainTcb.pStack;				// the variable as been initialized by previous Scoopswitch
	pSource = SCoopGetSP(); 				// start copying from the actual stack pointer position
	do { *pStack-- = *pSource--; }			// we copy all the stack context to the newly pStack space,  
	while (pSource != pEnd); 				// this includes the previous return adress (@!@) yep
											// so we ll endup just below the previous call to scoopswitch above (@\_/@)		                                             
    interrupts();							// we can restore interrupts as we are finished with critical stack handling
	SCoopStackFill(stackBegin, pStack);		// fill with patern in order to calculate StackLeft later
  }


  void leave() {
    if (SCoopAtomic) return;
	SCtaskTcb=NULL;
	SCoopSwitch(*nextStack,pStack);			// switch to next task, or scheduler...
	SCtaskTcb=this;							// we ll be right back here !
  }


  void enter() {							// should be called only from main sketch/loop()
     SCoopSwitch(pStack,SCmainTcb.pStack);	// save current context and switch to our task :)
  }


  void yield(SCmicros_t time) { 			// use to switch to next task or scheduler
	if (time) 								// imediately or after some delay
		if (timerYield < time) return;		// give a certain amount of time to our task
	yield0();
  }

  void yield() { yield0(); }				// switch imediately
  
  void yield0() { 	
	  leave();								// go back to scheduler or other tasks
	  timerYield = 0; 						// restart microsec timer
  } 

  
  static void yieldMain() {   				// this version can be called from the main sketch loop()
  static SCfunc_t SChedulerAddr;			// local variable containing return adress
  if (SCtaskTcb) SCtaskTcb->yield();		// check if we are in a SCoopTask object
  else 
    if (SChedulerAddr) 						// if we ve been given a special return label
	     SChedulerAddr = (SCfunc_t)SChedulerAddr();	// call this label in scheduler
    else SChedulerAddr = (SCfunc_t)SCheduler();	// call standard scheduler entry
  } 	
  
  
  void sleep(unsigned time) { 				// do yield until time is ellapsed
	if (time) {
	  timerSleep = 0;
	  while (timerSleep < time) yieldMain(); }
  }
  
  void sleepSync(unsigned time) { 		// do yied until time is ellapsed. no jitter
    if (time==0) timerSleepSync = 0; 
	else { 
	  while (timerSleepSync < time) yieldMain();
	  timerSleepSync.sub(time); }
  }
  
};


// object to create user tasks
template< class CHILD, 
          unsigned STACK   = SCoopDefaultStackSize, 
		  SCmicros_t TIME  = SCoopDefaultQuantum >

struct SCoopTask : SCoop< SCoopTask< CHILD, STACK, TIME > > {
  
  static SCtaskObject tcb;	// a static instance of the key variables for this task
    
  using   SCoop< SCoopTask< CHILD, STACK, TIME > >::status;
  using   SCoop< SCoopTask< CHILD, STACK, TIME > >::pause;
  using   SCoop< SCoopTask< CHILD, STACK, TIME > >::resume;
  using   SCoop< SCoopTask< CHILD, STACK, TIME > >::paused;
  
  
  static void run() {
    for (;;) {
	   if (!paused()){
	     status = SCRUN;
		 if (run != CHILD::run ) 
		      CHILD::run();						// seems we have a run method in CHILD : use it instead of loop
	     else CHILD::loop(); }					// call the user loop infinitely
	   yield(); }
	}

  
  static void setup() { }						// in case the user doesnt have his own setup()
  
  
  static void checkIn() {						// to be called from the main setup() scheduler
	checkIn(&SCmainTcb.pStack);	}				// this force yield to return to scheduler by default
  
  static void checkIn(SCoopStack_t ** addr) {		// gives the possibility to directly switch to the next task

  if (status == SCNEW) {						// called ONLY form the setup() of the main sketch

	  tcb.nextStack = addr; 
	  CHILD::setup();							// call the derived setup, if exist
	  status = SCSET; 							// ready for switching

	  tcb.init( _stack, 						// and prepare stack for context switching
	            _stack + sizeof(_stack)-sizeof(SCoopStack_t),// last element
				 run ); 
				 }
	else 
	   if (!paused()) 							// unless we are paused
	     tcb.enter();							// we enter in our task
   }
  

  static void yield0() { 	
	do { tcb.yield0();  } while (paused());
  }
  static void yield(SCmicros_t time) { 
    do { tcb.yield(time); }  while (paused());
  }

  static void yield() { 
	if (TIME)  yield(TIME);
	else yield0();
  }


  static void sleep(unsigned time)     { tcb.sleep(time); }
  
  static void sleepSync(unsigned time) { tcb.sleepSync(time); }

  
  static uint16_t stackLeft() 
  { if (status > SCNEW) return SCoopStackSearch(_stack, tcb.pStack);
    else return 0; }

  
  // creation of the task stack
   static SCoopStack_t _stack[STACK/sizeof(SCoopStack_t)];	//round lower
};

// specialization of static fields ......
template<class CHILD, uint16_t STACK, uint16_t TIME> SCtaskObject SCoopTask<CHILD, STACK, TIME>::tcb;
template<class CHILD, uint16_t STACK, uint16_t TIME> SCoopStack_t SCoopTask<CHILD, STACK, TIME>::_stack[STACK/sizeof(SCoopStack_t)];

	
// possibility to use this excellent trick for declaring non-yield section with macro SCoopATOMIC { .. code ... } credits to Dean Camera!!!
#ifndef yieldATOMIC
void    inline __decAtomic(const  uint8_t *__s) { --SCoopAtomic; }
uint8_t inline __incAtomic(void)                { ++SCoopAtomic; return 1; }
#define SCoopATOMIC for ( uint8_t __temp __attribute__((__cleanup__(__decAtomic))) = __incAtomic(); __temp  ; __temp = 0 )
#define yieldATOMIC SCoopATOMIC
#else
#define SCoopATOMIC yieldATOMIC
#endif

// see original SCoop V1.2 documentation

#ifndef yieldPROTECT
void    inline __SCoopUnprotect(uint8_t* *__s)  { uint8_t* staticFlag = *__s; *staticFlag = 0; };
#define SCoopPROTECT() static uint8_t __SCoopProtect = 0; \
register uint8_t* __temp __attribute__((__cleanup__(__SCoopUnprotect)))=& __SCoopProtect; \
while (__SCoopProtect) yield(); __SCoopProtect = 1; 
#define yieldPROTECT() SCoopPROTECT()
#else
#define SCoopPROTECT() yieldPROTECT()
#endif


#ifndef yieldUNPROTECT
#define SCoopUNPROTECT() { __SCoopProtect = 0; }
#define yieldUNPROTECT() SCoopUNPROTECT()
#else
#define SCoopUNPROTECT() yieldUNPROTECT()
#endif
  

#endif