/*
 * Copyright (c) 2012 by Hariharan Srinath, All right reserved.
 * ArdGrafix6100 - An Arduino/ATmega SPI Graphics Driver for Nokia 6100 LCDs
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. 
 * 
 * The latest version of this library can be found at its Google Code Project Page at http://code.google.com/p/ardgrafix6100/
 * For reporting issues, comments & requesting enhancements, pls use the "Issues" section at the Google Code Project page above
 * To contribute to the project, please send a mail to the project mailing list: ardgrafix6100@googlegroups.com
 *
 * This file defines the ArdGrafix6100_Philips class that contains the graphics driver for LCDs using the Philips PCF8833 controller chip
 */


#include <avr/io.h>
#include "fonts.h"
#include "graphic_types.h"
#include <string.h>
#include <util/delay.h>
#include <stdlib.h>


#ifndef ArdGrafix6100_PHILIPS_H
#define ArdGrafix6100_PHILIPS_H

#define PHILIPS_NOP 0x00 // nop
#define PHILIPS_SWRESET 0x01 // software reset
#define PHILIPS_BSTROFF 0x02 // booster voltage OFF
#define PHILIPS_BSTRON 0x03 // booster voltage ON
#define PHILIPS_RDDIDIF 0x04 // read display identification
#define PHILIPS_RDDST 0x09 // read display status
#define PHILIPS_SLEEPIN 0x10 // sleep in
#define PHILIPS_SLEEPOUT 0x11 // sleep out
#define PHILIPS_PTLON 0x12 // partial display mode
#define PHILIPS_NORON 0x13 // display normal mode
#define PHILIPS_INVOFF 0x20 // inversion OFF
#define PHILIPS_INVON 0x21 // inversion ON
#define PHILIPS_DALO 0x22 // all pixel OFF
#define PHILIPS_DAL 0x23 // all pixel ON
#define PHILIPS_SETCON 0x25 // write contrast
#define PHILIPS_DISPOFF 0x28 // display OFF
#define PHILIPS_DISPON 0x29 // display ON
#define PHILIPS_CASET 0x2A // column address set
#define PHILIPS_PASET 0x2B // page address set
#define PHILIPS_RAMWR 0x2C // memory write
#define PHILIPS_RGBSET 0x2D // colour set
#define PHILIPS_PTLAR 0x30 // partial area
#define PHILIPS_VSCRDEF 0x33 // vertical scrolling definition
#define PHILIPS_TEOFF 0x34 // test mode
#define PHILIPS_TEON 0x35 // test mode
#define PHILIPS_MADCTL 0x36 // memory access control
#define PHILIPS_SEP 0x37 // vertical scrolling start address
#define PHILIPS_IDMOFF 0x38 // idle mode OFF
#define PHILIPS_IDMON 0x39 // idle mode ON
#define PHILIPS_COLMOD 0x3A // interface pixel format
#define PHILIPS_SETVOP 0xB0 // set Vop
#define PHILIPS_BRS 0xB4 // bottom row swap
#define PHILIPS_TRS 0xB6 // top row swap
#define PHILIPS_DISCTR 0xB9 // display control
#define PHILIPS_DOR 0xBA // data order
#define PHILIPS_TCDFE 0xBD // enable/disable DF temperature compensation
#define PHILIPS_TCVOPE 0xBF // enable/disable Vop temp comp
#define PHILIPS_EC 0xC0 // internal or external oscillator
#define PHILIPS_SETMUL 0xC2 // set multiplication factor
#define PHILIPS_TCVOPAB 0xC3 // set TCVOP slopes A and B
#define PHILIPS_TCVOPCD 0xC4 // set TCVOP slopes c and d
#define PHILIPS_TCDF 0xC5 // set divider frequency
#define PHILIPS_DF8COLOR 0xC6 // set divider frequency 8-color mode
#define PHILIPS_SETBS 0xC7 // set bias system
#define PHILIPS_RDTEMP 0xC8 // temperature read back
#define PHILIPS_NLI 0xC9 // n-line inversion

//ATMEGA PORTB numbering of SPI Pins
#define PIN_CLK_PORTB 5      //maps to Arduino Pin 13
#define PIN_MOSI_PORTB 3     //maps to Arduino Pin 11
#define PIN_CS_PORTB 2       //maps to Arduino Pin 10
#define PIN_RST_PORTB 1      //maps to Arduino Pin  9

#define LCD_SPCR_ENABLE 0x50 //B01010000   enables SPI, sets as master, sets speed at 1/4 Master clock, 
#define LCD_SPCR_DISABLE 0x10 //B00010000  disables SPI, sets as master, sets speed at 1/4 Master clock, 

#define SCREEN_LOGICAL_WIDTH 128	//Nokia 6100 LCD usually has 128 visible horizontal pixels out of 132 possible pixels on controller
#define SCREEN_LOGICAL_HEIGHT 128	//Nokia 6100 LCD usually has 128 visible vertical pixels out of 132 possible pixels on controller

#define PHYSICAL_ADJUST_X 2			//adjustment of x reqd for a box to map logical pixels to physical pixels
#define PHYSICAL_ADJUST_Y 2			//adjustment of y reqd for a box to map logical pixels to physical pixels



class ArdGrafix6100_Philips			//	the Nokia 6100 LCD Screen Driver using the Philips PCF8833 controller
{
  protected: 
	  rectangle screen_dimensions;	//	stores the screen size, used in several comparison functions

	  inline void SPI_Transfer(uint8_t _data)	//the SPI code that trasfers commands using hardware SPI. This doesn't use Arduino's SPI
	  {
		  SPCR = LCD_SPCR_ENABLE;
		  SPDR = _data;
		  while(!(SPSR&0x80));
		  SPCR = LCD_SPCR_DISABLE;
	  }
  
	  inline uint16_t makeint(uint8_t char1, uint8_t char2)	//utility function takes 2 chars & makes it an int
	  {
		 uint16_t b0 = char1;
		 uint16_t b1 = char2;
		 return ((b0<<8|b1)&0xFFFF);
	  }
	  void WriteCommand(uint8_t command);		//writes one of the PHILIPS_XXXX commands using SPI
	  void WriteData(uint8_t data);				//writes data to the graphics controller using SPI
  
	  uint8_t getbit_fromdatablock(uint8_t* readstring,uint16_t bit_number);	//utility function to pull out a bit from a block of data - eg. pull out 13th bit from a set of 10 bytes	
	  void _rawprintchar(rectangle position, uint8_t *buf, uint16_t forecolor, uint16_t backcolor, uint8_t transparent_back);	//internal function to print a char given its bitmap... uses setpixel to give an option for transparancy
	  
  public:
	  ArdGrafix6100_Philips();					//constructor - create a new global LCD object in your code & call this constructor

	  //Hardware based functions for SPI communication with graphics controller - these need to be changed to suit Epson controller for porting
	  void lcd_reset();							//runs through the reset cycle of the philips PFC8833 controler & turns on the screen in its default 4096 color (12bit) mode

	  //Hardware based Graphics functions - these are fast functions - also need to be changed to suit Epson Controller for porting
	  bool set_position_for_write(rectangle rect);	//the nokia6100 controllers (both philips & epson) need you to set a "window" to write to before any data can be written to the screen ram
	  bool setpixel(xy point, uint16_t color);	//sets a pixel on screen to a particular color - the color format is 0xRGBX where the last 4 bits of the 16 bit integer are ignored. Internally, this sets the "write window" using set_position_for_write() to 1x1, writes the pixel to controller & then sends a NOP to make the controller write the pixel to screen. Note: fillrect() and showbitmap() are faster hardware oriented functions to set a rectangle to a particular color or show a 12bit bitmap and will be much faster than looped setpixels
	  bool fillrect(rectangle rect ,uint16_t color);	//fills a rectangle on screen to a particular color. This is implemented via a hardware oriented block write rather than using setpixels
	  void showbitmap(rectangle rect,uint8_t *data);	//fills a rectangle on screen with a given 12bit bitmap using hardware oriented block writes rather than setpixel

	  //Software based graphics functions - basic graphic primitives implemented using the hardware based grahic functions
	  void draw_rect(rectangle rect,uint16_t bordercolor, int16_t thickness);		//there are two overloaded "rects" - one lets you draw an empty box & the other lets you fill it
	  void draw_rect(rectangle rect,uint16_t bordercolor, uint16_t thickness, uint16_t fillcolor); //there are two overloaded "rects" - one lets you draw an empty box & the other lets you fill it
	  void draw_line(xy point1, xy point2,uint16_t color);							//draw line between any two on-screen points
	  void draw_circle(xy focus, uint16_t radius, uint16_t color);					//draw a circle
	  uint16_t printchars(char *string, genericfont *font, xy topleftcorner, uint16_t forecolor, uint16_t backcolor, bool transparent_back);	//print a string using the given font, forecolor, backcolor & transparency option (true or false)
	  void clearscreen(uint16_t color)
	  {	fillrect(screen_dimensions,color);	}

}; 

#endif