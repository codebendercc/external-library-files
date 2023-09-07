/*
 * Copyright (c) 2012 by Hariharan Srinath, All right reserved.
 * ArdGrafix6100 - An Arduino/ATmega SPI Graphics Driver for Nokia 6100 LCDs
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. 
 * 
 * The latest version of this library can be found at its Google Code Project Page at http://code.google.com/p/ardgrafix6100
 * For reporting issues, comments & requesting enhancements, pls use the "Issues" section at the Google Code Project page above
 * To contribute to the project, please send a mail to the project mailing list: ardgrafix6100@googlegroups.com
 *
 * This file contains the sample application using the Philips version of the ArdGrafix6100 library
 */


#include <Arduino.h>
#include <fonts.h>
#include <graphic_types.h>
#include <ArdGrafix6100_Philips.h>


ArdGrafix6100_Philips *my_lcd;
font_prop_sans_height14px_font *my_font;
uint8_t rainbow_ball_bitmap[] = {255,255,255,255,255,255,205,199,167,73,37,146,155,46,197,236,127,221,255,255,255,255,255,255,255,255,255,255,247,167,39,18,129,90,42,178,235,63,163,247,47,98,247,127,255,255,255,255,255,255,255,88,82,113,57,21,162,171,46,195,250,63,131,245,47,67,228,44,101,255,255,255,255,247,167,39,19,145,90,42,162,236,63,163,247,47,82,244,46,67,212,61,83,184,127,255,205,194,113,57,21,162,170,46,195,250,63,115,245,47,66,228,61,66,197,59,67,133,94,221,106,115,145,89,42,178,236,63,163,247,47,98,244,46,67,212,61,83,165,57,84,133,138,140,73,54,145,171,46,195,250,63,131,245,47,67,228,45,67,197,58,67,149,72,88,133,200,94,89,42,178,236,63,163,247,63,82,244,46,67,212,44,67,181,56,85,133,136,91,133,248,111,171,46,195,250,63,115,245,47,66,228,61,66,197,59,83,133,89,88,133,183,95,134,248,127,236,95,163,247,47,98,244,46,67,212,60,83,180,56,85,133,136,91,133,248,111,135,250,175,252,143,114,245,63,66,228,61,66,197,59,83,133,88,88,133,184,95,118,248,127,154,251,191,254,207,82,244,46,67,212,60,67,181,56,85,133,136,91,133,248,111,135,249,159,156,254,255,255,255,119,228,61,66,197,59,83,133,88,88,133,184,79,134,248,143,153,249,207,206,255,255,255,255,255,198,92,67,165,57,85,133,136,92,133,248,111,136,248,159,156,252,239,255,255,255,255,255,255,255,252,135,133,88,88,133,183,79,134,248,143,153,249,207,206,255,255,255,255,255,255,255,255,255,255,255,237,234,124,134,248,111,152,249,175,204,254,255,255,255,255,255,255,255};

int left, top, width, height;
unsigned int fillcolor, bordercolor,thickness;


void setup()
{
	randomSeed(analogRead(0));
	my_lcd = new ArdGrafix6100_Philips();
	my_font = new font_prop_sans_height14px_font();

	my_lcd->clearscreen(0x0000);
}


void loop()
{
	xy topleft(random(0,128),random(0,128));
	xy size(random(4,128-topleft.getx()+1), random(4,128-topleft.getx()+1));
	fillcolor = random(0,65535);
	bordercolor = random(0,65535);

	my_lcd->draw_rect(rectangle(topleft,size),bordercolor, 2, fillcolor); 
	my_lcd->printchars("Testing",my_font,xy(18,1),0xFF00,0x0000,true);
	my_lcd->showbitmap(rectangle(xy(0,0),xy(16,16)),rainbow_ball_bitmap);
}

