//
// ArduinoTimer is distributed under the FreeBSD License
//
// Copyright (c) 2013, Carlos Rafael Gimenes das Neves
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met: 
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution. 
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those
// of the authors and should not be interpreted as representing official policies, 
// either expressed or implied, of the FreeBSD Project.
//
// https://github.com/carlosrafaelgn/ArduinoTimer
//
#ifndef Timer1_h
#define Timer1_h

// Reference documentation:
// http://www.atmel.com/devices/atmega2560.aspx
// http://www.atmel.com/devices/atmega328.aspx
//
// Timer 1 is available on ATmega168, ATmega328 and on ATmega2560
// All other timers are only available on ATmega2560
//
// (The page and chapter numbers refer to the ATmega2560 documentation)

#include <inttypes.h>

// 16.9.6 TIMSK0  Timer/Counter Interrupt Mask Register (page 134)
#ifndef disableMillis
#define disableMillis() TIMSK0 = 0
#endif
#ifndef enableMillis
#define enableMillis() TIMSK0 = 1
#endif

extern uint8_t __timer1Control;
extern uint16_t __timer1CounterValue;
#define readTimer1Unsafe() TCNT1
#define resetTimer1Unsafe() TCNT1 = __timer1CounterValue
#define pauseTimer1() TCCR1B = 0
#define resumeTimer1() TCCR1B = __timer1Control
extern void startTimer1(uint32_t microsecondsInterval);
extern void startCountingTimer1(void);
extern uint16_t readTimer1(void);
extern void resetTimer1(void);

// 17.9.1 Normal Mode (page 149)
// The simplest mode of operation is the Normal mode (WGMn3:0 = 0). In this mode the counting
// direction is always up (incrementing), and no counter clear is performed. The counter simply
// overruns when it passes its maximum 16-bit value (MAX = 0xFFFF) and then restarts from the
// BOTTOM (0x0000). In normal operation the Timer/Counter Overflow Flag (TOVn) will be set in
// the same timer clock cycle as the TCNTn becomes zero. The TOVn Flag in this case behaves
// like a 17th bit, except that it is only set, not cleared. However, combined with the timer overflow
// interrupt that automatically clears the TOVn Flag, the timer resolution can be increased by software.
// There are no special cases to consider in the Normal mode, a new counter value can be
// written anytime.
#define timer1Event TIMER1_OVF_vect

#endif
