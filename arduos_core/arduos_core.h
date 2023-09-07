#ifndef ARDUOS_H
#define ARDUOS_H
#include </home/alle/arduino/arduino-1.5.2/hardware/arduino/avr/cores/arduino/Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

typedef byte byte;
typedef unsigned int uint;
typedef unsigned long int ulong;
//typedef boolean bool;

#define D asm volatile("cli");

#define E asm volatile("sei");

#define I delay(1);

#define SYS_getStack() \
  asm volatile\
  (\
    "LDS R26, stack_addr     \n\t"\
    "LDS R27, stack_addr + 1 \n\t"\
    "IN  R0, __SP_L__        \n\t"\
    "ST  X+, R0              \n\t"\
    "IN  R0, __SP_H__        \n\t"\
    "ST  X+, R0              \n\t"\
  );

#define SYS_setStack() \
  asm volatile\
  (\
    "LDS R26, stack_addr     \n\t"\
    "LDS R27, stack_addr + 1 \n\t"\
    "LD  R0, X+              \n\t"\
    "OUT __SP_L__, R0        \n\t"\
    "LD  R0, X+              \n\t"\
    "OUT __SP_H__, R0        \n\t"\
  );

#define SYS_storeContext() \
  asm volatile\
  (\
  "PUSH R0            \n\t"\
  "IN   R0, __SREG__  \n\t"\
  "CLI                \n\t"\
  "PUSH R0            \n\t"\
  "PUSH R1            \n\t"\
  "CLR R1             \n\t"\
  "PUSH R2            \n\t"\
  "PUSH R3            \n\t"\
  "PUSH R4            \n\t"\
  "PUSH R5            \n\t"\
  "PUSH R6            \n\t"\
  "PUSH R7            \n\t"\
  "PUSH R8            \n\t"\
  "PUSH R9            \n\t"\
  "PUSH R10           \n\t"\
  "PUSH R11           \n\t"\
  "PUSH R12           \n\t"\
  "PUSH R13           \n\t"\
  "PUSH R14           \n\t"\
  "PUSH R15           \n\t"\
  "PUSH R16           \n\t"\
  "PUSH R17           \n\t"\
  "PUSH R18           \n\t"\
  "PUSH R19           \n\t"\
  "PUSH R20           \n\t"\
  "PUSH R21           \n\t"\
  "PUSH R22           \n\t"\
  "PUSH R23           \n\t"\
  "PUSH R24           \n\t"\
  "PUSH R25           \n\t"\
  "PUSH R26           \n\t"\
  "PUSH R27           \n\t"\
  "PUSH R28           \n\t"\
  "PUSH R29           \n\t"\
  "PUSH R30           \n\t"\
  "PUSH R31           \n\t"\
  );

#define SYS_loadContext() \
  asm volatile\
  (\
  "POP R31            \n\t"\
  "POP R30            \n\t"\
  "POP R29            \n\t"\
  "POP R28            \n\t"\
  "POP R27            \n\t"\
  "POP R26            \n\t"\
  "POP R25            \n\t"\
  "POP R24            \n\t"\
  "POP R23            \n\t"\
  "POP R22            \n\t"\
  "POP R21            \n\t"\
  "POP R20            \n\t"\
  "POP R19            \n\t"\
  "POP R18            \n\t"\
  "POP R17            \n\t"\
  "POP R16            \n\t"\
  "POP R15            \n\t"\
  "POP R14            \n\t"\
  "POP R13            \n\t"\
  "POP R12            \n\t"\
  "POP R11            \n\t"\
  "POP R10            \n\t"\
  "POP R9             \n\t"\
  "POP R8             \n\t"\
  "POP R7             \n\t"\
  "POP R6             \n\t"\
  "POP R5             \n\t"\
  "POP R4             \n\t"\
  "POP R3             \n\t"\
  "POP R2             \n\t"\
  "POP R1             \n\t"\
  "POP R0             \n\t"\
  "OUT __SREG__, R0   \n\t"\
  "POP R0             \n\t"\
  );

#define SYS_storeFunc(X) \
  asm volatile\
  (\
    "PUSH R30   \n\t"\
    "PUSH R31   \n\t"\
    ::"z" (X)\
  );

#define SYS_invoke(X) \
  asm volatile\
      (\
        "SEI           \n\t"\
        "IJMP          \n\t"\
        ::"z" (X)\
      );\

namespace SYS
{
  enum section{scheduler_sect, task_sect};

  class Task
  {
  protected:
    byte *mem;
  public:
    Task(int (*nMain)(), byte stack_cnt);
    Task(int (*nMain)(), byte stack_cnt, ulong nDelay);
    ~Task();
    void setDelay(ulong nDelay);
    int (*main)();
    void *stack;
    bool called;
    ulong delay;
  };

  //a Basic Scheduler, unable to schedule anything
  class BasicScheduler
  {
  protected:
    Task **tasks;
    byte task_count;
    BasicScheduler **schedulers;
    byte scheduler_count;
    virtual Task *getScheduler();
    virtual Task *getTask();
  public:
    BasicScheduler();
    ~BasicScheduler();
    virtual Task* get();
    void milli();
    void append(Task *);
    void append(BasicScheduler *);
    Task *takeTask(byte);
    BasicScheduler *takeScheduler(byte);
    Task *replace(byte, Task*);
    BasicScheduler *replace(byte, BasicScheduler*);
  };
  
  class RoundScheduler : public BasicScheduler
  {
  protected:
    byte pos;
    section sect;
    virtual Task *getScheduler();
    virtual Task *getTask();
  public:
    RoundScheduler();
    virtual Task *get();
  };

  //a Scheduler that implements priorities: the first in index will be executed, if available.
  class PrioScheduler : public BasicScheduler
  {
  protected:
    section sect;
    virtual Task *getScheduler();
    virtual Task *getTask();
  public:
    PrioScheduler(section priority);
    virtual Task *get();
  };

  //========static varibles========

  static Task* currentTask;
  static BasicScheduler* master;
  static void* emptyStack;
  static int (*currentFunc)();
  static bool first;
  static bool state;
  static byte cnt;
}

static void **stack_addr;

#endif // ARDUOS_H
