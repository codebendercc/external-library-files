/*
 * This is the Starter sketch for Uno
 * It lets you control the board functions from an Android mobile running pfodApp
 * 
 *(c)2012 Forward Computing and Control Pty. Ltd. 
 * This code may be freely used for both private and commerical use.
 * Provide this copyright is maintained.
 */

#include <pfodParser.h>
#include <Stream.h>
pfodParser parser;

unsigned char deltaMillis;
unsigned long lastTime;

char cmdPrefix[10]; // allocate space for the cmd prefixes
int maxCmdPrefixSize = sizeof(cmdPrefix)/sizeof(cmdPrefix[0])-1; // allow one spot for the terminating null

char* completeCmd; // the cmd send by pfodApp 
char cmd = '\0'; // the current cmd char being processed
byte level = 0; // the menu level being processed.

Stream *io; // the stream to use for io  Uno uses Serial, FioV3 uses Serial1 (Serial One)

byte digitalSetting[14]; // 0 to 13, 0,1,2 not used
byte digitalPWMSetting[14]; // 0 to 254
byte digitalPWMCapable[14]; // 1 for 3,5,6,9,10,11 0 to rest

// Digital Pin settings stored in digitalSetting
const byte D_INPUT = 0; // default micro setting after reset
const byte D_INPUT_PULLUP = 1;
const byte D_OUTPUT_LOW = 2;
const byte D_OUTPUT_HIGH = 3;
const byte D_PWM = 4;

byte analogRefSetting[6]; // A0 to A5
int analogReading[6]; // A0 to A5

// Analog Ref settings stored in analogRefSetting
const byte A_DISABLED = 0; // default
const byte A_DEFAULT = 1; // 5V
const byte A_INTERNAL = 2; // 1.1V
const byte A_EXTERNAL = 3; // AREF


// the setup routine runs once, when you press reset:
void setup() {
  Serial.begin(9600);
  for (int i=3; i>0; i--) {
    Serial.print(i);
    delay(1000);
  }
  Serial.println();
  io = &Serial; // set the stream to use for io
  cmdPrefix[maxCmdPrefixSize] = '\0'; // make sure we have a null at the end
  // mark the Digital pins with PWM capabilities
  digitalPWMCapable[3] = 1; // 1 for 3,5,6,9,10,11 0 to rest
  digitalPWMCapable[5] = 1;
  digitalPWMCapable[6] = 1;
  digitalPWMCapable[9] = 1;
  digitalPWMCapable[10] = 1;
  digitalPWMCapable[11] = 1;
  // set Inputs with pullups
  for (int i=3; i<(int)(sizeof(digitalSetting)/sizeof(digitalSetting[0])); i++) {
    digitalSetting[i] = D_INPUT_PULLUP;
    setDigitalPin(i); // now set the digital pin
  }  
}

// the loop routine runs over and over again forever:
void loop() {
  byte in = 0;
  if (io->available()) {
    in = io->read();
    byte cmdFirstByte = parser.parse(in);
    if (cmdFirstByte != 0) { // have parsed a complete msg { to }
      completeCmd = (char*)parser.getCmd(); // set pointer to cmd
      level = 1; // set level
      if ('.' == cmdFirstByte) {
        getMainMenu(); // output main screen
      } else if ('d' == cmdFirstByte) {
        getDigitalMenu();
      } else if ('a' == cmdFirstByte) {
        getAnalogMenu();
      } else if ('h' == cmdFirstByte) {
        getMainHelp();
      }
    }
    cmdFirstByte = 0; // have processed this cmd now
    // so clear it and wait for next one
  }
  // else keep looping
}

/**
 * Output the main menu to the Serial connection
 */
void getMainMenu() {
  io->print(F("{." // start a pfod Menu msg
    "Uno Control Application.\n" // title of this menu
    "|d~Digital Pins" // first menu item cmd == d
    "|a~Analog Pins" // second menu item cmd == a
    "|h~Help" // third menu item cmd == h
    "}")); // close msg with }
}

/**
 * Handle the commands starting with 'd'
 */
void getDigitalMenu() {
  // strip off the 'd' and set cmd to next char
  // save 'd' in cmdPrefix for building sub-menu cmds
  if (!updateCmdCharToNextLevel()) {
    return; // error
  }
  if (cmd == '\0') { // cmd was just 'd' so return Digital Pins menu
    io->print(F("{.Digital Pins`1000")); // `1000 means pfodApp should re-request this menu every 1sec
    // the first three menu items D0,D1,D2 have commands d0 which just return {} i.e. do nothing when clicked
    io->print(F("|"));
    io->print((char*)cmdPrefix); // add prefix to cmd 
    io->print(F("0~D0 Output ")); // menu item is |d0~D0 Output ... (Rx)
    outputDigitalRead(0);
    io->print(F(" (Rx)"));
    io->print(F("|"));
    io->print((char*)cmdPrefix);
    io->print(F("0~D1 Input "));
    outputDigitalRead(1);
    io->print(F(" (Tx)"));
    io->print(F("|"));
    io->print((char*)cmdPrefix);
    io->print(F("0~D2"));
    outputDigitalSetting(2);
    io->print(F("\n(Bluetooth Led)"));
    // add menu items for pins 3 to 12
    for (int i=3; i<=12; i++) {
      io->print(F("|"));
      io->print((char*)cmdPrefix);
      char c = idxToCmdChar(i);
      io->print(c);
      io->print(F("~D"));
      io->print(i);
      if (digitalPWMCapable[i]) {
        io->print(F("*"));
      }
      outputDigitalSetting(i);
      if (digitalSetting[i] == D_PWM) {
        io->print(F("|"));
        io->print((char*)cmdPrefix);
        char c = idxToPWMCmdChar(i);
        io->print(c);
        io->print(F("~D"));
        io->print(i);
        io->print(F(" Duty Cycle `"));
        io->print(digitalPWMSetting[i]);
        io->print(F("`254~%~0.3937"));
      }
    }
    // add menu item for pin 13 with special text (Uno Led) 
    byte i = 13;
    io->print(F("|"));
    io->print((char*)cmdPrefix);
    char c = idxToCmdChar(i);
    io->print(c);
    io->print(F("~D"));
    io->print(i);
    outputDigitalSetting(i);
    io->print(F("\n(Uno Led)"));
    io->print(F("}"));
  } else {
    // next cmd char was not null so process that cmd
    if (cmd == '0') {
      returnEmptyResponse(); // do nothing for first three menu items      
    } else if (cmd >= 'a') {
      getDigitalPinSetup(cmd); // open setup for this pin
    } else if (cmd >= 'A') {
      setPWM(cmd); // process pwm slider msg 
    } else {
      // don't recognise this command just return {}
      returnEmptyResponse();
    }
  }
}

/**
 * Handle the commands starting with 'a'
 */
void getAnalogMenu() {
  if (!updateCmdCharToNextLevel()) {
    return; // error
  }
  if (cmd == '\0') {
    io->print(F("{.Analog Inputs`1000")); // get new reading every 1sec
    for (int i=0; i<=5; i++) {
      io->print(F("|"));
      io->print((char*)cmdPrefix);
      char c = idxToCmdChar(i);
      io->print(c);
      io->print(F("~A"));
      io->print(i);
      io->print(F(" "));
      outputAnalogReading(i);
    }
    io->print(F("}"));
  } else {
    // next cmd char was not null so process that cmd    
    if (cmd >= 'a') {
      getAnalogInputSetup(cmd);
    } else {
      // don't recognise this command just return {}
      returnEmptyResponse();
    }
  }
}

/**
 * Handle the commands starting with 'h'
 */
void getMainHelp() {
  if (!updateCmdCharToNextLevel()) {
    return; // error
  }
  if (cmd == '\0') {
    io->print(F("{."
      "The screens shown in this app are completely controlled by the Arduino code.\n\n"
      " The mobile's back button will navigate back to the previous menu."
      "|"));
    io->print(cmdPrefix); // add 'h' to this cmd to get hd
    io->print(F("d~Help Setting Digital Pins"
      "|"));
    io->print(cmdPrefix); // add 'h' to this cmd to get ha
    io->print(F("a~Help Setting Analog Pins"
      "}"));
  } else {

    if (cmd == 'd') {
      getDigitalSetupHelpMenu(); // show help Digital pins
    } else if (cmd == 'a') {
      getAnalogSetupHelpMenu(); // show help Digital pins
    } else {
      // don't recognise this command just return {}
      returnEmptyResponse();
    }
  }
}

/**
 * Return the help screen for Digital pins.
 * This is a menu screen with just a title and no menu items
 */
void getDigitalSetupHelpMenu() {
  if (!updateCmdCharToNextLevel()) {
    return; // error
  }
  if (cmd == '\0') {
    io->print(F("{."
      "Digital Inputs\n(except D0,D1 and D2)\n"
      "can be set to INPUT (default),\n"
      "INPUT_PULLUP (20K to 50K),\n"
      "OUTPUT HIGH or OUTPUT LOW.\n"
      "The change takes effect immeadiately.\n\n"
      "Some digital pins (3,5,6,9,10,11), marked with *, can also be set to PWM running at about 490Hz.\n"
      "In this setting a slider sets the duty cycle from 0% (always off) to 100% (always on)\n"
      "Note: D5 and D6 have slightly higher then set duty cycles and do not turn fully off."
      "}"));
  } else {
    // don't recognise this command just return {}
    returnEmptyResponse();
  }
}

/**
 * Return the help screen for Analog pins.
 * This is a menu screen with just a title and no menu items
 */
void getAnalogSetupHelpMenu() {
  if (!updateCmdCharToNextLevel()) {
    return; // error
  }
  if (cmd == '\0') {
    io->print(F("{."
      "Analog Reading range from 0 to 1023 counts\n"
      "where 1023 equals the the reference voltage\n\n"
      "On Uno, the refernce voltage can be selected between\n"
      "Disabled,\n"
      "5V (DEFAULT),\n"
      "1.1V (INTERNAL) or\n"
      "AREF pin Volts (EXTERNAL).\n"
      "There is an internal 32K resistor to Gnd on the AREF pin\n\n"
      "The 1.1V reference is not precise and can be anywhere in the range 1.0V to 1.2V"
      "}"));
  } else {
    // don't recognise this command just return {}
    returnEmptyResponse();
  }
}

/**
 * Read the pin and output the result
 * Only called for pin set as inputs
 */
void outputDigitalRead(byte idx) {
  if (digitalRead(idx) == LOW) {
    io->print(F("LOW"));
  } else {
    io->print(F("HIGH"));
  }
}

/**
 * Using the value stored in digitalSetting[]
 * output the current setup of the digital pin
 * and if it is an input read the current value and output
 * that as well.
 * This output forms part of the text for that pins menu item.
 */
void outputDigitalSetting(int idx) {
  byte b=digitalSetting[idx];
  switch (b) {
  case D_INPUT:
    io->print(F(" Input "));
    outputDigitalRead(idx);
    break;
  case D_INPUT_PULLUP:
    io->print(F(" Input "));
    outputDigitalRead(idx);
    io->print(F("\n (with pullup)"));
    break;
  case D_OUTPUT_LOW:
    io->print(F(" Output LOW"));
    break;
  case D_OUTPUT_HIGH:
    io->print(F(" Output HIGH"));
    break;
  case D_PWM:
    io->print(F(" PWM"));
    break;
  default:
    io->print(F(" unknown setting\n"));
  }
}

/**
 * Display and process the DigitalPin setup screen
 * cmds that end up here are of the form 'da' 'db' etc and 'daS', 'dbS' etc
 * The 'da' etc cmds will display the setup screen 
 * The 'daS' etc cmds return the selection made from that screen.
 * 
 */
void getDigitalPinSetup(char lastCmd) {
  byte idx = cmdCharToIdx(lastCmd);
  if (idx >= (sizeof(digitalSetting)/sizeof(digitalSetting[0]))) { // sanity check that pin number is in range 
    io->print(F("{.Error unrecognized\ndigital Pin : "));
    io->print(lastCmd);
    io->print(F("\n\n}"));
    return;
  } // else
  // strip off the 'da' 'db' etc and set cmd to next char
  // save 'da' 'db etc in cmdPrefix for building sub-menu cmds
  if (!updateCmdCharToNextLevel()) {
    return; // error
  }
  if (cmd == '\0') { // nothing after 'da', 'db' etc
    io->print(F("{?")); // start a single selection screen msg
    io->print((char*)cmdPrefix); // add the prefix that identifies this pin e.g 'dd' for pin 3
    io->print(F("S`")); // complete command e.g.  'ddS'
    io->print(digitalSetting[idx]); // current selection
    io->print(F("~Select Setting for Digital Pin D")); // screen title
    io->print(idx);
    io->print(F("|Input|Input with pullup|Output LOW|Output HIGH")); // possible choices
    if (digitalPWMCapable[idx]) {
      io->print(F("|PWM")); // add PWM choice is this is a pwm pin
    }
    io->print(F("}")); // close msg
    // the full msg looks like
    // {?ddS`0~Select Digital Pin Setting|Input|Input with pullup|Output LOW|Output HIGH}
  } else {
    // cmd was 'ddS' etc so update the pin setting with the user's choice
    if ('S' == cmd) {
      long singleSelection = 0;
      byte* argPtr = parser.getFirstArg();
      argPtr = parser.parseLong(argPtr, &singleSelection); // expects a long pointer, returns ptr to next arg
      digitalSetting[idx] = (byte)singleSelection; // save the selection
      setDigitalPin(idx); // apply the selection
    }
    returnEmptyResponse(); // return {}, pfodApp will close screen are re-request previous menu
  }
}

/**
 * Apply the user's selection for Digital Pin setting
 * This is called after saving the selection in digitalSetting[]
 */
void setDigitalPin(byte idx) {
  byte b=digitalSetting[idx]; // get the setting to apply
  switch (b) {
  case D_INPUT:
    pinMode(idx, INPUT); // set as input, no pullup
    break;
  case D_INPUT_PULLUP:
    pinMode(idx, INPUT_PULLUP); // set as input with pullup
    break;
  case D_OUTPUT_HIGH:
    digitalWrite(idx, HIGH); // make output high first to prevent Low pulse if was INPUT
    pinMode(idx, OUTPUT);
    break;
  case D_OUTPUT_LOW:
    digitalWrite(idx, LOW); // make output low first to prevent high pulse if was INPUT_PULLUP
    pinMode(idx, OUTPUT);
    break;
  case D_PWM:
    digitalWrite(idx, LOW); // make output low first to prevent high pulse if was INPUT_PULLUP
    pinMode(idx, OUTPUT);
    digitalPWMSetting[idx] = 0; // start at 0 for PWM output
    analogWrite(idx, digitalPWMSetting[idx]); // set PWM output
    break;
  default:
    io->print(F(" unknown setting\n"));
  }
}

/**
 * This method performs an AtoD conversion
 * First it sets the Reference from the idx 
 * using getRef to convert to Arduino constants
 * Then performs one reading and discards it.
 * This is to give the AtoD time to settle after the refV change
 * Then a second reading is taken and stored
 * Finally the reading is output as
 * 'reading~units~scale  where the reading is the AtoD counts
 * the units and scale depend of the ref setting 
 */
void outputAnalogReading(byte idx) {
  // set ref
  // do one reading and discard
  // output second reading
  if (analogRefSetting[idx] != A_DISABLED) {
    analogReference(getRef(idx));
    analogRead(idx);
    analogReading[idx] = analogRead(idx);
    io->print(F("`"));
    io->print(analogReading[idx]);
    outputAnalogScaling(idx);
  } else {
    io->print(F("Disabled"));
  }  
}

/**
 * Get the Arduino constant for the Ref setting in the array
 */
int getRef(byte idx) {
  if (analogRefSetting[idx] == A_DEFAULT) {
    return DEFAULT;
  } else if (analogRefSetting[idx] == A_INTERNAL) {
    return INTERNAL;
  } else if (analogRefSetting[idx] == A_EXTERNAL) {
    return EXTERNAL;
  } else {
    return DEFAULT;
  }
}

/**
 * Get the units and scaling for the Ref setting in the array
 * 1024 counts == refV 
 */
void outputAnalogScaling(byte idx) {
  if (analogRefSetting[idx] == A_INTERNAL) {
    io->print(F("~V~0.0010753"));
  } else if (analogRefSetting[idx] == A_EXTERNAL) {
    // nothing here as don't know the ref volts
    io->print(F("~ counts"));
  } else {
    // else default 5V ref
    io->print(F("~V~0.0048876"));
  }
}

/**
 * Display and process the Analog Reference Selection setup screen
 * cmds that end up here are of the form 'aa' 'ab' etc and 'aaS', 'abS' etc
 * The 'aa' etc cmds will display the setup screen 
 * The 'aaS' etc cmds return the selection made from that screen.
 * (similar to getDigitalPinSetup() above )
 */
void getAnalogInputSetup(char lastCmd) {
  byte idx = cmdToAnalogIdx(lastCmd);
  if (idx >= (sizeof(analogRefSetting)/sizeof(analogRefSetting[0]))) { // sanity check that pin number is in range 
    io->print(F("{.Error unrecognized\nAnalog Input : "));
    io->print(lastCmd);
    io->print(F("\n\n}"));
    return;
  } // else
  if (!updateCmdCharToNextLevel()) {
    return; // error
  }
  if (cmd == '\0') {
    io->print(F("{?"));
    io->print((char*)cmdPrefix);
    io->print(F("S`"));
    io->print(analogRefSetting[idx]);
    io->print(F("~Select Analog AtoD Reference Voltage for pin A"));
    io->print(idx);
    io->print(F("|DISABLED|5V (DEFAULT)|1.1V (INTERNAL)|AREF (EXTERNAL)"));
    io->print(F("}"));
  } else {
    if ('S' == cmd) {
      long singleSelection = 0;
      byte* argPtr = parser.getFirstArg();
      argPtr = parser.parseLong(argPtr, &singleSelection); // expects a long pointer, returns ptr to next arg
      analogRefSetting[idx] = (byte)singleSelection;
    }
    returnEmptyResponse();
  }
}

/**
 *  Convert cmdChar to AtoD input idx 
 */
byte cmdToAnalogIdx(char cmd) {
  byte b = (byte)(cmd-'a');
  if (b<0) {
    b = 0; // for sanity
  }
  return b;
}

/**
 * update the global cmd var with the next char in the complete command
 * also updates the cmdPrefix used for build cmd values to return to pfodApp
 * also increment and check the global level var
 */
bool updateCmdCharToNextLevel() {
  if (level > maxCmdPrefixSize) {
    io->print(F("{.Menu Nesting exceeds code storage|.~Return to main Menu}"));
    return false;
  } // else ok to copy  
  cmd = *(completeCmd+level); // cmd may be null
  strncpy((char*)cmdPrefix, completeCmd, level); // cmdPrefix is global
  *(cmdPrefix+level) = '\0'; // terminate str always
  level++;
  return true;
}

/**
 * Set the new PWM setting 
 * The cmd char indicates which digital pin is being changed
 * The fisrt arg of the message has the new value in the range
 * 0 to 254.  Use analogWrite() to make the change
 */
void setPWM(char cmd) {
  byte idx = cmdCharToIdx(cmd);
  // pick up new value from slider
  long value = 0;
  byte* argPtr = parser.getFirstArg();
  argPtr = parser.parseLong(argPtr, &value); // expects a long pointer, returns ptr to next arg
  digitalPWMSetting[idx] = (byte)value;
  analogWrite(idx, digitalPWMSetting[idx]); // update output
  returnEmptyResponse();
}

/**
 * Returns the Digial pin no corresponding to this cmd char
 * Return is never < 0 
 */
byte cmdCharToIdx(char cmd) {
  byte b;
  if (cmd >= 'a') {
    b = (byte)(cmd-'a'); // for setup menu items
  } else {
    b =(byte)(cmd-'A'); // for PWM slider menu items
  }
  if (b<0) {
    b = 0; // for sanity
  }
  return b;
}

/**
 * Turn this index into a cmd char 0=='a', 1=='b' etc
 * There cmd chars are appended to 'd' to give the cmds 'da' 'db' etc
 * When the menu item is clicked the digital pin setup screen is opened for that pin.
 * See cmdCharToIdx above
 */
char idxToCmdChar(byte idx) {
  return (char)('a'+(idx));
}

/**
 * Turn this index into a cmd char 0=='A', 1=='B' etc
 * There cmd chars are appended to 'd' to give the cmds 'dC'  etc
 * These cmds are used for the PWM sliders. 
 * When the slider is released the new value is sent with this command
 * e.g. {dC`125} and the PWM output for that pin 
 * See cmdCharToIdx above
 */
char idxToPWMCmdChar(byte idx) {
  return (char)('A'+(idx));
}

/**
 * Send back {} 
 * Every message the pfodApp sends to a pfodDevice (this micro)
 * must be responded to otherwise the pfodApp times out and 
 * drops the connection.
 * If there is nothing else to send back then just send 
 * and empty message.
 * This message is also often set if the code does not recognise
 * a command.  In this case the un-recognized command is ignored
 * and the pfodApp stays connected.
 */
void returnEmptyResponse() {
  io->print(F("{}"));
}

