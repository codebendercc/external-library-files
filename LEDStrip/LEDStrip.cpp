/*
This library is designed to make communication with an APA102 LED
strip simple and compact. It provides an object with just a few 
interaction methods to write individual pixels or all pixels at once,
clear and draw functions to update the display, and a single number
color interface to simplify color mixing.

Copyright (c) 2015, Let's Start Coding
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the organization nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Let's Start Coding BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "LEDStrip.h"

LEDStrip::LEDStrip(int n, byte data, byte clock){
	//set up the communication port
	dataPin = data;
	clockPin = clock;
	pinMode(dataPin , OUTPUT);
	pinMode(clockPin, OUTPUT);
	digitalWrite(dataPin , LOW);
	digitalWrite(clockPin, LOW);
	
	//These variables let you specify to 'set all pixels'
	ALL = "ALL";
	All = "ALL";
	all = "ALL";
	aLL = "ALL";
	
	//Strip related set up
	defaultBright = 100; //Default brightness level
	int bytes = n*3; //3 RGB components per pixel
	pixels = (byte *)malloc(bytes); //Create space for the RGB color components
	brightnesses = (byte *)malloc(n); //for the brightness values
	memset(brightnesses,map(defaultBright,0,100,0,50),n);//set all brightnesses to default
    numLEDs = n; //keep a record of the number of LEDs
    clear(); //blank the strip on init
    draw(); //Draw the cleared strip
}

void LEDStrip::clear() {
	memset(pixels, 0, numLEDs*3); //Erase all pixels at once
}

void LEDStrip::draw() {
	//put the current color array on the strip
	byte *ptr = pixels; //point at the pixels array
	int n   = numLEDs; //iterate over all the LEDs

	//start packet
	for(int i=0; i<4; i++){
  		sw_spi_out(0);}
	//transmit
	while (n > 0) {
    	sw_spi_out(0xFF); //section header
    	for(int i=0; i<3; i++){
			sw_spi_out(*ptr++);//push out each color value in the RGB
		}
    	n--;}
   	//end packet
	for(int i=0; i<4; i++) sw_spi_out(0xFF); //end-frame footer- irrelevant, but reserved by APA
}


void LEDStrip::setPixel(int n, int color, int brightness) {
	//Write a specific pixel n to given color with given brightness value
	if (brightness != -1){
		brightnesses[n] = brightness;
	}
	unsigned long c = mix_color_int(color,n); //make the encoded color for transmission
	if(n < numLEDs) {
		//check if actually in the list- writing to a pixel past the end can cause problems
		byte *p = &pixels[n*3]; //Getting access to the array
    	p[0] = (byte)(c >> 16); //Write the R value 
    	p[1] = (byte)(c >>  8); //Write the G value
    	p[2] = (byte)c;			//Write the B value
  }
}

void LEDStrip::setPixel(String ALL, int color, int brightness) {
	//Ser all the pixels to a color, and a specific brightness
	for (int i=0;i<numLEDs;i++){
		if (brightness != -1){
			setPixel(i,color,brightness); //Just piggyback on the original function
		}
		else{
			setPixel(i,color);	//If you aren't changing the brightness
		}
	}
}


unsigned long LEDStrip::mix_color_int(int color, int n){
  //A utility function to take an integer 0-299 straight to
  //  a 1-D color. Basically a formatting conversion.
  if (color == 300){
  	return mix_color(254,254,254,n);
  }
  int colors[3] = {0,0,0};
  int_to_color(color,colors);
  unsigned long color_out = mix_color(colors[2],colors[1],colors[0],n);
  return color_out;
}

unsigned long LEDStrip::mix_color(unsigned long red, unsigned long green, unsigned long blue,int n){
  //A utility function to turn an RGB color into a
  //  HEX value color. Just math.
  unsigned long color = (blue*brightnesses[n]/100) + (green*brightnesses[n]/100<<8) + (red*brightnesses[n]/100<<16);
  return color;
}

void LEDStrip::int_to_color(int color, int color_out[3]){
  //A utility function to take an integer and convert it
  //  to color compontnents, for setting values on the strip itself
  int region = (0<=color)*(color<100) + 2*(100<=color)*(color<200) + 4*(200<=color)*(color<300);
  color_out [0] = (color)*((region&1)>0) + (200 - color)*((region&2)>0);
  color_out [1] = (color-100)*((region&2)>0) + (300 - color)*((region&4)>0);
  color_out [2] = (100-color)*((region&1)>0) + (color-200)*((region&4)>0);
}

void LEDStrip::sw_spi_out(byte n) {
	//send out a byte n on the 'port', just bit-banging the pins
	for(uint8_t i=8; i--; n<<=1){
		//clock it out by the MSB
		if(n & 0x80){
			//High if MSB 0x08 (0b10000000) is high
			digitalWrite(dataPin, HIGH);}
    	else{
			digitalWrite(dataPin, LOW);}
		//toggle the clock to push data through
    	digitalWrite(clockPin, HIGH);
    	digitalWrite(clockPin, LOW);
  	}
}

