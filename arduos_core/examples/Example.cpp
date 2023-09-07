
#include <arduos_core.h>  //this file contains almost everything, except board-specific things
#include <arduos328.h>    //board specific things for Atmega328
#include <avr/io.h>       //and
#include <avr/interrupt.h>//for the interrupt

SYS_enable  //has to be written. it "enables" the interrupt function

SYS::Task *task0;
SYS::Task *task1;
SYS::Task *task2;
SYS::Task *task3;  //pointers to the tasks, that will be scheduled
SYS::PrioScheduler *prio0;  //a PrioScheduler will execute the first thing thats available(you can select if tasks come before schedulers or in reverse)

int fnc0()
{
  while(1)  //for now, you can stop a Task only with setting its delay to 0xFFFFFFFF or taking it from the scheduler. its a bad idea to let it return.
  {
    digitalWrite(9, HIGH);
    task0->delay = 1250;
    I  //activate the interrupt
    digitalWrite(9, LOW);
    task0->delay = 1250;
    I  //activate the interrupt
  }
}

int fnc1()
{
  while(1)  //for now, you can stop a Task only with setting its delay to 0xFFFFFFFF or taking it from the scheduler. its a bad idea to let it return.
  {
    digitalWrite(10, HIGH);
    task1->delay = 750;
    I  //activate the interrupt
    digitalWrite(10, LOW);
    task1->delay = 750;
    I  //activate the interrupt
  }
}

int fnc2()
{
  while(1)  //for now, you can stop a Task only with setting its delay to 0xFFFFFFFF or taking it from the scheduler. its a bad idea to let it return.
  {
    digitalWrite(11, HIGH);
    task2->delay = 1000;
    I  //activate the interrupt
    digitalWrite(11, LOW);
    delay(1000);
  }
}

int fnc3()
{
  while(1)  //for now, you can stop a Task only with setting its delay to 0xFFFFFFFF or taking it from the scheduler. its a bad idea to let it return.
  {
    digitalWrite(12, HIGH);
    task3->delay = 250;
    I  //activate the interrupt
    digitalWrite(12, LOW);
    task3->delay = 250;
    I  //activate the interrupt
  }
}

// on all the controlled pins (8-12) should be leds(or what you want to use...).

void setup()
{
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);

  task0 = new SYS::Task(fnc0, 64);
  task1 = new SYS::Task(fnc1, 64);
  task2 = new SYS::Task(fnc2, 64);
  task3 = new SYS::Task(fnc3, 64);  //the constructor:(int(*main)(), unsigned int stacksize, unsigned long delay = 0);

  prio0 = new SYS::PrioScheduler(SYS::task_sect);
  prio0->append(task2);
  prio0->append(task3);

  SYS::master = new SYS::RoundScheduler();  //the SYS::master is the top-level scheduler
  SYS::master->append(task0);
  SYS::master->append(task1);
  SYS::master->append(prio0);

  SYS_start();  //starts the multitasking
}

void loop()  //why is 8 also blinking?
{
  digitalWrite(8, LOW);
  delay(500);
  digitalWrite(8, HIGH);
  delay(500);
}  //answer: if there is nothing else to execute, the code below SYS_start() will be executed.
