#ifndef apa102h
#define apa102h

#include <Arduino.h>


class LEDStrip{

 public:

    LEDStrip(int n, byte data, byte clock);

  	void clear();

    void setPixel(int pixel, int color, int brightness = -1);
    void setPixel(String ALL, int color, int brightness = -1);

    void draw();
    
    byte defaultBright;
    
    String ALL;
	String All;
	String all;
	String aLL;
	

 private:

    unsigned long mix_color_int(int color,int n);
    unsigned long mix_color(unsigned long red, unsigned long green, unsigned long blue,int n);
	int numLEDs;
	byte dataPin;
    byte clockPin;
	byte *pixels;
	byte *brightnesses;	
	byte dataPinMask;
    byte clockPinMask;
	volatile byte *dataPort;
	volatile byte *clockPort;
	int brightness;
	
    void sw_spi_init(void);
    void sw_spi_out(uint8_t n);
    void int_to_color(int color, int color_out[3]);

};

#endif
