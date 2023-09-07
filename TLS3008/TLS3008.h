#ifndef TLS3008_H
#define TLS3008_H

// TLS3008 library Version 1.1.0
 
#if (ARDUINO >= 100)
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#define MAXFRAMESBEFORESYNC 20

#define RGB_RED 255, 0, 0
#define RGB_GREEN 0, 255, 0
#define RGB_BLUE 0, 0, 255
#define RGB_YELLOW 255, 255, 0
#define RGB_MAGENTA 255, 0, 255
#define RGB_CYAN 0, 255, 255
#define RGB_WHITE 255, 255, 255
#define RGB_BLACK 0, 0, 0

class TLS3008 {
public:
		// Constructor
		//  bPin is the pin number connected to the LEDs
		//  ptRGBTable is the byte pointer (byte*) to the RGB buffer that should have been reserved by the user application.
		//   This buffer size should be a multiple of 3. It is organized as bTable[NUMLEDS][3].
		//   uiRGBTableSize is the size of the RGB buffer. This value must be consistent with the real size of the buffer.
        TLS3008( byte bPin, byte* ptRGBTable, unsigned int uiRGBTableSize );
//        ~TLS3008(); // Destructor
		// setOutputPin: Call this to setup the pin used for serial LED output. bPin is the pin number.
		void setOutputPin( byte bPin );
		// initCom: This MUST be called to initialize the communication with the LED string. Usually, this is done in the setup() function.
		void initCom();
		// sendEmptyFrame: Call this to send an empty DATA frame.
		void sendEmptyFrame(); // Private?
		// sendRGBFrame: Call this to send a DATA frame.
		//  ptRGB is a pointer to a 'byte' array containing RGB data. Format of array is byte R, byte G, byte B.
		//  uiTableSize is the size in bytes of the array.
		//  NOTE: The LEDs are only updated after resending the data or sending another data frame !
		//        Check the example for more information.
		void sendRGBFrame();
		// setPixelColor: Set the RGB data for a single LED.
		//  pixel is the LED number to be set. Numbering begins at 0. THE VALUE IS NOT CHECKED BY THE CODE, so if you set an illegal value, RAM corruption WILL occur!
		//  bR, bG and bB are respectively the RED, GREEN and BLUE values.
		//  NOTE: This function only changes the buffered values. The data is not automatically sent to the LEDs...
		void setPixelColor(unsigned int pixel, byte bR, byte bG, byte bB);
		// fillPixels: Sets the RGB data for all LEDs.
		//  bR, bG and bB are respectively the RED, GREEN and BLUE values.
		//  NOTE: This function only changes the buffered values. The data is not automatically sent to the LEDs...
		void fillPixels(byte bR, byte bG, byte bB);
		// clearPixels: Sets all the LEDs to BLACK (OFF).
		//  NOTE: This function only changes the buffered values. The data is not automatically sent to the LEDs...
		void clearPixels();

		// setPulseLength: Manchester half period length in microseconds
		void setPulseLength( byte bPulse );
		// setEOSyncDelay: Call this to change the default delay after sending a SYNC frame. Delay is in milliseconds.
		void setEOSyncDelay( byte bEOSD );
		// setEOResetDelay: Call this to change the default delay after sending a RESET frame. Delay is in milliseconds.
		void setEOResetDelay( byte bEOR );
		// setEOEmptyFrameDelay: Call this to change the default delay after sending an empty DATAHEADER frame. Delay is in milliseconds.
		void setEOEmptyFrameDelay( byte bEOEF );
		// setEOEmptyFrameDelay: Call this to change the default delay after sending a DATA frame. Delay is in milliseconds.
		void setEOFrameDelay( byte bEOF );
		
private:
		byte _bLEDPin; // pin number connected to the LEDs
        byte* _ptRGBT; // RGB Buffer pointer
        unsigned int _uiRGBTabSz; // RGB Buffer size in bytes
		byte _bPulsUs; // Pulse length
		byte _bEOSMs; // End Of Sync delay in ms
		byte _bEORMs; // End Of Reset delay in ms
		byte _bEOEFrameMs; // End of Empty Data Frame delay in ms
		byte _bEOFrameMs; // End of Data Frame delay (end of all leds data in string)
		byte _bFrameCount; // Sent Frame counter

		void bitd(boolean b);
		void sync();
		void reset();
		void frame_start();
};
#endif