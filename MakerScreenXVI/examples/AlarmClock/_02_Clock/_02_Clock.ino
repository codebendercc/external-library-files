/*
 * In this lesson, you'll convert the simple counter into 
 * a clock.
 * 
 * To do so, add in an extra variable for the AM/PM cycle and some
 * more 'if' checks in loop to keep the 12 hour 
 * cycle correct.
 */

#include "MakerScreenXVI.h"

MakerScreenXVI lcd;

// Set the starting time with these variables.
// They are set to 1:21:00 PM currently
int seconds = 20;
int minutes = 56;
int hours = 10;

/* 
 * The AMPM variable is a 'bool' type. Bool variables can only be 
 * equal to 0 or 1. When you create a bool variable, it's a 
 * helpful way to remind others (or yourself) that the variable 
 * shouldn't be updated to hold 2,3,or 4. 
 */
bool AMPM = 1; //0 is AM and 1 is PM

long timer;

void setup() {
  lcd.begin();

  lcd.backlightOn();

  timer = millis();
}

void loop() {

  if (millis() - timer >= 1000){
    seconds = seconds + 1;
    timer = timer + 1000;
  }

  if (seconds >= 60){
    seconds = seconds - 60;
    minutes = minutes + 1;
    }

  if (minutes >= 60){
    minutes = minutes - 60;
    hours = hours + 1;
    /*
     * Add in the check to swap AM/PM here when the hours tick up.
     * The change only happens once and hour,just as the hour changes.
     */
    if (hours == 12){
      /*
       * If hours just became 12, then swap AMPM.
       * Because AMPM is either 0 or 1, 1-AMPM will change it from
       * its current value to the other one. 1-1 = 0 / 1-0 = 1.
       */
      AMPM = 1 - AMPM;
    }
    // Check for when hours tries to change from
    // 12 to 13. Instead make it go from 12 to 1:
    if (hours == 13){
      hours = 1;
    }
  }

  lcd.clear();
  lcd.print(hours);
  lcd.print(":");
  
  if (minutes < 10){
    lcd.print("0");}
  lcd.print(minutes);
  lcd.print(":");
  
  if (seconds < 10){
    lcd.print("0");}
  lcd.print(seconds);

  /*
   * Include a check to print
   * AM or PM to the screen, based on the AMPM variable:
   */
  if (AMPM == 0){  //0 means 'AM'  
    lcd.print(" AM");
  }
  if (AMPM == 1){ //1 means 'PM'
    lcd.print(" PM");
  }
  
  delay(300);

}

/*
 * In this lesson, you shifted and adjusted pieces 
 * to convert the counter into a clock displaying the time.
 * 
 * In the next lesson you'll include a message alarm.
 * 
 * Try changing the 1000 in the 'if' statement that converts seconds
 * to minutes. By making it smaller, you can accelerate
 * time! 
 * Doing so can be a useful way to help check code in
 * the future, so that you don't have to set a time carefully
 * and then wait several minutes for that time to be reached.
 */

 
