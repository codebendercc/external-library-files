#ifndef SM_h
#define SM_h
#include "Arduino.h"
//macro to call runstate member fuction by runstate function pointer
#define EXEC(object) ((object).*(object.Exec))()

//macro for detection af rasing edge
#define RE(signal, state) (state=(state<<1)|(signal&1)&3)==1

//macro for detection af falling edge
#define FE(signal, state) (state=(state<<1)|(signal&1)&3)==2

//save som time by reading variable directly
extern volatile unsigned long timer0_millis;

typedef void State;//statefunctions should be of this type
typedef State (*Pstate)();//pointer to statefunction


class SM{
typedef void(SM::*PRun)();//defines runstate pointer
  public:
    SM(Pstate);//constructor simple SM
    SM(Pstate, Pstate);//constructor machine with head and body state
    PRun Exec;//runstate pointer
    void Set(Pstate);//simple statechange
    void Set(Pstate, Pstate);//statechange to head and body state
    void Finish();//suspend mschine execution
    boolean Finished;//flag suspended machine
	void *data;//generic data pointer
    void Restart();//resume from suspension with last known states
    unsigned long Statetime(){//return ms since entry, inline for efficiency
      return timer0_millis - Mark;
    };//Statetime();
    boolean Timeout(unsigned long T){//returns true if Statetime > T
      return (timer0_millis - Mark)>T;
    };//Timeout()
  private:
    Pstate Head;//pointer to head state function
    Pstate Body;//pointer to body state function
    void Run(){//runstate running
      Body();
    };//Run()
    void FRun(){//runstate entry
      Mark = timer0_millis;//reset timer on entry
      Head();//call head state function
      Exec = &SM::Run;//set runstate to running
    };//FRun()
    void Idle();//runstate finished
    unsigned long Mark;//timer mark
};

#endif
