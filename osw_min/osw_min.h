///////////////////////////////////////////////////////////////////////////
// osw_min.h
///////////////////////////////////////////////////////////////////////////
//
//  ########   #  ####   ######    ######  ######    #####    #####  ######
//  #   #   #  #  #   #  #     #  #        #     #  #     #  #       #
//  #   #   #  #  #   #  #     #   #####   ######   #######  #       ######
//  #   #   #  #  #   #  #     #        #  #        #     #  #       #
//  #   #   #  #  #   #  ######   ######   #        #     #   #####  ######
//  R=========E=========S=========E=========A=========R=========C=========H
//
//                        Computing the World Within
//                         Est. September 24, 1999
//
//  MindSpace Research (c) 1999,2011
//
//  Shareware:  Free software's great, but it doesn't pay the bills.
//  Feel free to share it with everyone.  But if you like this and use it
//  for everything like I do, fork over $30 to:
//
//  MindSpace Research
//  168 South 425 West
//  Bountiful, Utah, 84010
//
//  Snail mail only, maybe someday I'll get on the internet, when I'm
//  convinced it's not just a passing fad.  Thanks for your support.
//
///////////////////////////////////////////////////////////////////////////
// 990223  dwendel Initial Wapper for VxWorks to work in DOS.
// 070821  dwendel Wrapper for UNIX POSIX functions.
// 080808  dwendel Created a Safety Critical version of os_wrap.cpp/h.
// 110430  dwendel Ported to Arduino, limited space, strip to min funcs.
///////////////////////////////////////////////////////////////////////////
//
//  It started out as a VxWorks project.  As such, it used message
//  queues, semaphores, and tasks in the multi-tasking system.  But we
//  were short on embedded boards, and so this OS_WRAP was developed
//  so that it would use the VxWorks calls, but it could run as a single
//  threaded DOS program.
//
//  It made switching between single-threaded DOS and multitasking VxWorks
//  totally transparent to the developer (as long as they stayed in the
//  framework).
//
//  Development could continue with tasks, message queues and semaphores,
//  but when running in a single-threaded operating system, it would really
//  be running in a "cooperative multitasking" system.  The tasks had to
//  voluntarily give up the processor so that other tasks could run.
//
//  Now, it is ported to Arduino so the real concepts of message queues,
//  semaphores, and tasks can lay-out the archetecture as it would be in a
//  real multi-tasking system.  But it runs on Arduino as a single threaded
//  system.
//
//  osw_mini: has tasks, semaphores, counting semaphore, events, and odometer,
//            but message queues, and shared memory are implemented as
//            templates to avoid using malloc() and free().  Also, timouts of
//            semaphores and message queues are removed from this
//            implementation to save space.  (But the paramenters remain in
//            the interface, for seamless transition to the other versions of
//            os_wrap.)
//            ($20.00/seat/project shareware)
//            (tested on the Duemilanove Board.)
//
//  osw_min: has tasks, semaphores, counting semaphores, message queues,
//           events, odometer, and shared memory.  Has message queues for
//           fixed message sizes, as well as packed message queues.
//           Semaphores, and message queues have timeout values implemented.
//           Uses malloc() and free(), recommended for boards with more memory.
//           ($30.00/seat/project shareware, upgrade--pay the difference.)
//           (tested on the Duemilanove Board.)
//
//  os_wrap: The complete package, with semaphores, counting semaphores,
//           message queues, packed message queues, priority message queues,
//           (all with timeouts, and wait times for elements in the queues),
//           semTee's and message queue Tee's for multiple destinations,
//           odometer, memory management, events, tasks, syslog capabilities,
//           and an OS trace and loging capability for debug post mortums.
//           Uses malloc() and free() or os_wrap memory management, recommended
//           for boards with more memory.
//           ($50.00/seat/project shareware, upgrade--pay the difference.)
//           (tested on the Duemilanove Board.)
//
//  To detmine how many seats to pay for, or what projects need to be re-paid
//  for, use the force, you must.

#ifndef osw_min_h
#define osw_min_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <avr/pgmspace.h>  // read/write to program memory
#include <avr/eeprom.h>    // read/write to EEPROM if needed

#define OS_WRAP_VERSION "OSW_MIN_120106"
//  returns the version string of os_wrap
char* osw_version(void);

/////////////////////////////////////////////////
//  Constants that others will use.
/////////////////////////////////////////////////
#define OSW_SINGLE_THREAD (-1)
#define OSW_WAIT_FOREVER (-1)
#define OSW_NO_WAIT (0)
#define OSW_NULL (0)
#define OSW_USE_PREVIOUS_VALUE (-2)

/////////////////////////////////////////////////
//  Configure the os_wrap
/////////////////////////////////////////////////
//  OSW_STARTUP_BANNER  prints the startup banner and copyright out
//    the serial port.  If OSW_USE_SERIAL_IO is not enabled, this
//    switch has no effect.
//  OSW_USE_SERIAL_IO if the embedded system has no serial port
//    (usually for production, instead of development) then no output
//    at all will be generated.  (no error messages or info messages)
//  OSW_PRINT_ERRORS errors posted with osw_error go to the serial
//    port.  Otherwise, errors are silent.  If OSW_USE_SERIAL_IO is
//    not enabled, this switch has no effect.
//  OSW_USE_STDSTRING Use the standard lib funtions for memcpy(),
//    strlen(), strcpy(), strncpy(), strcat(), and strncat().  Otherwise,
//    the os_wrap versions are used.
//  OSW_USE_HW_WATCHDOG_ARDUINO activates and uses the watchdog
//    timer on the arduino ATmega chip.  If the watchdog timer expires,
//    only the chip gets reset, not the rest of the board.  This
//    usually leaves the board in an unusable state until power-cycle.
//  OSW_USE_IDLE_TASK Calls an idle function, the user can modify
//    the idle function, provide their own, or not use it at all
//    to save memory.
//  OSW_USE_PIN13_ARDUINO Lets the idle function blink the on
//    board led (pin 13).  If OSW_USE_IDLE_TASK is not enabled, this
//    switch has no effect.
//  OSW_USE_EVENTS Events driven programming archetecture is
//    convienient for a number of applications.  But it uses memory,
//    for Arduino--perhaps prohibitively so.  Saves memory if Events
//    are not used.
//  OSW_EVENT_USE_TASK  The event service loop will be a task.  If it is
//    not defined, a task will not be created, and the user must manually
//    call the service loop.  If OSW_USE_EVENTS is not enabled, this
//    switch has no effect.
//  OSW_USE_ODOMETER_ARDUINO Uses EEPROM memory to keep a count of
//    time the board has been on.  Updates every 6 minutes (tenth of hour).
//  OSW_ODOMETER_PRINT_6MIN  Odometer prints out it's value when it
//    increments (every 6 minutes).  If OSW_USE_ODOMETER_ARDUINO is not
//    enabled, this switch has no effect.  If OSW_USE_SERIAL_IO is not
//    enabled, this switch has no effect.
//  OSW_ODOMETER_USE_TASK  The odometer loop will be a task.  If it is
//    not defined, a task will not be created, and the user must manually
//    call the odometer loop.  If OSW_USE_ODOMETER_ARDUINO is not
//    enabled, this switch has no effect.

#define OSW_STARTUP_BANNER  // comment out if unwanted
#define OSW_USE_SERIAL_IO   // comment out if unwanted
#define OSW_PRINT_ERRORS  // comment out if unwanted
#define OSW_USE_STDSTRING // comment out if unwanted (memcpy() strlen())
//#define OSW_USE_HW_WATCHDOG_ARDUINO // comment out if unwanted
#define OSW_USE_TASKS  // comment out if unwanted.
#define OSW_USE_IDLE_TASK // comment out if unwanted.
#define   OSW_USE_PIN13_ARDUINO // comment out if unwanted
#define OSW_USE_EVENTS  // comment out if unwanted.
#define   OSW_EVENT_USE_TASK
#define OSW_USE_ODOMETER_ARDUINO  // comment out if unwanted
#define   OSW_ODOMETER_PRINT_6MIN  // comment out if unwanted
#define   OSW_ODOMETER_USE_TASK

#include<stdlib.h>  // malloc() and free()

#ifdef OSW_USE_STDSTRING
#include<string.h> //  memcpy() and strlen()
#endif

#ifdef OSW_USE_HW_WATCHDOG_ARDUINO
#include <avr/wdt.h>
#endif

/////////////////////////////////////////////////
//  os_wrap errors
/////////////////////////////////////////////////
#define OSW_OK                   (0)
#define OSW_MEM_ALLOC_FAILED     (-5001)  // failure in allocating
#define OSW_UNALLOCATED_MEM      (-5002)  // using ptr before memory allocated
#define OSW_EXCEED_MAX_SIZE      (-5003)
#define OSW_TOO_MANY_TEES        (-5004)
#define OSW_UNTEE_ERROR          (-5005)
#define OSW_MSG_Q_FULL_BUMP      (-5006)  //  Q full, old message bumped
#define OSW_MSG_Q_FULL_REJECT    (-5007)  //  Q full, new message rejected
#define OSW_PKD_Q_FULL           (-5008)  //  packed Q full
#define OSW_TOO_MANY_TASKS       (-5009)
#define OSW_TASK_NOT_FOUND       (-5010)  //  trying to delete a task twice.
#define OSW_UNDER_MIN_SIZE       (-5011)
#define OSW_INVALID_PARAMETER    (-5012)
#define OSW_TOO_MANY_EVENT_PAIRS (-5013)
#define OSW_MEM_VALID_FAIL       (-5014)
#define OSW_TIMEOUT              (-5015)
#define OSW_ALREADY_INITIALIZED  (-5016)
#define OSW_SHARED_MEM_NULL      (-5017) // if memory block was not defined
#define OSW_SHARED_MEM_BUSY      (-5018)
#define OSW_EVENT_SEND_FAIL      (-5019)
#define OSW_PRI_Q_FULL_REJECT    (-5020)
#define OSW_PRI_Q_PRI_NOT_FOUND  (-5021)
#define OSW_PRI_Q_FREE_NOT_FOUND (-5022)
#define OSW_PRI_Q_BUMP_PRI_HEAD  (-5023)
#define OSW_PRI_Q_BUMP_LOW_HEAD  (-5024)
#define OSW_PRI_Q_BUMP_LOW_TAIL  (-5025)
#define OSW_PRI_Q_TOO_MANY_PRI   (-5026)
#define OSW_GENERAL_ERROR        (-5027)

void osw_error(int _errorno);

void osw_error(char* _errstr);

/////////////////////////////////////////////////
//  Arduino Specific Functions
/////////////////////////////////////////////////

//  The memoryFree() function return how much free RAM is currently
//  available on the board.  Was taken from Arduino Cookbook, by
//  Michaael Margolis, pg 535
int memoryFree(void);


//  The printP() function prints a string that has been stored
//  in program memory (instead of RAM), using Serial.
//  The _new_line parameter implements putting a new line at the
//  end of the string or not.  Was taken from Arduino Cookbook, by
//  Michaael Margolis, pg 540
void printP(const prog_uchar* _str, bool _new_line = true);

//  Utility function to take error messages stored in program memory and
//  deliver them as strings.
char* err2str(int _error);

//  Odometer uses two 4-byte int's to double buffer the odometer value.
//  So, for example, if the OSW_ODOMETER_EEPROM_ADDRESS is 1016, the
//  odometer will use the addresses from [1016..1023].  It's up to the
//  user not to overwrite the odometer.  (There's no way to protect it,
//  you can just call dibs for it.)
#define OSW_ODOMETER_EEPROM_ADDRESS  (1016)

//  This is the number of tenths of hours that the odometer
//  has been running. If included in every sketch, it measure how long
//  the board has been used.
extern unsigned long uptime_tenths_of_hours;
extern unsigned long odometer_tenths_of_hours;
//  The odometer_main_loop can be called manually, periodically, if
//  tasks are not used.  No harm in calling it, even with it's performed
//  by a task.
void* odometer_main_loop(void* _pNotUsed = OSW_NULL);

//  Some EEPROM routines.  Can use these instead of the EEPROM.h that
//  is included with Arduino.
int eepromRead8(int _addr);
void eepromWrite8(int _addr, int _value);
int eepromRead16(int _addr);
void eepromWrite16(int _addr, int _value);
long eepromRead32(int _addr);
void eepromWrite32(int _addr, long _value);
//  Gives a hex/ascii-dump of the eeprom memory.
void xdumpEEPROM(void);
//  Writes all of EEPROM with 0xff.
void clearEEPROM(void);

/////////////////////////////////////////////////
//  memory manager.
/////////////////////////////////////////////////
typedef void* osw_mem_t;

//  These are just C function wrappers for malloc() and free().
osw_mem_t osw_malloc(int _num_bytes);
int osw_free(osw_mem_t _location);
int osw_mem_check(osw_mem_t _location);
void osw_print_mem_free(void);

/////////////////////////////////////////////////
//  Time Functions
/////////////////////////////////////////////////

//  on a DOS system the system tic is at 18.2 Hz.
//#define MS_PER_TIC (54.945054945)
//  on this system a tic is at 65.5 Hz.
//#define MS_PER_TIC (15.26)
// For Arduino there is one tic per millisec.
#define OSW_MS_PER_TIC (1)

typedef unsigned long osw_tick_t;

osw_tick_t osw_getTick(void); // ms
osw_tick_t osw_diffTime_ms(osw_tick_t _oldTime, osw_tick_t _newTime = 0);

// time to string
// expensive function increases program size by 1864 bytes
// probably because of the sprintf().
char* osw_ttoa(osw_tick_t _time);

//  A delay class to do a delay in a task.  It is a non-blocking call that
//  will return 0 if the delay has not expired, or it will return
//  OSW_SINGLE_THREAD if it is not expired on a single threaded system.
//  It will return 1 and reset itself when the time has expired.
//
//  Example snippet:
//
//  void task1(void)
//  {
//    static osw_delay delay;
//    while(!done) {
//      if (delay.Delay(3000) != 1) {
//        return OSW_SINGLE_THREAD;
//      }
//
//      // Do task1 stuff here, every 3000 ms.
//    } // endwhile.
//  }
//
//  sizeof(osw_delay) == 5
class osw_delay
{
  public:
    osw_delay();
    int delay(osw_tick_t _ms);
  private:
    osw_tick_t time;
    bool first_call;
};

//  This is a timer that stays 'engaged' for a period of time,
//  then becomes 'timedOut' when it times out.
//
//  Uses getTick() and diffTime_ms() from os_wrap.
//
//  Set the timeout value in ms.
//  then after start(), engaged() will return true until timeout
//  is reached, then it will return false.
//  stop() makes engaged() timeout immediately, returning false.
//
//
//  time--> _________________________________
//  _______|                                 |____________________
//         <---msAge()---><---msTimeLeft()--->
//         <--constructor()                  <--engaged() == 0
//         <--start()                        <--timedOut() == 1
//         <--engaged() == 1                 <--stop()
//         <--timedOut() == 0
//
//  msTimeLeft() return the amount of time (ms) before the timer
//  times out and goes to false.  msAge() returns the amount of time
//  since the constructor, or the most recent call to start().
//  msAge() continues counting even after the boolean has timed out, and
//  thus can be used as an elapsed timer.
//
//  osw_dt_timer value;
//  value.start(5000);                       //  resets timer to 5 secs
//  if (value.engaged()) printf("engaged");  //  check if value is engaged
//  else printf("timed out");
//
//  value.start();                          //  resets timer to 5 secs
//  if (value.timedOut()) printf("timed out");
//
class osw_dt_timer
{
  public:
    osw_dt_timer(osw_tick_t _msTimeout = 0);
    //  Assign one osw_dt_timer variable to another:
    osw_dt_timer(const osw_dt_timer& _source); // copy constructor
    osw_dt_timer& operator=(osw_dt_timer& _source);
    // Access functions:
    osw_tick_t getDuration(void);
    void setDuration(osw_tick_t _msTimeout);
    //  start() and stop() will clear a pause
    //  msAge() continues to count time even after a stop(), or a timedOut().
    //  stop() does NOT need to be called between successive start()'s.
    //  OSW_WAIT_FOREVER may be used as a timeout value, useful for
    //  testing.
    void start(osw_tick_t _msTimeout = OSW_USE_PREVIOUS_VALUE);
    void stop(void);
    //  engaged and timedOut are logical opposites.  Both are
    //  provided since the code is more readable one way or another.
    bool engaged(void);
    bool timedOut(void);
    //  TimeLeft and Age since the constructor, or start().
    //  If duration is OSW_WAIT_FOREVER, msTimeLeft() will return 1
    //  until stop() is called.
    osw_tick_t msTimeLeft(void);
    //  Running time count since the constructor, or start().
    osw_tick_t msAge(void);
    
  private:
    bool engaged_;
    osw_tick_t tick_;
    osw_tick_t msTimeout_;
};

/////////////////////////////////////////////////
//  interrupt enble/disable
/////////////////////////////////////////////////
void osw_interrupt_enable(void);
void osw_interrupt_disable(void);


/////////////////////////////////////////////////
//  hardware watchdog
/////////////////////////////////////////////////

//  For Arduino, the implementation is taken from  Semyon Tushev's Web Site
//  tushev.org, "Arduino and watchdog timer".
//
// valid call values are:
//  WDTO_8S    (8 secs) (ATmega 168, 328, 1280, 2560 only)
//  WDTO_4S    (4 secs) (ATmega 168, 328, 1280, 2560 only)
//  WDTO_2S    (2 secs)
//  WDTO_1S    (1 sec)
//  WDTO_500MS (500 ms)
//  WDTO_250MS (250 ms)
//  WDTO_120MS (120 ms)
//  WDTO_60MS  (60 ms)
//  WDTO_30MS  (30 ms)
//  WDTO_15MS  (15 ms)
//
//  While having a watchdog timer seems like a really good idea,
//  if/when it expires it only resets the chip not the board
//  So the serial line dies and requires a power-cycle to come back.
//  Or worst case, it bricks the board.
//  So, the watchdog timer should be the last thing added to the software
//  because it's somewhat dangerous to use it for day to day development.
void osw_watchdog_enable(int _value);
void osw_watchdog_reset(void);


/////////////////////////////////////////////////
//  A Binary Semaphore   
/////////////////////////////////////////////////
class osw_semaphore
{
  public:
    osw_semaphore();
    ~osw_semaphore();
    int semCreate(char* _name = OSW_NULL, int _full = 0);
    // Semtake will return a 1 if it got the semaphore.
    // 0 if it timed out waiting.
    // osw_SINGLE_THREAD otherwise.
    // Warning: to_wait = 0 can cause an infinite loop in the
    // calling routine if it waits for OSW_SINGLE_THREAD to return control,
    // since OSW_SINGLE_THREAD will never be returned.
    int semTake(int _to_wait_ms = OSW_WAIT_FOREVER);
    int semGive(void);
  private:
    bool sem_;
    osw_tick_t sem_tick_;
    char* name_;
};


/////////////////////////////////////////////////
//  A Counting Semaphore
/////////////////////////////////////////////////
class osw_counting_sem
{
  public:
    osw_counting_sem();
    ~osw_counting_sem();
    int semCreate(char* _name = OSW_NULL);
    // Semtake will return a 1 if it got the semaphore.
    int getCount(void);
    int semTake(int _to_wait_ms = OSW_WAIT_FOREVER);
    int semGive(void);
    void semFlush(void);
  private:
    osw_semaphore semSem_;
    int count_;
    char* name_;
};

/////////////////////////////////////////////////
//  Message Queue
/////////////////////////////////////////////////

//  The implementation of message queues between tasks.
//  The receive will receive a message or return 0
//  if the Q is empty or if an error occured.  The send will send a
//  message to the Q.  
#define OSW_BUMP_ON_FULL   (1)
#define OSW_REJECT_ON_FULL (2)

class osw_message_q
{
  public:
    osw_message_q();
    ~osw_message_q();
    int msgQcreate(char* _name, int _max_msg_size, int _max_msgs);
    // msgQreceive will return a >0 if it got a message.
    // 0 if it timed out waiting, OSW_SINGLE_THREAD otherwise.
    //  _tick will return the tick value when the element was entered
    //  into the queue.  The caller can see how long it was in the Q.
    int msgQreceive(char *_in_msg,
                    int _size = 0,
                    int _to_wait_ms = OSW_WAIT_FOREVER,
                    osw_tick_t* _tick = OSW_NULL);
    int msgQsend(char *_in_msg,
                 int _size,
                 int _control = OSW_REJECT_ON_FULL);
    int msgQcount(void) {return Qsem_.getCount();}
    void msgQflush(void);
    
  private:
    struct Qheader {
      int size;
      osw_tick_t tick;
    };
    char* name_;
    int   max_msg_size_;
    int   max_msgs_;
    char *msg_array_;
    //  at the grocery store, when you go to check out you go
    //  to the end of the line (the tail), when you get to the
    //  head of the line, you're being serviced.
    int   head_;
    int   tail_;
    osw_counting_sem Qsem_;
};

class osw_packed_q
{
  public:
    osw_packed_q();
    ~osw_packed_q();
    //  The _size_bytes, not only has to be large enough
    //  to hold the messages the user wants to pass, but also
    //  the bookkeeping that is needed by the queue, which is
    //  adding sizeof(Qheader) to each message in the queue.
    //  Note that for Arduino, sizeof(Qheader) == 6
    //  So to just pass a 2 byte int as a message the _size_bytes
    //  needs to be at least 8.
    int pkdQcreate(char* _name, int _size_bytes);
    // msgQreceive will return a >0 if it got a message.
    // OSW_TIMEOUT if it timed out waiting, OSW_SINGLE_THREAD otherwise.
    //  _tick will return the tick value when the element was entered
    //  into the queue.  The caller can see how long it was in the Q.
    int pkdQreceive(char *_buf,
                    int _buf_size = 0,
                    int _to_wait_ms = OSW_WAIT_FOREVER,
                    osw_tick_t* _tick = OSW_NULL);
    int pkdQsend(char *_in_msg,
                 int _size);
    int pkdQcount(void) {return Qsem_.getCount();}
    void pkdQflush(void);
    
  private:
    struct Qheader {
      int size;
      osw_tick_t tick;
    };
    char* name_;
    int   buf_size_;
    char* msg_array_;
    char* buf_end_;
    char* head_;
    char* tail_;
    const int ROLLOVER_VALUE;
    osw_counting_sem Qsem_;
};

/////////////////////////////////////////////////
//  Events
/////////////////////////////////////////////////

#ifdef OSW_USE_EVENTS

//  Note: this is not a limit to how many events there are
//  but it is a limit of "registrations", so for a large
//  system this number needs to be increased.
#define OSW_MAX_EVENT_PAIRS (10)
//  This is the event Q size this will determine howmany events
//  can be pending at any given time.  For simple systems, there
//  may only be 1 or 2 events pending, but size needs to be the bytes
//  reserved for the message q, about 7 bytes per event in queue.
#define OSW_EVENT_Q_DEPTH (10)

//  The "callback" function.
typedef void (*osw_eventFucnPtr) (int);

int osw_evt_register(int _event, osw_eventFucnPtr _eventHandler);
int osw_evt_publish(int _event);

//  Can be called manually.  It will run as a task if OSW_EVENT_USE_TASK
//  is defined.
void* osw_event_service_loop(void* _pNotUsed = OSW_NULL);

#endif

/////////////////////////////////////////////////
//  Shared Memory
/////////////////////////////////////////////////

//  A class used to set up a region of memory and then limit access so
//  that only one task writes or reads memory at a time.  Mostly useful
//  for interrupt handlers that need to read data and place it somewhere
//  for the application code to use.  This takes care of the flags and
//  latches so that no other task writes or reads while a task or interrupt
//  handler is writing or reading.
//
//  A pointer to the memory region needs to be provided, as well as a pointer
//  to an integer to be used as the controlling flag.  Usually the flag
//  integer is either the first or last word of the common region provided.
//  But the user is free to use something else.

class osw_shared_mem {
  public:
    osw_shared_mem(void);//:data_(OSW_NULL), flag_(OSW_NULL) {}
    int init_mem(unsigned int _max_size);
    int init_mem(unsigned char* _data, unsigned int* _flag);
    int write(void* _data, int _size);
    int read(void* _data, int _size);
    // a write call for use inside an interrupt handler.
    void int_write(void* _data, int _size);
  private:
    unsigned char* data_;
    unsigned int* flag_;
};

/////////////////////////////////////////////////
//  Tasks
/////////////////////////////////////////////////

#ifdef OSW_USE_TASKS

#define OSW_MAX_TASKS (20)  // increment as needed
// osw_funcptr is type pointer to a function.
typedef void* (*osw_funcptr) (void*);

//  Tasks.  VxWorks uses task spawn to create a new thread.
//  The DOS implementation will create a table of functions
//  and call them in order when task_go is called.

//  The evt_trace parameter to turn off events from high frequency tasks
//  that just fill the event log.  (not implemented in osw_mini)
class osw_task
{
  public:
    osw_task(void) {}
    ~osw_task() {taskDelete();}
    int taskCreate(char *_name,
                   osw_funcptr _the_func,
                   void* _data = OSW_NULL,
                   unsigned int _interval_ms = 0,  // task interval
                   bool _evt_trace = false,  // not used in osw_min
                   bool _active = true);
    int taskDelete();
    char* getName(void) {return name_;}
    unsigned int& getInterval(void) {return mod_;}
    bool& getActive(void) {return active_;}

    friend int osw_tasks_go(void);
    friend void osw_first_time_thru(void);
    friend void osw_list_tasks(void);
  private:
    char* name_;
    osw_funcptr the_func_;
    void* data_;
    unsigned int mod_;
    bool active_;
};

//  Utility library functions needed by others.

//  osw_done returns the value of the done flag.
int osw_done(void);
//  exit with an exit code.
void osw_exit(int _code = 0);

//  This returns the name of the task that is currently executing.
//  Used for things like SysLog.
char* osw_task_name(void);

//  This is the function that runs the tasks.  It is totally deterministic
//  in how it runs things.
int osw_tasks_go(void);

// Prints out the current list of tasks.
// It prints task name, task pointer, the mod value, and ACTIVE/INACTIVE state.
void osw_list_tasks(void);

#ifdef OSW_USE_IDLE_TASK
//  idle function, called while waiting for other tasks to run.
//  User implimentable.
//  Can also be used to pet watchdog timers. (usually there'd be a
//  separate task for that, recommended).
void idle_task(void);
#endif

#endif  // OSW_USE_TASKS

/////////////////////////////////////////////////
//  utility functions
/////////////////////////////////////////////////

//  Pushes the value up to the next multiple of 4.
//  (not needed for Arduino)
int align4(int _val);

void xdump(unsigned char* _buf, int _size);

//  K&R pg 62
//  reverse:  reverse string s in place.
void reverse(char s[]);
//  K&R pg 64
char* itoa(long n, int base = 10);

#ifndef OSW_USE_STDSTRING
int strlen(char* _s);
void strcpy(char* _dest, char* _source);
void strncpy(char* _dest, char* _source, int _len);
void strcat(char* _dest, char* _source);
void strncat(char* _dest, char* _source, int _len);
int memcpy(void* _dest, void* _source, long _size);
#endif

void osw_banner(void);

#endif // .h file


