// Fix for codebender compatibility
#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
  #define ARDUINO_ARCH_AVR
#endif

#include "MyHwATMega328.cpp"
#include "MySensorCB.h"
