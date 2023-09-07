/*
 pfodParser for Arduino
The pfodParser parses messages of the form
 { cmd ` arg1 ` arg2 ` ... }
 or
 { cmd ~ arg1}
 or 
 { cmd }
 The args are optional
 This is a complete paser for ALL commands a pfodApp will send to a pfodDevice
 see www.pfod.com.au  for more details.
 
  pfodParser adds about 482 bytes to the program and uses about 260 bytes RAM
 
The pfodParser parses messages of the form
 { cmd ` arg1 ` arg2 ` ... }
 or
 { cmd ~ arg1}
 or 
 { cmd }
The message is parsed into the args array by replacing '|', '`' and '}' with '/0' (null)
When the the end of message } is seen
  parse() returns the first byte of the cmd
  getCmd() returns a pointer to the null terminated cmd
  getFirstArg() returns a pointer to the first arg (null terminated)
      or a pointer to null if there are no args
  getArgsCount() returns the number of args found.
These calls are valid until the start of the next msg { is parsed.
At which time they are reset to empty command and no args.

 (c)2012 Forward Computing and Control Pty. Ltd. 
 This code may be freely used for both private and commerical use.
 Provide this copyright is maintained.
 */
#include "Arduino.h"
#include "pfodParser.h"
pfodParser parser;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
    delay(1000);
  // allow a little time to connect the serialMonitor before running the rest of the setup.
  for (int i = 10; i>0; i--) {
    delay(1000);
    Serial.print(F(" "));
    Serial.print(i);
  }
  Serial.println();  
  Serial.println("pfodParser ready, enter messages via serial monitor (9600baud)");
  Serial.println(" of the form");
  Serial.println(" {cmd~arg1~arg2` ... }  OR");
  Serial.println(" {cmd`arg1`arg2` ... }");
  Serial.println("eg. {testCmd~fred~mary}");
  Serial.println("or to test parsing args as numbers start the cmd in 'n'");
  Serial.println("eg. {nTest`-55`0`33333}");
}

void loop() // run over and over
{
  byte cmd = 0;
  byte in = 0;
  if (Serial.available()) {
    in = Serial.read();
    cmd = parser.parse(in);
    if (cmd != 0) {
      Serial.println();
      Serial.print("Cmd:");
      byte* idxPtr = parser.getCmd();
      idxPtr = parser.writeToSerial(&Serial,idxPtr);
      Serial.println();
      Serial.print("number of Args:");
      Serial.println(parser.getArgsCount());
      // parse result as chars
      idxPtr = parser.getFirstArg();
      Serial.println(" as chars");
      for (int i=0;i<parser.getArgsCount(); i++) {
          idxPtr = parser.writeToSerial(&Serial,idxPtr);
          Serial.println();
      } 
      if (cmd == 'n') {
        idxPtr = parser.getFirstArg();
        Serial.println(" as numbers");
        // parse args as numbers as well
        long longResult;
        for (int i=0;i<parser.getArgsCount(); i++) {
          idxPtr = parser.parseLong(idxPtr,&longResult);
            Serial.println(longResult);
        } 
      }        
      // so clear it
      cmd = 0; // have processed this cmd now
      // so clear it and wait for next one
    } 
    // else keep looping
  }  
}




