/*
 * In the prior lesson, you created an alarm by adding checks to the
 * clock code to make a message appear on screen when the alarm time
 * was reached. Now you'll modify that code to create a speaker alarm.
 * 
 * The speaker should be connected to pin 2 and the ground (GND)
 * line on the back of the screen. Use your component cards to see how 
 * the speaker should be connected.
 */
 
#include "MakerScreenXVI.h"

MakerScreenXVI lcd;

//Setting the time of day
int seconds = 50;
int minutes = 59;
int hours = 1;
bool AMPM = 1;

long timer;

long beepTimer; //The speaker alarm will use a timer to turn on and off

int alarmMinute = 0;
int alarmHour = 2;
bool alarmAMPM = 1;

bool alarmState = 0;

void setup() {
  lcd.begin();

  lcd.backlightOn();

  timer = millis();//set the timer variable equal to the internal counter

  /*
   * Pins on Maker Screen can be set as either inputs or outputs. 
   * Outputs receive signals from Maker Board, inputs send signals to 
   * Maker Board. A speaker is an output and is declared so with the
   * pin number it's on and the pinMode() function.
   */
  pinMode(2,OUTPUT);
}

void loop() {

  if (millis() - timer >= 1000){
    seconds = seconds + 1;
    timer = timer + 1000;
  }

  if (seconds >= 60){
    seconds = seconds - 60;
    minutes = minutes + 1;

    alarmState = 0; 
 /* 
  *  This is beepTimer's 'starting point'. Remember that millis() is an
  *  automatic counter, so you may set any variables equal to it and use
  *  the millis() timer to count how much time has passed. It's not
  *  important what the value of millis() is, only that you measure how
  *  much it has changed since you started using it and when you check it
  *  again.
  */
    beepTimer = millis();
  }

  if (minutes >= 60){
    minutes = minutes - 60;
    hours = hours + 1;

    if (hours == 12){
      AMPM = 1 - AMPM;
    }
    if (hours == 13){
      hours = 1;
    }
  }

  //If the hours, minutes, and AMPM of the time match the alarm time...   
  if ((hours == alarmHour)&&(minutes == alarmMinute)&&(AMPM == alarmAMPM)){
    alarmState = 1; //'turn on' the alarm.
  }

  if (alarmState == 0){ //if the alarm isn't on, print the time
    lcd.clear();
    lcd.print(hours);
    lcd.print(":");
    
    if (minutes < 10){
      lcd.print("0");
     }
    lcd.print(minutes);
    lcd.print(":");
    
    if (seconds < 10){
      lcd.print("0");
     }
    lcd.print(seconds);
    
    if (AMPM == 0){
      lcd.print(" AM");
    }
    if (AMPM == 1){
      lcd.print(" PM");
    }

    // Also, turn off the Tone, if it's still on
    noTone(2);
  }
  
  else{
    lcd.clear();
    lcd.print("Wake Up!");
    /*
     * When you are putting the message on the screen, you also
     * check beepTimer and turn the speaker on and off based
     * on it.
     */
    if (millis()-beepTimer < 500){ //if less than 500 ms has passed...
      /*
       * Tone takes two numbers to work. The first is the pin the
       * speaker is on, and the second is the frequency of the 
       * noise you want to make. We've selected 330 Hz.
       */
      tone(2,330);//play the alarm tone on the speaker
    }
    /*
     *'Else if' statements go after an 'if' and before 'else' statements. 
     * Think of them as alternative routes for the code to take. They 
     * are checked only if all preceeding 'if' and 'else if' statements 
     * are false. They execute if their condition is true. 
     * 
     * Once an 'else if' is run, none of the following 'else ifs' or 
     * 'else' statements in that specific chain will run.
     * 
     * This 'else if' checks if it has been less than 1000 ms since
     * the timer was reset. Because it comes after the 'if' above, it's
     * only checked when it has been more than 500ms and less
     * than 1000ms: between 1/2 and 1 second. It turns the speaker off
     * for the second 500ms.
      */
    else if (millis()-beepTimer < 1000){
      noTone(2); // noTone() stops all tones when given a pin number.
    }
    /*
     * An 'else' resets the timer when the previous two checks fail, 
     * which means it has been more than a second since the timer was 
     * reset.
     * 
     * When beepTimer = millis(), the difference between them is 0. 
     * Because millis() is always rising, the difference between them 
     * will immediately start to grow. That difference is the timer.
     */
    else{
      beepTimer = millis();
    }
  }

  delay(300);

}

/*
 * Try to make the alarm beep faster using the millis - beepTimer checks.
 * You'll use numbers other than 500 and 1000.
 * 
 * Try changing the tone of the alarm from 330 to something else. What's
 * the best alarm sound?
 * 
 * In the next lesson, you'll learn to set the time on the clock with
 * buttons so that you don't have to carefully time your code uploads.
 */

// Need help? http://www.letsstartcoding.com/help

// (c) 2016 Let's Start Coding. License: www.letsstartcoding.com/bsdlicense
 
