/*
 FioV3_wifiWithSecurity
 
 this sketch is for FioV3 board with a RN wifi Xbee board, using password 173057F7A706AF9BBE65D51122A14CEE
 This sketch illustrates all the pfod msgs and how to parse the results of user inputs
 This test sketch illustrates how to use the pfodCHAP library
 to protect the pfodDevice with a 128bit password

 This example sketch reproduces the demo screens and how to parse the returns from user input
 
 (c)2013 Forward Computing and Control Pty. Ltd. 
 This code may be freely used for both private and commerical use.
 Provide this copyright is maintained.
 */

#include <pfodCHAP.h>
pfodCHAP parser;

unsigned char deltaMillis;
unsigned long lastTime;
int infoCount;
int info_Count;
long currentNo = 25; // variable for numeric input screen

int maxSizeText = 0; // set in setup below
byte currentText[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x0a, 0xe4, 0xbd, 0xa0, 0xe5, 0xa5, 0xbd, 0xe4, 0xb8, 0x96, 0xe7, 0x95, 0x8c, 0x00};
// initial text is Hello World + Hello World in Chinese

long currentSingleSelection = 1;
const int maxMultiSelections = 2;
long multiSelections[] = {-1,-1}; // max two possible selections, -1 means not selected

// the setup routine runs once when you press reset:
void setup() {
 // Open serial communications and wait for port to open:
  Serial1.begin(9600);
  Serial.begin(9600);
  // allow a little time to connect the serialMonitor before running the rest of the setup.
  // also introduces a delay on power cycles to make it infeasable to deny access by using up all the power cycles.
  for (int i = 5; i>0; i--) {
    delay(1000);
    Serial.print(F(" "));
    Serial.print(i);
  }
  Serial.println();
   // set the max number of bytes that can be sent back so it can fit in memory allocated
  maxSizeText = sizeof(currentText)-1; // allow one position on the end for the terminating null 0x00
  
  // call this BEFORE init so any debug output from init goes to the correct place
  parser.setDebugStream(&Serial); // default goes to parser.init serial connection, this optional call redirects the dubug output for FioV3 to the USB connection.
  // for FioV3 which has two serial connections Serial is the USB one used for debug output from the paser
  // There is NO debug output by default. You need to edit the C++ library code to turn on debug output, see the CPP code for details.
  // for UNO there is only one serial connection so 

  
//  parser.init(&Serial1,0, F(""));  // start with no password to just test the connection, this will work with bluetooth connections also
  parser.init(&Serial1,0, F("173057F7A706AF9BBE65D51122A14CEE"));  // start with password given in GettingStarted docs
// DO NOT use this password for real connections as it is well known. See the GettingStarted doc for a password generator

  parser.setIdleTimeout(0); // the default
  // this next setting is the idleTimeout default is 0mS i.e. never timeout
  // a non-zero setting is only used for wifi connections
  // setting this to non-zero value protects against a hacker taking over your connection and just hanging on to it
  // not sending any msgs but not releasing it, so preventing you from re-connecting.
  // when setting to non-zero you can use the pfod re-request time to ask the pfodApp to re-request a menu every so often to prevent the connection timing out
  // while the pfodApp is running. See the pfod Specification for details.
  
  Serial.println("Parser Initialized:");
  lastTime = millis();
}

/*
 * !!! This disconnect( ) method is the one you need to implement for your particular hardware wifi board
 *
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
  }
}  

int counter = 0;
int printTimer = 0;
// the loop routine runs over and over again forever:
void loop() {
  unsigned long mS = millis();
  deltaMillis = mS-lastTime;
  lastTime = mS;
  
  printTimer -= deltaMillis;
  if (printTimer<=0) {
    // note DO NOT call Serial1.print directly as it will interfere with the msg hash
    // uncommment the next two line to send a sample reading every 1sec
    //parser.print(F("Sample Reading:"));  // this is an example of raw data just sent any time outside a msg {}
    //parser.println(counter++);    
    printTimer = 1000;
  }  

  byte cmd = 0;
  cmd = parser.parse(); // do the parser stuff reading from Serial1 the wifi connection
    
  if (cmd != 0) {  // have parsed a complete msg { to }
    if (cmd == pfodCHAP::DisconnectNow) {
      // need to disconnected
      // do disconnection stuff here and then call
      disconnect(parser.getPfodAppStream());
      parser.disconnected();
      #ifdef DEBUG
      Serial.println(F("Disconnected"));
      #endif
    } else {
      // process the returns from the input screens, lists, numeric and text inputs
       byte* firstArg = parser.getFirstArg();
       switch(cmd) {
         case 'y': // return from numeric input
          // pickup the first arg which is the number 
          parser.parseLong(firstArg,&currentNo);
          break;
         case 'x': // return from text input
          // pickup the first arg which is the number 
          strncpy((char*)currentText,(const char*)firstArg,sizeof(currentText)-1); // keep null at end
          break;
         case 'S': // return from single selection input
          // pickup the first arg which is the number 
          parser.parseLong(firstArg,&currentSingleSelection);
          break;
         case 'M': // return from multi selection input
          byte* argIdx = firstArg; // pickup the first arg which is the number 
          // will be null if no arges
          int i=0;
          for (; (i<maxMultiSelections) && (*argIdx != 0); i++) {
            argIdx = parser.parseLong(argIdx,&multiSelections[i]);
            //argIdx will be null after last arg is parsed
          }
          if (i< maxMultiSelections) {
            // add a -1 to terminate the array of indices
             multiSelections[i] = -1;
          }   
          break;
       }   
       // have save the results (if any) now get the correct response
       //
       parser.print(demoMsgs(cmd)); // send the constant part of the msg
       // add the variable part for cmds r,i,s,m,a
       switch(cmd) {
          case 'a':
           parser.print(F("|.~Back to Main Screen"));  // for ASCII you can just use the F( ) macro to store the text in Program memeory (flash)
           // for UTF-8 bytes see the examples below
           // add the utf8 parts here  the utf8 byte codes are generated by UTF8converter available from www.pfod.com.au
           // each of these is "|.~Back to Main Screen" in various languages, each has the |.~ as the first 3 bytes
           // NOTE carefully
           // a) there is a null, 0x00, on the end to terminate the string
           // b) static char backChinese[] PROGMEM ,   static is very important the code does not work without it.  PROGMEM goes on the end, just before the =, and forces the bytes into flash instead of RAM 
           static char backChinese[] PROGMEM  = {0x7c, 0x2e, 0x7e, 0xe5, 0x9b, 0x9e, 0xe5, 0x88, 0xb0, 0xe4, 0xb8, 0xbb, 0xe7, 0x94, 0xbb, 0xe9, 0x9d, 0xa2, 0x00};
           parser.print((const __FlashStringHelper *)backChinese); // casting the array reference to (const __FlashStringHelper *) tell print to get the bytes out of flash instead of RAM
           static char backGreek[] PROGMEM  = {0x7c, 0x2e, 0x7e, 0xce, 0xa0, 0xce, 0xaf, 0xcf, 0x83, 0xcf, 0x89, 0x20, 0xcf, 0x83, 0xcf, 0x84, 0xce, 0xb7, 0xce, 0xbd, 0x20, 0xce, 0xba, 0xcf, 0x8d, 0xcf, 0x81, 0xce, 0xb9, 0xce, 0xb1, 0x20, 0xce, 0xbf, 0xce, 0xb8, 0xcf, 0x8c, 0xce, 0xbd, 0xce, 0xb7, 0x00};
           parser.print((const __FlashStringHelper *)backGreek); 
           static char backJapanese[] PROGMEM  = {0x7c, 0x2e, 0x7e, 0xe3, 0x83, 0xa1, 0xe3, 0x82, 0xa4, 0xe3, 0x83, 0xb3, 0xe7, 0x94, 0xbb, 0xe9, 0x9d, 0xa2, 0xe3, 0x81, 0xab, 0xe6, 0x88, 0xbb, 0xe3, 0x82, 0x8b, 0x00};
           parser.print((const __FlashStringHelper *)backJapanese); 
           static char backGerman[] PROGMEM  = {0x7c, 0x2e, 0x7e, 0x5a, 0x75, 0x72, 0xc3, 0xbc, 0x63, 0x6b, 0x20, 0x7a, 0x75, 0x72, 0x20, 0x48, 0x61, 0x75, 0x70, 0x74, 0x73, 0x65, 0x69, 0x74, 0x65, 0x20, 0x42, 0x69, 0x6c, 0x64, 0x73, 0x63, 0x68, 0x69, 0x72, 0x6d, 0x00};
           parser.print((const __FlashStringHelper *)backGerman); 
           static char backKorean[] PROGMEM  = {0x7c, 0x2e, 0x7e, 0xeb, 0xa9, 0x94, 0xec, 0x9d, 0xb8, 0x20, 0xed, 0x99, 0x94, 0xeb, 0xa9, 0xb4, 0xec, 0x9c, 0xbc, 0xeb, 0xa1, 0x9c, 0x20, 0xeb, 0x8f, 0x8c, 0xec, 0x95, 0x84, 0x00};
           parser.print((const __FlashStringHelper *)backKorean); 
           static char backRussian[] PROGMEM  = {0x7c, 0x2e, 0x7e, 0xd0, 0x9e, 0xd0, 0xb1, 0xd1, 0x80, 0xd0, 0xb0, 0xd1, 0x82, 0xd0, 0xbd, 0xd0, 0xbe, 0x20, 0xd0, 0xb2, 0x20, 0xd0, 0xb3, 0xd0, 0xbb, 0xd0, 0xb0, 0xd0, 0xb2, 0xd0, 0xbd, 0xd0, 0xbe, 0xd0, 0xb5, 0x20, 0xd0, 0xbc, 0xd0, 0xb5, 0xd0, 0xbd, 0xd1, 0x8e, 0x00};
           parser.print((const __FlashStringHelper *)backRussian); 
           // finally terminate this message
           parser.print(F("}"));
           break;  
         case 'r':
          // add the "30`currentNo`5}
          parser.print(F("30`"));
          parser.print(currentNo);
          parser.print(F("`5}"));
         break;
         case 'i':
          parser.print(maxSizeText);
          parser.print(F("~Example Text Input screen.\nThe pfodDevice controls this prompt and the text shown."
		     "\nThe pfodDevice message has set the maximum length of text to "));
          parser.print(maxSizeText);
          parser.print(F(" bytes\n"
		     "Your device can set the length upto 250 bytes\n"
		     "The Accept button sends your edited text back to the pfodDevice."
                     "|"));
          parser.print((char*)currentText);
          parser.print(F("}"));
         break;
         case 's':
           parser.print(currentSingleSelection);
           parser.print(F("~Single Selection Example Screen\n"
		"The pfodDevice determines what is displayed on this screen.  "
	    "When you press the Accept button your selection is sent back to the pfodDevice and the previous menu is displayed."
		"|Enable|Disable}"));
         break;
         case 'm':
           // add the current selections
           for (int i=0; i<maxMultiSelections; i++) {
             long idx = multiSelections[i];
             if (idx<0) {
               break;
             } // else
             parser.print(F("`"));
             parser.print(idx);
           }  
            parser.print(F("~Multi-selection Example Screen\n"
	    "The pfodDevice determines what is displayed on this screen.  "
	    "When you press the Accept button your selection is sent back to the pfodDevice and the previous menu is displayed."
		"|Fade on/off|3 Levels}"));
         break;
         default:
          ; // nothing extra
      }   
    }  
    cmd = 0; // have processed this cmd now
      // so clear it and wait for next one
    } 
  // else keep looping
} 


/**
 *  For the given cmd, c return the message to send back to pfodApp
 *  Some of these msgs will need extra variable information added.  See the switch statement above 
 */
const __FlashStringHelper* demoMsgs(byte c) {
	const __FlashStringHelper* str = F("{}"); // default return
	switch (c) {
	case '.':
		infoCount=0;
		// put main msg in input array
		str =F("{^Navigation Example Screen.\n"
		 "The particular pfodDevice you connect to will control what text and buttons are displayed.|l~Lists and Text|u~Raw Data|r~Numeric Input|a~Languages Examples|d~Demo Info}");

	break;
        case 'a':
               str = F("{.Languages Examples\n"
               " You can code your pfodDevice (Arduino or micro) to send screens in your own language (using UTF-8) and this pfodApp will displayed them, "
	       "if your mobile has the font. Here are a few examples all completely controlled by the pfodDevice. ");
        break;       
	case 'l':
		// put main msg in input array
		str =F("{.Single and Multi-selection Lists\nThis screen is an example of a menu screen.\n"
                " The message from the pfodDevice determines what menus are displayed.\n Click one of the items.|s~Single Selection List|m~Multi-selection List|i~Text Input}");
	break;
	case 'r':
		str =F("{#y~Example Number input screen.\nThe pfodDevice message has specified valid range the number to be between 5 and 30 (inclusive).\n"
		"The Accept button sends your number back to the pfodDevice.|");
	break;
	case 'i':
               str = F("{'x`");
	break;
	case 's':
		str =F("{?S`"); // rest of msg handle in main loop
	break;
	case 'm':
		str =F("{*M"); // don't include ` here as loop above will add it
	break;
	case 'u':  // this illustrates how you can send much more then 255 bytes as raw data.  
                   // The {=Raw Data Screen} just tells the pfodApp to open the rawData screen and give it a title
                   // all the rest of the text (outside the { } ) is just raw data text and can be a much as you like
                   // Note the raw data includes { } using { is OK as long as the next char is not a pfodApp msg identifer
                   // that is the following cannot appear in rawData {@ {. {: {^ {= {' {# {?  or {* 
		str =F("{=Raw Data Screen}"
		"This is the Raw Data Screen\n\n"
		"The pfodDevice can write more the 255 chars to this screen.\n"
		"Any bytes sent outside the pfod message { } start/end characters are"
		" displayed here.\n \n"
		"Since this screen can display more the 255 bytes, I will use it to tell"
		" you about some more about the pfodApp.\n \n"
		"All the screens shown here, including the first (main) screen are displays of sample messages "
		"that could be sent by a pfodDevice "
		"in the same way a web browser displays pages sent by a web server.\n"
		"But pfod messages are much simplier then web pages an much easier to code into micro-devices.\n"
		"See the Debug screen (accessed from the phone's application's menu) to see the messages that "
		"have produced these screens. "
		"Just what messages a pfodDevice sends depends on what functions it provides.\n \n"
		"This pfodApp will work with and control ANY pfodDevice with a Bluetooth or WiFi connection."
		"\nWiFi connections can be secured with a 128bit security"
		"\n \n"
		"The pfodApp also has additional application menus.\n"
		"If you press the phone's menu button, the additional menu items will be displayed:- "
		"Exit, About and Debug View\n"
		" The Debug screen shows messages that have been set to (<) pfodDevice and the messages that " 
		"have been sent from the pfodDevice to this pfodApp (>) \n"
		"As the name implies the Debug screen's primary use is to help you debug probems if the pfodDevice has "
		"not been coded correctly."
		"\n \nUse the Back Button on the mobile to go back to the previous menu."
		"\n \n");
	break;
	case 'd':
		info_Count = 0;
		infoCount++;
		if (infoCount==1) {
			str =F("{.pfodApp V1.2.\n"
			"The pfodApp is like a micro-browser that displays the micro-"
			"pages served by a pfodDevice.\n"
			"Connects via Bluetooth or Wifi.\n"
			"WiFi connections can be secured with 128bit security."
			"`1000|n}");  // note the 1000 suggests to the pfodApp to re-request this item in 1 sec
                        // the n with nothing else reserves a menu item currently empty
		} else {
			str =F("{.pfodApp V1.2.\n"
			"The pfodApp is like a micro-browser that displays the micro-"
			"pages served by a pfodDevice.\n"
			"Connects via Bluetooth or Wifi.\n"
			"WiFi connections can be secured with 128bit security."
			"`0|n~Press for next page ...}");  // next time pfodApp requests this page, add the menu and set the re-request time to 0 i.e. never				
		}
	break;
	case 'n':
		info_Count++;
		if (info_Count==1) {
			str = F( "{."
			"pfod stands for\nProtocol for Operations Discovery\n"
			" The full spec. with examples of all the messages "
			"and screens is "
			"available from \nwww.pfod.com.au\n" 
			"See the Raw Data screen for additonal info."
			"`1000|.}"); // see comments above
		} else {
			str =F("{."
			"pfod stands for\nProtocol for Operations Discovery\n" 
			" The full spec. with examples of all the messages "
			"and screens is "
			"available from \nwww.pfod.com.au\n" 
			"See the Raw Data screen for additonal info."
			"`0|.~Back to front screen}"); // see comments above
		}
	break;
	
	default:
	;// just return {}				
	}
	return str;
}



