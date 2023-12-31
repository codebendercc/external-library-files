#include "arduos_core.h"

SYS::Task::Task(int (*nMain)(), byte stack_cnt)
{
  main = nMain;
  delay = 0;
  called = false;
  mem = new byte[stack_cnt];
  stack = (void*)(&(mem[stack_cnt - 1]));
}

SYS::Task::Task(int (*nMain)(), byte stack_cnt, ulong nDelay)
{
  main = nMain;
  delay = nDelay;
  called = false;
  mem = new byte[stack_cnt];
  stack = (void*)(&(mem[stack_cnt - 1]));
}

SYS::Task::~Task()
{
  delete []mem;
}

void SYS::Task::setDelay(ulong nDelay)
{
  delay = nDelay;
  ::delay(1);
}

// SYS::BasicScheduler::BasicScheduler(...)

SYS::BasicScheduler::BasicScheduler()
{
  task_count = 0;
  scheduler_count = 0;
  tasks = 0x0000;
  schedulers = 0x0000;
}

SYS::BasicScheduler::~BasicScheduler()
{
  {
    byte b;
    for(b = 0; b < scheduler_count; b++)
    {
      delete schedulers[b];
    }
    for(b = 0; b < task_count; b++)
    {
      delete tasks[b];
    }
  }
  delete [] schedulers;
  delete [] tasks;
}

//SYS::RoundScheduler acsess-base (called by "get()")

SYS::Task *SYS::BasicScheduler::getTask()
{
  return (SYS::Task *)0x0000;
}

SYS::Task *SYS::BasicScheduler::getScheduler()
{
  return (SYS::Task *)0x0000;
}

SYS::Task *SYS::BasicScheduler::get()
{
  return (SYS::Task *)0x0000;
}

//Is called for decrementing the delays generated by SYS::sleep()   called by another scheduler or ISR
void SYS::BasicScheduler::milli()
{
  byte b;
  for(b = 0; b < scheduler_count; b++)
  {
    schedulers[b]->milli();
  }

  for(b = 0; b < task_count; b++)
  {
    if(tasks[b]->delay < 0xFFFFFFFF && tasks[b]->delay > 0)
      tasks[b]->delay--;
  }
}

//called by the program to append an Object to the proper array

void SYS::BasicScheduler::append(SYS::Task *nTask)
{
  SYS::Task **tmp = tasks;
  tasks = new SYS::Task*[task_count + 1];
  for(byte i = 0; i < task_count; i++)
  {
    tasks[i] = tmp[i];
  }
  tasks[task_count] = nTask;
  task_count++;
  delete[] tmp;
}

void SYS::BasicScheduler::append(SYS::BasicScheduler *nBasicScheduler)
{
  SYS::BasicScheduler **tmp = schedulers;
  scheduler_count++;
  schedulers = new SYS::BasicScheduler*[scheduler_count];
  for(byte i = 0; i < scheduler_count - 1; i++)
  {
    schedulers[i] = tmp[i];
  }
  schedulers[scheduler_count - 1] = nBasicScheduler;
  delete[] tmp;
}

//called by the program to extract an Object from the array

SYS::Task *SYS::BasicScheduler::takeTask(byte pos)
{
  bool en = true;
  SYS::Task *out;
  SYS::Task **tmp = tasks;
  for(byte i = 0; i < task_count; i++)
  {
    if(i == pos && en)
    {
      out = tasks[i];
      i--;
      task_count--;
      en = false;
    }
    else
    {
      tasks[i] = tmp[i];
    }
  }
  delete[] tmp;
  return out;
}

SYS::BasicScheduler *SYS::BasicScheduler::takeScheduler(byte pos)
{
  bool en = true;
  SYS::BasicScheduler *out;
  SYS::BasicScheduler **tmp = schedulers;
  for(byte i = 0; i < scheduler_count; i++)
  {
    if(i == pos && en)
    {
      out = schedulers[i];
      i--;
      scheduler_count--;
      en = false;
    }
    else
    {
      schedulers[i] = tmp[i];
    }
  }
  delete[] tmp;
  return out;
}

//called by the program to replace the Object at position with another
SYS::Task *SYS::BasicScheduler::replace(byte position, Task * newTask)
{
  if(position < task_count)
  {
    SYS::Task *p = tasks[position];
    tasks[position] = newTask;
    return p;
  }
  return (Task *)0x0000;
}

SYS::BasicScheduler *SYS::BasicScheduler::replace(byte position, BasicScheduler * newBasicScheduler)
{
  if(position < scheduler_count)
  {
    SYS::BasicScheduler *p = schedulers[position];
    schedulers[position] = newBasicScheduler;
    return p;
  }
  return (SYS::BasicScheduler *)0x0000;
}

//============================================================================================

SYS::PrioScheduler::PrioScheduler(section priority)
{
  task_count = 0;
  scheduler_count = 0;
  sect = priority;
  tasks = 0x0000;
  schedulers = 0x0000;
}

SYS::Task *SYS::PrioScheduler::getTask()
{
  SYS::Task *tmp;
  for(byte i = 0; i < task_count; i++)
  {
    tmp = tasks[i];
    if(tmp->delay == 0)
      return tmp;
  }
  return (SYS::Task *)0x0000;
}

SYS::Task *SYS::PrioScheduler::getScheduler()
{
  SYS::Task *tmp;
  for(byte i = 0; i < scheduler_count; i++)
  {
    tmp = schedulers[i]->get();
    if(tmp != 0x0000)
      return tmp;
  }
  return (SYS::Task *)0x0000;
}

SYS::Task *SYS::PrioScheduler::get()
{
  SYS::Task *tmp;
  if(sect == SYS::scheduler_sect)
  {
    tmp = getScheduler();
    if(tmp != 0x0000)
      return tmp;
    tmp = getTask();
    if(tmp != 0x0000)
      return tmp;
  }
  else// if(sect == SYS::task_sect)
  {
    tmp = getTask();
    if(tmp != 0x0000)
      return tmp;
    tmp = getScheduler();
    if(tmp != 0x0000)
      return tmp;
  }
  return (SYS::Task *)0x0000;
}

//=======================================================

SYS::RoundScheduler::RoundScheduler()
{
  task_count = 0;
  scheduler_count = 0;
  sect = SYS::scheduler_sect;
  pos = 255;
  tasks = 0x0000;
  schedulers = 0x0000;
}

SYS::Task *SYS::RoundScheduler::getScheduler()
{
  if(scheduler_count)
  {
    byte lpos = pos < scheduler_count ? pos : 0;
    byte tpos = pos < scheduler_count ? pos : scheduler_count - 1;

    SYS::Task *p;
    do
    {
      if(p = schedulers[lpos]->get())
        return p;
      lpos++;
      if(!lpos < scheduler_count)
        lpos = 0;
    }
    while(tpos != lpos);
  }
  return (SYS::Task *)0x0000;
}

SYS::Task *SYS::RoundScheduler::getTask()
{
  byte lpos = pos < task_count ? pos : 0;
  byte tpos = pos < task_count ? pos : 0;
  do
  {
    if(tasks[lpos]->delay == 0)
    {
      return tasks[lpos];
    }
    lpos++;
    if(lpos >= task_count)
      lpos = 0;
  }
  while(tpos != lpos);
  return (SYS::Task*)0x0000;
}

SYS::Task *SYS::RoundScheduler::get()
{
  SYS::Task *p;
  pos++;
  if(task_count > 0 && scheduler_count > 0)
  {
    if(sect == SYS::scheduler_sect)
    {
      if(pos >= scheduler_count)
      {
        pos = 0;
        if(task_count > 0)
          sect = SYS::task_sect;
      }
    }
    else// if(sect == SYS::task_sect)
    {
      if(pos >= task_count)
      {
        pos = 0;
        if(scheduler_count > 0)
          sect = SYS::scheduler_sect;
      }
    }

    if(sect == SYS::scheduler_sect)
    {
      p = getScheduler();
    }
    else// if(sect == SYS::task_sect)
    {
      p = getTask();
    }
  }
  else if(task_count > 0)
  {
    if(pos >= task_count)
      pos = 0;
    p = getTask();
  }
  else if(scheduler_count > 0)
  {
    if(pos >= scheduler_count)
      pos = 0;
    p = getScheduler();
  }
  else
  {
    p = (SYS::Task*)0x0000;
  }
  return p;
}

//functions in SYS:

// nothing here
