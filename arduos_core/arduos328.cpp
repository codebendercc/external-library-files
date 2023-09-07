#include "arduos328.h"

/*void SYS::start()
{
  pinMode(13, OUTPUT);
  //SYS::emptyT = new SYS::Task(SYS::emptyF, 128);

  stack_addr = &SYS::emptyStack;

  // Set the Timer Mode to CTC
  TCCR0A |= (1 << WGM01);

  // Set the value that you want to count to
  OCR0A = 0xF9;

  TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect

  E         //enable interrupts

  TCCR0B |= (1 << CS01) | (1 << CS00);
  // set prescaler to 64 and start the timer
}*/
