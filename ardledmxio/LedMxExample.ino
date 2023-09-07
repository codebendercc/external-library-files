#include "ardledmxio.h"

// I/O pins connection
LEDMXIOCFG g_IOCfg = { 
  LMXSHIELD_WR,  // WR pin 
  LMXSHIELD_RD,  // RD pin
  LMXSHIELD_DATA,  // Data pin
  LMXSHIELD_EN, // En pin
  { LMXSHIELD_AD0, LMXSHIELD_AD1, LMXSHIELD_AD2, -1,}, // CS pins
  3,  // Number of CS pins 
  LMXSHIELD_CSTYPE
};

// Display board configuration
LEDMXCFG g_Cfg = {
  &g_IOCfg,
  3,  // Number of display board in daisy chain 
  {4, 0, 1, 3, 3, 4, 5, 6}, // display board ordering
};

LEDMXDEV g_LmxDev = {0,};

LEDMXCFG g_Cfg1 = {
  &g_IOCfg,
  1,  // Number of display board in daisy chain 
  {4, 5, 6, 7, }, // display board ordering
};
LEDMXDEV g_LmxDev1 = {0,};


const char txt[] = "This text is scrolling!";


void setup()
{
Serial.begin(9600);
while (! Serial);
/*pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);*/
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

    digitalWrite(13, 0);
   digitalWrite(12, 0);

  Serial.print("Setup\n");
// digitalWrite(12, 1);

  LedMxInit(&g_LmxDev, &g_Cfg);
  //LedMxInit(&g_LmxDev1, &g_Cfg1);
  
  Serial.println(g_LmxDev.FontLen);
  Serial.println(g_LmxDev.NbPanel);
  //Serial.println(pgm_read_byte_near(&g_LmxDev.pFont[0].Width));
  //Serial.println(pgm_read_byte_near(&g_LmxDev.pFont[1].Width));
  //Serial.println(g_LmxDev.pFont[2].Width);

//digitalWrite(12, 1);
 //LedMxPrintCenter(&g_LmxDev1, "Hello");
 LedMxPrintLeft(&g_LmxDev, "LMXDisplaySample Ver. 1.00");
 digitalWrite(12, 0);
  
  delay(5000); // Delay 5 sec
}

void DispFill(char pat)
{
  for (int i = 0; i < g_LmxDev.NbPanel; i++)
  {
    LedMxSetRam(&g_LmxDev, 0, pat, 32, g_LmxDev.PanelAddr[i]);
  }
}

// Scroll text from right to left until text runs off the display
void DispScroll(char *pText)
{
  int col = g_LmxDev.NbPanel * 32;
  int i = col;
  int pixlen;

  pixlen = LedMxPixStrLen(&g_LmxDev, pText);
  for (i = col; i + pixlen > 0; i-= 1)
  {
    LedMxPrintAt(&g_LmxDev, i, pText);
    //delay(5);
  }
}


void loop()
{
  //LedMxPrintCenter(&g_LmxDev1, "Chan 2");
  LedMxPrintCenter(&g_LmxDev, "Chan 1");
  delay(2000);
  
   // digitalWrite(13, 1);
  DispFill(0xff);
  delay(2000);
   // digitalWrite(13, 0);

  //LedMxPrintRight(&g_LmxDev1, "Right");
  LedMxPrintLeft(&g_LmxDev, "Left");
 delay(2000);
 // digitalWrite(13, 1);

  DispFill(0xff);
  delay(2000);

  //LedMxPrintLeft(&g_LmxDev1, "Left");
  LedMxPrintRight(&g_LmxDev, "Right");
  delay(2000);

  DispFill(0xff);
  delay(2000);

  //LedMxPrintCenter(&g_LmxDev1, "Center");
  LedMxPrintCenter(&g_LmxDev, "Center");
  delay(2000);

  //DispFill(0xff);
  //delay(2000);

  DispFill(0x55);
  delay(2000);

  DispFill(0xaa);
  delay(2000);

  DispFill(0);
  delay(2000);

  DispScroll((char*)txt);
  delay(2000);	
}



