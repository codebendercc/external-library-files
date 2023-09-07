#ifndef ARDUOS328_H
#define ARDUOS328_H

#include "../arduos_core/arduos_core.h"

extern void** stack_addr;

namespace SYS
{
  extern Task* currentTask;
  extern BasicScheduler* master;
  extern void* emptyStack;
  extern int (*currentFunc)();
  extern bool first;
  extern bool state;
  extern byte cnt;
}

#define SYS_start() \
{\
  pinMode(13, OUTPUT);\
  stack_addr = &SYS::emptyStack;\
  TCCR0A |= (1 << WGM01);\
  OCR0A = 0xF9;\
  TIMSK0 |= (1 << OCIE0A);\
  E\
  TCCR0B |= (1 << CS01) | (1 << CS00);\
}\

#define SYS_enable \
ISR(TIMER0_COMPA_vect) __attribute__ ((signal, naked));\
\
ISR(TIMER0_COMPA_vect)  \
{\
  SYS_storeContext();\
  SYS_getStack();\
  {\
    {\
      SYS::master->milli();\
      SYS::first = false;\
      SYS::cnt++;\
      if(SYS::cnt >= 100)\
      {\
        SYS::state = !SYS::state;\
        digitalWrite(13, SYS::state);\
        SYS::cnt = 0;\
      }\
    }\
    {\
      SYS::currentTask = SYS::master->get();\
      if(SYS::currentTask == (SYS::Task*)0x0000)\
      {\
        stack_addr = &SYS::emptyStack;\
      }\
      else\
      {\
        if(SYS::currentTask->called == false)\
        {\
          SYS::first = true;\
          SYS::currentFunc = SYS::currentTask->main;\
          SYS::currentTask->called = true;\
        }\
        stack_addr = &(SYS::currentTask->stack);\
      }\
    }\
  }\
  if(SYS::first)\
  {\
    SYS_setStack();\
    SYS_invoke(SYS::currentFunc)\
  }\
  else\
  {\
    SYS_setStack();\
    SYS_loadContext();\
    asm volatile("RETI");\
  }\
}

#endif // ARDUOS328_H
