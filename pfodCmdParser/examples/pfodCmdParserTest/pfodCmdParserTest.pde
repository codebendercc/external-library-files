/*
 pfod cmdParser for Arduino
 parses { cmd }
 return cmd found (first byte only)  when get }
 otherwise return 0 if no complete msg yet.
 see www.pfod.com.au  for more details.
 
 pfodCmdParser adds about 152 bytes to the program and uses about 3 bytes RAM

 (c)2012 Forward Computing and Control Pty. Ltd. 
 This code may be freely used for both private and commerical use.
 Provide this copyright is maintained.
 */
 
#include <pfodCmdParser.h>
pfodCmdParser cmdParser;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("pfodCmdParser ready, enter messages of the form");
  Serial.println(" {cmd} ");
  Serial.println(" via the serial monitor (9600baud)");
}

void loop() // run over and over
{
  byte cmd = 0;
  byte in = 0;
  if (Serial.available()) {
    in = Serial.read();
    cmd = cmdParser.parse(in);
    if (cmd != 0) {
      Serial.print("Cmd:");
      Serial.write(cmd);
      Serial.println();
      cmd = 0; // have processed this cmd now
      // so clear it and wait for next one
    } 
    // else keep looping
  }  
}


