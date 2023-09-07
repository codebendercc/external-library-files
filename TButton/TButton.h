/* Toggle Button */

#ifndef TButton_h
#define TButton_h

#include <WProgram.h>

class TButton
{
  public:
    TButton(int pin);
    bool isActive(void);
    void delay(int msecs);

    int bounce_delay;
    int check_interval;

  private:
    int pin;
	bool active_state;
	int old_value;
};

#endif
