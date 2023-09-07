// TLS3008 based 50 RGB LEDs string test application for 16MHz Arduino
// V1.1.0 library example code
//
// - DonJuanito99 -

#include <TLS3008.h>

// Number of times to repeat each 50 LEDs frame. Usually works OK with '1', but the LEDs are only updated on the next data transmission.
// You can set the repeat count to 2 or more to immediately update the LEDs.
// Alternatively, you can keep it to '1' and uncomment the FORCEIMMEDIATEUPDATE line to force the update of the LEDs.
// You can see the results of these different settings by visually checking the timings of the LEDs...
#define FRAMERPTCNT 1
//#define FORCEIMMEDIATEUPDATE

byte RGBTable[50][3];  // RGB Values Table. Use type casting (byte*) to pass as a pointer to the sendRGBFrame function.

// Create instance of object TL3008.
// 11: Setup output pin to LEDs.
// &RGBTable: Pointer to the start of the RGB buffer.
// 150: Size of the table in bytes.
TLS3008 pixels = TLS3008(11, (byte*) &RGBTable, 150);

void setup()
{
  pixels.initCom(); // Initilize the communication with the LEDs
  randomSeed(analogRead(5)); // Get a ramdom seed to initialize the random number generator
}

void loop()
{
  byte ii;

  // Light all LED to flashy colors for 1 second ==============================================
  pixels.setPixelColor(0, RGB_RED);     // Set pixel 0 to RED
  pixels.setPixelColor(1, RGB_BLUE);    // Set pixel 1 to BLUE
  pixels.setPixelColor(2, RGB_GREEN);   // Set pixel 1 to GREEN
  pixels.setPixelColor(3, RGB_YELLOW);  // Set pixel 3 to YELLOW
  pixels.setPixelColor(4, RGB_CYAN);    // Set pixel 4 to CYAN
  pixels.setPixelColor(5, RGB_MAGENTA); // Set pixel 5 to MAGENTA
  pixels.setPixelColor(6, RGB_BLACK);   // Set pixel 6 to BLACK (OFF)
  pixels.setPixelColor(7, RGB_WHITE);   // Set pixel 7 to WHITE
  for (ii=8;ii<50;ii++)  { // Set the rest of the LEDs to random flashy colors
    RGBTable[ii][0] = (byte) (random(2)*255); // R
    RGBTable[ii][1] = (byte) (random(2)*255); // G
    RGBTable[ii][2] = (byte) (random(2)*255); // B
  }
  for(ii=0;ii<FRAMERPTCNT;ii++)
    pixels.sendRGBFrame(); // Send buffer to pixels
#ifdef FORCEIMMEDIATEUPDATE
  pixels.sendEmptyFrame(); // *** Force the update of the LEDs ***
#endif
  delay(1000);  

  // Flash all LEDs to WHITE for 100 ms =======================================================
  pixels.fillPixels(255, 255, 255); // Fill all pixels white (255, 255, 255)
  for(ii=0;ii<FRAMERPTCNT;ii++)
    pixels.sendRGBFrame(); // Send buffer to pixels
#ifdef FORCEIMMEDIATEUPDATE
  pixels.sendEmptyFrame(); // *** Force the update of the LEDs ***
#endif
  delay(100);

  // Turn off all LEDs for 500 ms =============================================================
  pixels.clearPixels(); // Clear all pixels (actually fills them with (0, 0, 0))
  for(ii=0;ii<FRAMERPTCNT;ii++)
    pixels.sendRGBFrame(); // Send buffer to pixels
#ifdef FORCEIMMEDIATEUPDATE
  pixels.sendEmptyFrame(); // *** Force the update of the LEDs ***
#endif
  delay(500);
}
