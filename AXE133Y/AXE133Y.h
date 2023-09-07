
/*
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */


//
// Code by SmudgerD, May 2012
//    www.stompville.co.uk
//

#ifndef AXE133Y_h
#define AXE133Y_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <inttypes.h>

class AXE133Y
{

public:
	// Initialize
  AXE133Y(uint8_t pin); 
      // write a byte to the display
  void writeByte(uint8_t foo);
	// Write splash screen
  void splash(String message); 
	// show/hide display (defaults to show)
  void displayShow(bool show); 
	// show cursor (defaults to no-show)
  void cursorShow(bool show); 
      // blink cursor
  void cursorBlink(bool blink);
	// clear screen and home cursor to beginning of line 1
  void clearScreen();
      // goto home (line 1 or line 2)
  void cursorHome(uint8_t line);
      // move cursor left
  void cursorLeft(uint8_t moves);
      // move cursor right
  void cursorRight(uint8_t moves);
      // move cursor to absolute position
  void cursorPosition(uint8_t line, uint8_t position);
      // display preloaded message
  void printMessage(uint8_t messageNumber);
      // print a string
  void print(String message);
      // set/reset all output pins
  void outputWrite(uint8_t state);
      // backspace and delete
  void backspace(uint8_t moves);
      // convert a float to a string for display
  void printFloat(float foo, uint8_t minimumLength, uint8_t decimalPlaces);
  
private:
  uint8_t _pin;
};

#endif


