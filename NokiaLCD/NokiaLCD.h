/*
  NokiaLCD.h - Library for a Nokia LCD with the epson driver.
  Created by Thomas Jespersen, July 2009 (Originally Arduino Sketch by Gravitech.us)
  Released into the public domain.
*/
#ifndef NokiaLCD_h
#define NokiaLCD_h

#include "WProgram.h"

class NokiaLCD
{
  public:
    void lcd_init();
    void draw_color_bar();
    void lcd_clear(uint16_t color, byte x0, byte y0, byte x1, byte y1);
    void LCDPutStr(char *pString, int x, int y, int Size, int fColor, int bColor);
    void LCDPutChar(char c, int x, int y, int size, int fColor, int bColor);
	void LCDSetLine(int x0, int y0, int x1, int y1, int color);
	void LCDSetRect(int x0, int y0, int x1, int y1, unsigned char fill, int color);
	void LCDSetCircle(int x0, int y0, int radius, int color);
	void LCDSetPixel(byte x, byte y, int color);
	void LCDSetXY(byte x, byte y);

  private:
    void shiftBits(byte data);
    void sendData(byte data);
    void sendCMD(byte data);
};

#endif
