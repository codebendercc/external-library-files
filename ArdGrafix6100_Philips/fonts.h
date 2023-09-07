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
 * This file defines the genericfont base class which forms base for fonts. It also provides a default 14-pixel height sans-serif proportional font
 */
 

#ifndef __STDINT_H_
	#include <stdint.h>
#endif

#include "graphic_types.h"

#ifndef FONTS_H
#define FONTS_H

enum font_return_codes {FONT_CHARACTER_EXISTS, FONT_CHARACTER_DOES_NOT_EXIST};

class genericfont //Abstract Font base class. THis defines minimum features that MUST be common to fonts. 
{
public:
	virtual uint16_t get_max_buf_size();	//this function should return the maximum size of buffer requred to hold a font's mono bitmap
	virtual uint16_t get_character_spacing();	//this function should return the preferred character spacing
	virtual font_return_codes getcharacter(uint8_t asciicode, uint8_t * buf, xy* size);	//this function should return the bitmap of the specified ascii character and its size
};
  
class font_prop_sans_height14px_font:public genericfont
{
public:
	//get the font character's bitmap along with its width & height into the 
	uint16_t get_max_buf_size()
	{	return 20;	}

	font_return_codes getcharacter(uint8_t asciicode, uint8_t * buf, xy* size);

	uint16_t get_character_spacing()
	{	return 1;	};

};


#endif
