
#include "SCoopME.h"

volatile uint8_t SCoopAtomic;	// !=0 when the yield is not authorized

SCtaskObject * SCtaskTcb;		// point on the current Task environment
SCtaskObject   SCmainTcb;		// task environement for the main sketch
SCvoid_t 	   SCoopCheckIn;	// launch all the checkIn() for all registered objects

void yield() { 
 if (SCoopAtomic==0) SCmainTcb.yieldMain(); }

void sleep(unsigned time) __attribute__((weak));    
void sleep(unsigned time) { SCmainTcb.sleep(time); };

void sleepSync(unsigned time) __attribute__((weak));
void sleepSync(unsigned time) { SCmainTcb.sleepSync(time); };

void * SCheduler() __attribute__((weak));
void * SCheduler() { // default scheduler and typical example
  if (SCoopCheckIn) SCoopCheckIn();	// this calls in cascade all the checkin() 
  // myEvent.checkIn();
  // myTimer.checkIn();
  // return &&label1; label1:
  // myTask1.checkIn();
  // return &&label2; label2:
  // myTask2.checkIn();
return NULL; };

// all code below extracted from original SCOOP libray/no changes on low level code

/********* ASSEMBLY / LETS GET STARTED WITH THE COMPLEX THINGS **********/
// original idea for switching stack pointer taken out from ChibiOS. 
// Credit to the author. now slightly modified. 
// http://forum.pjrc.com/threads/540-ChibiOS-RTand-FreeRTOS-for-Teensy-3-0
//
// original idea for micros() optimization taken from CORE_TEENSY
// credit to Paul http://www.pjrc.com/teensy/
//************************************************************************/

#if defined(SCoop_ARM) && (SCoop_ARM == 1)

void SCoopSwitch(SCoopStack_t *&newSP, SCoopStack_t *&oldSP) __attribute__((naked,noinline)) ;
void SCoopSwitch(SCoopStack_t *&newSP, SCoopStack_t *&oldSP)
{ asm volatile (	"push    {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}" : : : "memory");
  asm volatile (	"str     sp, [%[oldsp], #0]  \n\t"
					"ldr     sp, [%[newsp], #0]" : : [newsp] "r" (newSP), [oldsp] "r" (oldSP));
  asm volatile (	"pop     {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, pc}" 
					: : : "memory");
};

#endif


#if defined (SCoop_MSP) && (SCoop_MSP == 1)

void SCoopSwitch(SCoopStack_t *&newSP, SCoopStack_t *&oldSP) __attribute__((naked,noinline)) ;
void SCoopSwitch(SCoopStack_t *&newSP, SCoopStack_t *&oldSP)
{	asm volatile (	"push	r4 \n\t push r5 \n\t push	r6 \n\t push r7 \n\t push r8 \n\tpush r9 \n\t push r10 \n\t push r11 \n\t"
					"push	r2			\n\t" 
					"mov.w	r1,@r14		\n\t"	
					"mov.w	@r15,r1		\n\t"	
					"pop	r2			\n\t"	
					"pop	r11 \n\t pop r10 \n\t pop r9 \n\t pop r8 \n\t pop r7 \n\t pop r6 \n\t pop r5 \n\t pop r4 \n\t" 
					"ret				\n\t"	
					: : [newsp] "r" (newSP), [oldsp] "r" (oldSP) : "memory"); };

#endif


#if defined(SCoop_AVR) && (SCoop_AVR == 1)

void SCoopSwitch(SCoopStack_t * & newSP, SCoopStack_t * & oldSP) __attribute__((naked,noinline));
void SCoopSwitch(SCoopStack_t * & newSP, SCoopStack_t * & oldSP)
{ asm volatile ("push r2  \n\t push r3  \n\t push r4  \n\t push r5  \n\t push r6  \n\t push r7  \n\t push r8  \n\t push r9  \n\t push r10 \n\t"
                "push r11 \n\t push r12 \n\t push r13 \n\t push r14 \n\t push r15 \n\t push r16 \n\t push r17 \n\t push r28 \n\t push r29");
  
  asm volatile ("movw    r28, r22"); // r22:r23 contains & oldSp
  asm volatile ("in      r2, 0x3d"); // SPL
  asm volatile ("in      r3, 0x3e"); // SPH
  asm volatile ("std     Y+0, r2");  // store the current SP into the pointer oldSP
  asm volatile ("std     Y+1, r3");

  asm volatile ("movw    r28, r24"); // r24:r25 contains & newSp
  asm volatile ("ldd     r2, Y+0");  // restore the SP from the pointer newSP
  asm volatile ("ldd     r3, Y+1");
  asm volatile ("in      r4, 0x3f"); // save SREG
  asm volatile ("cli             "); // just to be safe on playing with stack ptr :) (useless with xmega)
  asm volatile ("out     0x3e, r3"); // SPH
  asm volatile ("out     0x3d, r2"); // SPL
  asm volatile ("out     0x3f, r4"); // restore SREG asap (same approach as in setjmp.S credit to Marek Michalkiewicz)
  
  asm volatile ("pop r29 \n\t pop r28 \n\t pop r17 \n\t pop r16 \n\t pop r15 \n\t pop r14 \n\t pop r13 \n\t pop r12 \n\t pop r11 \n\t"
                "pop r10 \n\t pop r9  \n\t pop r8  \n\t pop r7  \n\t pop r6  \n\t pop r5  \n\t pop r4  \n\t pop r3  \n\t pop r2");
  asm volatile ("ret" ::: "memory" );  };

#endif

/******** NEW MICROS METHOD BASED ON CORE_TEENSY / LIMITED TO 16 BITS **********/

#if defined(SCoop_AVR) && (SCoop_AVR == 1)

/* Copyright (c) 2008-2010 PJRC.COM, LLC
 * for the Teensy and Teensy++
 * http://www.pjrc.com/teensy/
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#if F_CPU == 16000000L
  #define TIMER0_MICROS_INC  	4
#elif F_CPU == 8000000L
  #define TIMER0_MICROS_INC  	8
#elif F_CPU == 4000000L
  #define TIMER0_MICROS_INC  	16
#elif F_CPU == 2000000L
  #define TIMER0_MICROS_INC  	32
#elif F_CPU == 1000000L
  #define TIMER0_MICROS_INC  	64
#else
#define SCoopMicros()   ((SCmicros_t)micros())     // using the standard micros()
#warning "CPU Frequence not supported by the new micros() function"
#endif

#if defined(CORE_TEENSY)
extern volatile unsigned long timer0_micros_count;   // this variable is incremented by timer 0 overflow
SCmicros_t SCoopMicros16(void) // same as standrad PJRC micros, but in 16 bits and with inlining
{	register SCmicros_t out; 
	asm volatile(
		"in	__tmp_reg__, __SREG__"			"\n\t"
		"cli"								"\n\t"
		"in	%A0, %2"						"\n\t"
		"in	__zero_reg__, %3"				"\n\t"
		"lds	%B0, timer0_micros_count"	"\n\t"
		"out	__SREG__, __tmp_reg__"		"\n\t"
		"sbrs	__zero_reg__, %4"			"\n\t"
		"rjmp	L_%=_skip"					"\n\t"
		"cpi	%A0, 255"					"\n\t"
		"breq	L_%=_skip"					"\n\t"
		"subi	%B0, 256 - %1"				"\n\t"
	"L_%=_skip:"							"\n\t"
		"clr	__zero_reg__"				"\n\t"
		"clr	__tmp_reg__"				"\n\t"
#if F_CPU == 16000000L || F_CPU == 8000000L || F_CPU == 4000000L
		"lsl	%A0"						"\n\t"
		"rol	__tmp_reg__"				"\n\t"
		"lsl	%A0"						"\n\t"
		"rol	__tmp_reg__"				"\n\t"
#if F_CPU == 8000000L || F_CPU == 4000000L
		"lsl	%A0"						"\n\t"
		"rol	__tmp_reg__"				"\n\t"
#endif
#if F_CPU == 4000000L
		"lsl	%A0"						"\n\t"
		"rol	__tmp_reg__"				"\n\t"
#endif
		"or	%B0, __tmp_reg__"				"\n\t"
#endif
#if F_CPU == 1000000L || F_CPU == 2000000L
		"lsr	%A0"						"\n\t"
		"ror	__tmp_reg__"				"\n\t"
		"lsr	%A0"						"\n\t"
		"ror	__tmp_reg__"				"\n\t"
#if F_CPU == 2000000L
		"lsr	%A0"						"\n\t"
		"ror	__tmp_reg__"				"\n\t"
#endif
		"or	%B0, %A0"						"\n\t"
		"mov	%A0, __tmp_reg__"			"\n\t"
#endif
		: "=r" (out)
		: "M" (TIMER0_MICROS_INC),
		  "I" (_SFR_IO_ADDR(TCNT0)),
		  "I" (_SFR_IO_ADDR(TIFR0)), 
		  "I" (TOV0) : "r0" ); return out; }

		  
#else // end of CORE_TEENSY. Now same job for the Arduino wiring.c library

extern volatile unsigned long timer0_overflow_count; // use this variable which is incremented at each overflow
SCmicros_t SCoopMicros16(void) // same as standrad PJRC micros, but in 16 bits and with inlining
{	register SCmicros_t out ;
	asm volatile(
		"in	__tmp_reg__, __SREG__"			"\n\t"
		"cli"								"\n\t"
		"in	%A0, %2"						"\n\t"
		"in	__zero_reg__, %3"				"\n\t"
		"lds	%B0, timer0_overflow_count"	"\n\t"
		"out	__SREG__, __tmp_reg__"		"\n\t"
		"sbrs	__zero_reg__, %4"			"\n\t"
		"rjmp	L_%=_skip"					"\n\t"
		"cpi	%A0, 255"					"\n\t"
		"breq	L_%=_skip"					"\n\t"
#if F_CPU == 16000000L 
		"subi	%B0, 1"						"\n\t"
#elif F_CPU == 8000000L 
		"subi	%B0, 2"						"\n\t"
#endif		
	"L_%=_skip:"							"\n\t"
		"clr	__zero_reg__"				"\n\t"
		"clr	__tmp_reg__"				"\n\t"
#if F_CPU == 16000000L || F_CPU == 8000000L
		"lsl	%B0"						"\n\t"
		"lsl	%B0"						"\n\t"
		"lsl	%A0"						"\n\t"
		"rol	__tmp_reg__"				"\n\t"
		"lsl	%A0"						"\n\t"
		"rol	__tmp_reg__"				"\n\t"
#if F_CPU == 8000000L 
		"lsl	%B0"						"\n\t"
		"lsl	%A0"						"\n\t"
		"rol	__tmp_reg__"				"\n\t"
#endif
		"or	%B0, __tmp_reg__"				"\n\t"
#endif
		: "=r" (out)
		: "M" (TIMER0_MICROS_INC),
		  "I" (_SFR_IO_ADDR(TCNT0)),
		  "I" (_SFR_IO_ADDR(TIFR0)),
		  "I" (TOV0)
		: "r0" ); return out; }

#endif // for arduino

#endif

