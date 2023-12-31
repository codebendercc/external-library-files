///////////////////////////////////////////////////////////////////////////
// os_wrap.h
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
//  Feel free to use and share it with everyone.  But if you like this and
//  use it for everything like I do, fork over some coin to:
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
//  but when running in a single-threaded operating system, it is really
//  running in a "cooperative multitasking" system.  The tasks have to
//  voluntarily give up the processor so that other tasks could run.
//
//  Now, it is ported to Arduino so the real concepts of message queues,
//  semaphores, and tasks can lay-out the archetecture as it would be in a
//  real multi-tasking system.  But it runs on Arduino as a single threaded
//  system.
//
//  Development note: I tried os_wrap on a Duemilanove Board, and it failed.
//    So I started stripping stuff out until I got something that worked.
//    That's when I coded up osw_min, and even that still had problems with
//    malloc and free, so then I stripped it down to osw_mini.
//
//    Then I got the Arduino Cookbook, and found out about PROGMEM, and
//    putting all my strings into program memory (especially the splash-
//    screen banner), and that solved nearly all my memory problems.
//    So now, even the full os_wrap works on the Duemilanove board, even if
//    it doesn't leave much room for anything else.  Since I had already done
//    the work of creating osw_min and osw_mini, they are available as well.
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

#ifndef os_wrap_h
#define os_wrap_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <avr/pgmspace.h>  // read/write to program memory
#include <avr/eeprom.h>    // read/write to EEPROM if needed

#define OS_WRAP_VERSION "OS_WRAP_120106"
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
//  OSW_USE_STDMEM Use the standard lib funtions for malloc() and free()
//    If this is not defined then the os_wrap memory manager is used.
//  OSW_USE_STDSTRING Use the standard lib funtions for memcpy(),
//    strlen(), strcpy(), strncpy(), strcat(), and strncat().  Otherwise,
//    the os_wrap versions are used.
//  OSW_USE_HW_INTERRUPTS_ARDUINO activates using interrupt enable/disable
//    on the Arduino.  It was noticed that using trace with disabled
//    interrupts (to simulate the atomic running of tasks), would slow
//    down the clock by as much as 400%.  So disable this if the clock
//    is more important than the atomic model of running tasks.
//  OSW_USE_HW_WATCHDOG_ARDUINO activates and uses the watchdog
//    timer on the arduino ATmega chip.  If the watchdog timer expires,
//    only the chip gets reset, not the rest of the board.  This
//    usually leaves the board in an unusable state until power-cycle.
//  OSW_SAFETY_CRITICAL_PARADIGM, For safety critical systems, no code
//    is executed in a different order from run to run.  No tasks are
//    run "flat-out".  The task loop is triggered by a time tick.  The
//    tasks are executed with interrupts disabled.  So however long that
//    loop takes to execute is added to the interrupt latency.  This is
//    for the paradigm that tasks should be "atomic" functions.  Turning
//    off this switch will allow tasks to be run "flat-out" and interrupts
//    will happen anyplace in the execution of the code. 
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
//  OSW_USE_TRACE provides a real-time trace of the sytem as it's running
//    and stores it into EEPROM.  Writing to EEPROM happens at about
//    a 2400 baud rate.  So it will seriously slow down the application.
//    Plus, since there only 1K of EEPROM, it fills up and is overwritten
//    frequenly, every 1 to 10 seconds, depending on the application.  So
//    it's designed to capture the last things that os_wrap did before a
//    crash.  (If using the odometer, care must be taken to not overwrite
//    the odometer values.)
//  OSW_TRACE_DUMP_PREV_RUN enabling this switch will dump the previous
//    run's EEPROM trace at the begining of this run.  Useful for
//    debugging.  If OSW_USE_TRACE is not enabled, this switch has no
//    effect.  If OSW_USE_SERIAL_IO is not enabled, this switch has no
//    effect.
//  OSW_TRACE_TIME and
//  OSW_TRACE_TICK  choices for having the tick of the trace line or the
//    time or both or neither.  tick uses getTick, time uses Clock() from
//    stdtime.  Note:trace_tick uses fewer characters and so is smaller
//    and faster. If OSW_USE_TRACE is not enabled, this switch has no
//    effect.
//  OSW_TRACE_ECHO determines if the trace gets echo-ed to the serial port.
//    If OSW_USE_SERIAL_IO is not enabled, this switch has no effect.
//    If OSW_USE_TRACE is not enabled, this switch has no effect.


#define OSW_STARTUP_BANNER  // comment out if unwanted
#define OSW_USE_SERIAL_IO   // comment out if unwanted
#define OSW_PRINT_ERRORS  // comment out if unwanted
#define OSW_USE_STDMEM // comment out if unwanted (malloc() free())
#define OSW_USE_STDSTRING // comment out if unwanted (memcpy() strlen())
#define OSW_USE_INTERRUPTS_ARDUINO // comment out if unwanted
//#define OSW_USE_HW_WATCHDOG_ARDUINO // comment out if unwanted
//#define OSW_SAFETY_CRITICAL_PARADIGM // commend out if unwanted
#define OSW_USE_IDLE_TASK // comment out if unwanted.
#define   OSW_USE_PIN13_ARDUINO // comment out if unwanted
#define OSW_USE_EVENTS  // comment out if unwanted.
#define   OSW_EVENT_USE_TASK  // comment out if unwanted
#define OSW_USE_ODOMETER_ARDUINO  // comment out if unwanted
#define   OSW_ODOMETER_PRINT_6MIN  // comment out if unwanted
#define   OSW_ODOMETER_USE_TASK  // comment out if unwanted
//#define OSW_USE_TRACE  // comment out if unwanted
//#define   OSW_TRACE_DUMP_PREV_RUN // comment out if unwanted.
//#define   OSW_TRACE_TIME // comment out if unwanted
//#define   OSW_TRACE_TICK // comment out if unwanted
//#define   OSW_TRACE_ECHO // comment out if unwanted

#define OSW_MAX_TEES (2)

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


//  Utility function to take error messages stored in program memory and
//  deliver them as strings.  (Arduino)
char* err2str(int _errorno);

/////////////////////////////////////////////////
//  Arduino Specific Functions
/////////////////////////////////////////////////

//  The memoryFree() function return how much free RAM is currently
//  available on the board.  Was taken from Arduino Cookbook, by
//  Michael Margolis, pg 535
int memoryFree(void);


//  The printP() function prints a string that has been stored
//  in program memory (instead of RAM), using Serial.
//  The _new_line parameter implements putting a new line at the
//  end of the string or not.  Was taken from Arduino Cookbook, by
//  Michael Margolis, pg 540
void printP(const prog_uchar* _str, bool _new_line = true);

//  Odometer uses two 4-byte int's to double buffer the odometer value.
//  So, for example, if the OSW_ODOMETER_EEPROM_ADDRESS is 1016, the
//  odometer will use the addresses from [1016..1023].  It's up to the
//  user not to overwrite the odometer.  (There's no way to protect it,
//  you can just call dibs for it.)
#define OSW_ODOMETER_EEPROM_ADDRESS  (1016)

//  The tenths of hours that the odometer and uptime has been
//  running. If included in every sketch, it measures how long
//  the board has been used.  uptime measures how long the board's
//  been up since it's last boot.
extern unsigned long uptime_tenths_of_hours;
extern unsigned long odometer_tenths_of_hours;
//  The odometer_main_loop can be called manually, periodically, if
//  tasks are not used.  No harm in calling it manually, even if it's being
//  called periodically in it's task.
void* odometer_main_loop(void* _pNotUsed = OSW_NULL);

//  Some EEPROM routines.
//  I prefer these instead of the EEPROM.h that is included with Arduino.
int eepromRead8(int _addr);
void eepromWrite8(int _addr, int _value);
int eepromRead16(int _addr);
void eepromWrite16(int _addr, int _value);
long eepromRead32(int _addr);
void eepromWrite32(int _addr, long _value);
//  Gives a hex/ascii-dump of the eeprom memory.
void xdumpEEPROM(void);
//  Writes all of EEPROM with 0xff. (as it comes in new boards)
void clearEEPROM(void);

/////////////////////////////////////////////////
//  memory manager.
//  based on the K&R example pg 185-189
/////////////////////////////////////////////////
#ifdef OSW_USE_STDMEM
#include<stdlib.h>  // malloc() and free()
#else
#define OSW_HEAP_SIZE (450)
#endif

typedef void* osw_mem_t;

//  if OSW_USE_STDMEM is set then these function just use
//  the library malloc and free.  If it's not set then these
//  functions use the osw_memory class patterned after the
//  K&R example.  It then grabs a block of memory of size
//  OSW_HEAP_SIZE and manages that block.
osw_mem_t osw_malloc(int _num_bytes);
int osw_free(osw_mem_t _location);

//  if using std malloc and free, this function does nothing.
//  if using os_wrap memory manager, this function checks the
//  bytes before and after the allocated memory to see if there
//  have been overruns.
int osw_mem_check(osw_mem_t _location);

//  if using std malloc and free, this function does nothing.
//  if using os_wrap memory manager, this will print out the
//  free-list.  It gives an indication of how fragmented the
//  memory is, and what the largest chuncks of free memory are.
void osw_print_mem_free(void);

/////////////////////////////////////////////////
//  Time Functions
/////////////////////////////////////////////////

//  on a DOS system (windows 3.1) the system tic is at 18.2 Hz.
//#define OSW_MS_PER_TIC (54.945054945)
//  on this system (windows XP) a tic is at 65.5 Hz.
//#define OSW_MS_PER_TIC (15.26)
// For Arduino there is one tic per millisec.
#define OSW_MS_PER_TIC (1)

typedef unsigned long osw_tick_t;

//  returns the system tick, for Arduino, it's just milisecs since boot.
osw_tick_t osw_getTick(void); // ms

//  given two system tick values, finds how many ms have transpired between
//  them.  If the _newTime is zero, then this function return time in
//  ms from _oldTime to current time.
osw_tick_t osw_diffTime_ms(osw_tick_t _oldTime, osw_tick_t _newTime = 0);

// time to string
//  creats a string of the form:
//  0D:00:00:00.000  which is
//  xD:hh:mm:ss.ms, days, hours, minutes, seconds, milisecs since boot.
// This is a very expensive function, increases program size by 1864 bytes
// because of the sprintf().
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
    osw_dt_timer(int _msTimeout = 0);
    //  Assign one osw_dt_timer variable to another:
    osw_dt_timer(const osw_dt_timer& _source); // copy constructor
    osw_dt_timer& operator=(osw_dt_timer& _source);
    // Access functions:
    int getDuration(void);
    void setDuration(int _msTimeout);
    bool isPaused(void);
    //  start() and stop() will clear a pause
    //  msAge() continues to count time even after a stop().
    //  stop() does NOT need to be called between successive start()'s.
    //  OSW_WAIT_FOREVER may be used as a timeout value, useful for
    //  testing.
    void start(int _msTimeout = OSW_USE_PREVIOUS_VALUE);
    void stop(void);
    //  pause() and resume() will pause the passage of time.
    //  msTimeLeft() and msAge() will freeze until unpaused.
    //  engaged() and timedOut() will be unchanged during pause.
    void pause(void);
    void resume(void);
    //  engaged and timedOut are logical opposites.  Both are
    //  provided since the code is more readable one way or another.
    bool engaged(void);
    bool timedOut(void);
    //  TimeLeft and Age since the constructor, or start().
    //  If duration is OSW_WAIT_FOREVER, msTimeLeft() will return 1
    //  until stop() is called.
    int msTimeLeft(void);
    //  Running time count since the constructor, or start().
    int msAge(void);
    
  private:
    void clearPause(void); // internal use only, users must use resume()
    bool engaged_;
    osw_tick_t tick_;
    int msTimeout_;
    osw_tick_t tickAtPause_;
};

/////////////////////////////////////////////////
//  interrupt enble/disable
/////////////////////////////////////////////////

//  Just a wrapper for Arduino's interrupts() and noInterrupts().
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
    int semTee(osw_semaphore &tee_from);
    int semUntee(osw_semaphore &tee_from);
  private:
    bool sem_;
    osw_tick_t sem_tick_;
    char* name_;
    osw_semaphore* the_list_[OSW_MAX_TEES];
    int semSend(osw_semaphore* _node);
};


/////////////////////////////////////////////////
//  A Counting Semaphore
/////////////////////////////////////////////////
class osw_counting_sem
{
  public:
    osw_counting_sem(void);
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
//  message to the Q.  If the Q is full it will overwrite the oldest
//  message and return 0.
//
//  Nope: what follows is ugly to implement and get the
//  sequence correct the msgQ needs to be created before it can be
//  tee-ed from.  This really makes it untenable, and a separate
//  event handler needs to be developed:
//
//  A publish/subscribe functionality is provided through the
//  message Q mechanism.  By creating a message Q with max_msgs set to 0
//  the message will dead end.  But any other Q tee-ing off of this
//  Q will receive it.  So events can be generated, subscription is
//  accomplished by tee-ing off the Q, and publish is accomplished
//  by the message Q send.
#define OSW_BUMP_ON_FULL   (1)
#define OSW_REJECT_ON_FULL (2)

class osw_message_q
{
  public:
    osw_message_q();
    ~osw_message_q();
    int msgQcreate(char* _name, int _max_msg_size, int _max_msgs);
    int msgQtee(osw_message_q &_tee_from);
    int msgQuntee(osw_message_q &_tee_from);
    // msgQreceive will return a >0 if it got a message.
    // 0 if it timed out waiting, OSW_SINGLE_THREAD otherwise.
    //  _tick will return the tick value when the element was entered
    //  into the queue.  The caller can see how long it was in the Q.
    int msgQreceive(char *_in_msg,
                    int _size = 0,
                    int _to_wait_ms = OSW_WAIT_FOREVER,
                    int* _tick = OSW_NULL);
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
    osw_message_q *the_list_[OSW_MAX_TEES];
    int Qsend(osw_message_q *node, char *in_msg, int size, int _control);
};

class osw_packed_q
{
  public:
    osw_packed_q();
    ~osw_packed_q();
    int pkdQcreate(char* _name, int _size_bytes);
    int pkdQtee(osw_packed_q &_tee_from);
    int pkdQuntee(osw_packed_q &_tee_from);
    // msgQreceive will return a >0 if it got a message.
    // OSW_TIMEOUT if it timed out waiting, OSW_SINGLE_THREAD otherwise.
    //  _tick will return the tick value when the element was entered
    //  into the queue.  The caller can see how long it was in the Q.
    int pkdQreceive(char *_buf,
                    int _buf_size = 0,
                    int _to_wait_ms = OSW_WAIT_FOREVER,
                    int* _tick = OSW_NULL);
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
    osw_packed_q *the_list_[OSW_MAX_TEES];
    int Qsend(osw_packed_q *node, char *in_msg, int size);
};

/////////////////////////////////////////////////////////////////////////
//  This is the osw_priority_q.  It uses time and a counting
//  semaphore from the os_wrap routines.  It provides a tick to see how
//  long a message was in the queue before it was retrieved.
//
//  This uses fixed length arrays, and if a queue fills up then there is
//  the bump logic explained above.
/////////////////////////////////////////////////////////////////////////

//  Priorities: biggest number have the highest priority.
//  Usually 0 is the lowest priority, and no upper limit to highest
//  priority.  If negative priorities are needed, just ensure that
//  the value of OSW_ALL_PRIOIRITIES is different than a needed priority.
#define OSW_ALL_PRIOIRITIES (-1)

//  The priority queue is a single array to hold all the data.  Then
//  for every priority there is a linked list of data inside the data array.
//  There is also a free list that keeps track of the memory as it's read
//  out.  The bookkeeping to hold the data contains heads and tails of the
//  data for each priority.  That is held in another array and it's size
//  is determined by the number of different priorities that is being used.
//
//  If n is the number of elements in the Q, and p is the number of different
//  priorities that are used, then store and retrive is O(log(p+1)).  Note that
//  it's based on p not n.  For most routing applications p is only 3 or 4
//  and so for that few of priorities, or if p is a fixed number, this
//  algorithm is ~= O(constant).
//
//  When the Q fills up:
//  OSW_PRIQ_BUMP_LOW_HEAD_FULL finds the lowest priority queue, and removes
//    the head of the queue (oldest) and uses the space for the new element.
//    The priQsend() function will return the value: OSW_PRI_Q_BUMP_LOW_HEAD.
//  OSW_PRIQ_BUMP_LOW_TAIL_FULL find the lowest priority queue, and removes
//    the tail of the queue.  This is useful for some network implementations.
//    If the network protocol calls for retransmission after a missed
//    message, bumping off the end of the queue means more sequenced messages
//    get through reducing the number of messages that need to be retransmitted.
//    The priQsend() function will return the value: OSW_PRI_Q_BUMP_LOW_TAIL.
//  OSW_PRIQ_BUMP_PRI_HEAD_FULL bumps the head (oldest) message of the same
//    priority as the new message, if there are no previous messages at that
//    priority, the new message is rejected. The priQsend() function will
//    return the value: OSW_PRI_Q_BUMP_PRI_HEAD.
//  OSW_PRIQ_REJECT_ON_FULL if the queue is full, reject the new message
//    regardless of its priority.  The priQsend() function will return the
//    value: OSW_PRI_Q_FULL_REJECT.
#define OSW_PRIQ_BUMP_LOW_HEAD_FULL   (1)
#define OSW_PRIQ_BUMP_LOW_TAIL_FULL   (2)
#define OSW_PRIQ_BUMP_PRI_HEAD_FULL   (3)
#define OSW_PRIQ_REJECT_ON_FULL       (4)

class osw_priority_q
{
  public:
    osw_priority_q();
    ~osw_priority_q();
    int priQcreate(char* _name,
                   int _max_msg_size,
                   int _max_msgs,
                   int _max_num_priorities);
    // msgQreceive will return a >0 if it got a message.
    // 0 if it timed out waiting, OSW_SINGLE_THREAD otherwise.
    //  _tick will return the tick value when the element was entered
    //  into the queue.  The caller can see how long it was in the Q.
    int priQreceive(int* _priority,
                    char* _in_msg,
                    int _size, // sizeof _in_msg array
                    int _to_wait_ms = OSW_WAIT_FOREVER,
                    osw_tick_t* _tick = OSW_NULL);
    int priQsend(int _priority,  // 0=lowest priority
                 char* _in_msg,
                 int _size,
                 int _control = OSW_PRIQ_REJECT_ON_FULL);
    int priQcount(int _priority = OSW_ALL_PRIOIRITIES);
    void priQflush(int _priority = OSW_ALL_PRIOIRITIES);
  private:
    char* name_;
    int max_msg_size_;
    int max_msgs_;
    int max_num_priorities_;
    bool initialized_;
    osw_counting_sem Qsem_;

    struct Qheader {
      int size;
      osw_tick_t tick;
      int prev;
      int next;
    };
    //ckd_array< ckd_array<int, 16>, 10 > msgs_;
    char* msgs_;
    int msgs_used_;
    int& get_hdr_size(int _idx);
    osw_tick_t& get_hdr_tick(int _idx);
    int& get_hdr_prev(int _idx);
    int& get_hdr_next(int _idx);
    char* get_msg_data(int _idx);

    struct priListStruct {
      int priority;
      int count;
      int head;
      int tail;
      priListStruct():priority(-1), count(0), head(-1), tail(-1) {}
    };
    //ckd_array<priListStruct, 10> priList_;
    priListStruct* priList_;
    int priCount_;

    int get_highest_pri(void);
    int get_lowest_pri(void);
    int get_pri_struct_idx(int _priority);
    int push_pri_struct(int _data_idx, int _priority);
    int/*data_idx*/ pop_pri_head(int _priority);
    int/*data_idx*/ pop_pri_tail(int _priority);
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
//  may only be 1 or 2 events pending.
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
//  A pointer to the memory region needs to be provided, as well as a pointer
//  to an integer to be used as the controlling flag.  Usually the flag
//  integer is either the first or last word of the common region provided.
//  But the user is free to use something else.

class osw_shared_mem {
  public:
    osw_shared_mem(void);//:data_(OSW_NULL), flag_(OSW_NULL) {}
    //int init_mem(unsigned int _max_size);
    int init_mem(unsigned char* _data, unsigned int* _flag);
    int write(void* _data, int _size, bool _trace = false);
    int read(void* _data, int _size, bool _trace = false);
    // a write call for use inside an interrupt handler.
    void int_write(void* _data, int _size);
  private:
    unsigned char* data_;
    unsigned int* flag_;
};

/////////////////////////////////////////////////
//  Tasks
/////////////////////////////////////////////////

#define OSW_MAX_TASKS (20)  // increment as needed
// osw_funcptr is type pointer to a function.
typedef void* (*osw_funcptr) (void*);

//  Tasks.  VxWorks uses taskSpawn to create a new thread.
//  The single threaded implementation will create a table of functions
//  and call them in order when task_go is called.

//  The _osw_trace parameter to turn off tracing in high frequency tasks
//  that just fill the event log.
class osw_task
{
  public:
    osw_task(void) {}
    ~osw_task() {taskDelete();}
    int taskCreate(char *_name,
                   osw_funcptr _the_func,
                   void* _data = OSW_NULL,
                   unsigned int _interval_ms = 0,  // task interval
                   bool _osw_trace = true,
                   bool _active = true);
    int taskDelete();
    char* getName(void) {return name_;}
    int getTid(void) {return tid_;}
    unsigned int& getInterval(void) {return mod_;}
    bool& getTrace(void) {return osw_trace_;}
    bool& getActive(void) {return active_;}

    friend int osw_tasks_go(void);
    friend void osw_first_time_thru(void);
    friend void osw_list_tasks(void);
  private:
    char* name_;
    osw_funcptr the_func_;
    void* data_;
    unsigned int mod_;
    int tid_;
    bool osw_trace_;
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
void osw_idle_task(void);
#endif

/////////////////////////////////////////////////
//  Trace
/////////////////////////////////////////////////

//  Traceing writes a symbolized log to EEPROM, that allows for
//  a post-mortum to see what events traspired prior to the
//  end of the last run.
//  A sample run may look like:
//
//  0 T0 Tc 5Hz
//  121 T0 Tc 1sec
//  278 T0 Tc 5sec
//  2415 osw_tasks_go
//  2809 T1 Mget80
//  2809 T1 Qc osw_eventQ
//
//  The first column is the time-tick.
//  The next column is T<xxx>, where xxx is the task_idx.  Note that before
//    tasks start running, it is a zero.  There can also exist a task 0
//    so they can only be distinguished by context.
//  The third column is the action that has been preformed.  Along with
//    any extra information such as strings of values.
//
//  These are the abreviations:
//  T<tid>           Task and task id (tid)
//  Tc <taskname>    Task create and name
//  Td               Task delete
//  Qc <qname>       Message Q create and name
//  Qt <qname>       Message Q tee and name
//  Qut <qname>      Message Q un-tee and name
//  QR <numbytes>    Message Q receive and number of bytes
//  QS <numbytes>    Message Q send and number of bytes
//  QF               Message Q flush
//  SW <numbytes>    Shared memory write and number of bytes
//  SR <numbytes>    Shared memory read and number of bytes
//  Sc <semname>     Semaphore create and name
//  ST               Semaphore take
//  SG               Semaphore give
//  St <semname>     Semaphore tee and name
//  Sut <semname>    Semaphore un-tee and name
//  SCc <semname>    Counting semaphore create and name
//  SCT <semcount>   Counting semaphore take and the count
//  SCG <semcount>   Counting semaphore give and the count
//  SCF              Counting semaphore flush
//  EVP <evtnumber>  Event publish and number (event id)
//  EVR <evtnumber>  Event register and number
//  Mget <numbytes>  Memory malloc and number of bytes
//  Mfree0x <ptr>    Memory free call for memory pointer
//  Timeout          Timer timeout.
//  exit             Exit and quit.
//
//  User defined strings can be "injected" into the log.

//  Start and end of EEPROM memory to use for the trace.
//  Does not include end byte.  [start..end)
#define OSW_TRACE_EEPROM_START (0)
#define OSW_TRACE_EEPROM_END (OSW_ODOMETER_EEPROM_ADDRESS) //avoid the odometer

//  The user can inject a string into the osw trace.
void osw_trace_inject(char* _str);
void osw_trace_on(void);
void osw_trace_off(void);
//  Dumps the memory to the serial port.
void osw_dump_EEPROM_trace(void);
//  Flushes the trace buffers (if they exist) (i.e. to a file).
void osw_trace_flush(void);

/////////////////////////////////////////////////
//  Logging service.
/////////////////////////////////////////////////

//  for Arduino this is reduced down to serialIO.
class osw_log {
  public:
    osw_log();
    ~osw_log();
    // echo to serial port
    int init(bool _echo = true);
    // write to eeprom (specify address and size)
    // careful not to overwrite odometer.
    int init_eeprom(void* _address, long _size);
    // log.
    void logc(char _c);
    void logs(char* _s);
    void logi(long _n);
    void logx(long _n);
    void flush(void) {}
    //  write the memory block the serial port. (post processing).
    void mem2file(void);
    void dumpLog(void);
  private:
    //  variables for eeprom
    char* base_;
    char* curptr_; // point to current location in mem block
    int size_;
    char* end_;
    //  variable for serialIO
    bool echo_;
};

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


