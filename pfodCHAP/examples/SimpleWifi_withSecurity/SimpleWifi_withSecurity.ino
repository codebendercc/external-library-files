/*
 SimpleWifiWithSecurity for FioV3 board using RN wifi module 
 
 This test sketch illustrates how to use the pfodCHAP library
 to protect the pfodDevice with a 128bit password

 In this example, output D2 will be turned on and off from the pfodApp
 
 (c)2012 Forward Computing and Control Pty. Ltd. 
 This code may be freely used for both private and commerical use.
 Provide this copyright is maintained.
 */

// this define prints out extra info to the Arduino IDE Serial Monitor
#define DEBUG 

#include "pfodCHAP.h"  // include the library
pfodCHAP parser;  // define a parser

// give pin 2 a name:
int D2 = 2;

void setup() {
 // Open serial communications and wait for port to open:
  Serial1.begin(9600);
  Serial.begin(9600);
  // allow a little time to connect the serialMonitor before running the rest of the setup.
  for (int i = 5; i>0; i--) {
    delay(1000);
    Serial.print(F(" "));
    Serial.print(i);
  }
  Serial.println();
  
  // set D2 as an output
  pinMode(D2,OUTPUT);
 
  #ifdef DEBUG
  parser.setDebugStream(&Serial); // call this BEFORE init
  #endif
  
  // this next setting is the idleTimeout default is 60000mS i.e. 60sec
  // the protect against a hacker taking over your connection and just hanging on to it
  // not sending any msgs but not releasing it, so preventing you from re-connecting.
  //
  // set the idle timeout in mS, default is zero i.e. never times out 
  //  parser.setIdleTimeout(600000); // set to 60sec
  // it is recommended you set an idle timeout to prevent a hacker taking over your connection and keeping it open so preventing you from reconnecting.
  //
  parser.init(&Serial1,0, F(""));  // start with no password to just test the connection
    
  #ifdef DEBUG
    Serial.println("Parser Initialized:");
  #endif
}

/*
 * Need to be able to close the connection from the pfodDevice end
 * if an invalid hash is received or the idle timer times out
 *
 * This code forces the RN wifi Xbee module to drop the connection.
 *
 * @param io -- the stream that talks to the pfodApp (via the wifi)
 *  get this from parser.getPfodAppStream() 
 */
void disconnect(Stream *io) {
  // code what ever is needed to force the link to disconnect
  // typically get into cmd mode and either reboot or close the link
  // for RN wifi Xbee module need to wait >250mS send $$$ and then wait >250mS
  // then send close and exit 
  // should get EXIT back
  char EXIT[6] = {'E','X','I','T',13,10};  
  boolean foundExit = false;
  while (!foundExit) {
    delay(300);
    io->write('$');
    io->write('$');
    io->write('$');
    delay(300);
    io->print("close\r");
    delay(300);
    io->print("exit\r"); 
    // wait for EXIT
    foundExit = io->find(EXIT,6);
  };
  
}  

unsigned long lastMs;

void loop() {
  byte cmd = 0;
  unsigned long deltaMs = 0;
  unsigned long mS = millis();
  if (mS != lastMs) {
    deltaMs = mS -lastMs;
    lastMs = mS;
  }  
  
  cmd = parser.parse(); // do the parser stuff reading from Serial1 the wifi connection
  if (cmd != 0) {
  #ifdef DEBUG
     Serial.println();
  #endif

    if (cmd == pfodCHAP::DisconnectNow) {
      // need to disconnected
      // do disconnection stuff here and then call
      disconnect(parser.getPfodAppStream());
      parser.disconnected();
  #ifdef DEBUG
      Serial.println(F("Disconnected"));
  #endif
    } else {
  #ifdef DEBUG
      Serial.print(F("Cmd:'"));
      Serial.write(cmd);
      Serial.println(F("'"));
  #endif
      if (cmd == '.') {
        mainMenu(); // send back main menu
      } else if (cmd == '1') {
        // toggle output D2
       int D2_state = digitalRead(D2);
       if (D2_state == HIGH) {
  #ifdef DEBUG
         Serial.println(F("D2 HIGH -> LOW"));
  #endif
         digitalWrite(D2,LOW);
       } else {
  #ifdef DEBUG
         Serial.println(F("D2 LOW -> HIGH"));
  #endif
         digitalWrite(D2,HIGH);
       }  
       sendUpdatedState(); // send back current state
      } else {
        // don't recognize this cmd 
        parser.print(F("{}\n")); // send back empty msg
      }  
      cmd = 0; // have processed this cmd now
      // so clear it and wait for next one
    } 
    // else keep looping
  } 
}


void mainMenu(void) {
  // the main menu 
  // send a menu msg to pfodApp
  // whole msg is {.D2 Control|l~Toggle D2\n D2 is On}    or Off
    parser.print(F("{.D2 Control"));
    addState();
}     
     
void sendUpdatedState(void) {
  // after each toggle command 
  // send a menu update msg to update the state of pfodApp menu
  // whole msg is {:|l~Toggle D2\n D2 is On}    or Off
     parser.print(F("{:"));
     addState();
}     

void addState(void) {     
     parser.print(F("|1~Toggle D2\n D2 is "));
     int D2_state = digitalRead(D2);
     if (D2_state == HIGH) {
       parser.print(F("On"));
     } else {
       parser.print(F("Off"));
     }  
     parser.print(F("}"));
}  








