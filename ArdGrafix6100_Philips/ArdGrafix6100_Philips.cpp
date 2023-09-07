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


#include "ArdGrafix6100_Philips.h"


uint8_t ArdGrafix6100_Philips::getbit_fromdatablock(uint8_t* readstring,uint16_t bit_number)
{
	uint16_t char_to_read = readstring[bit_number/8];
	uint8_t pos = 7-(bit_number % 8);
	if (char_to_read&(1<<pos))
		return 1;
	else
		return 0;
}


ArdGrafix6100_Philips::ArdGrafix6100_Philips()
{
	screen_dimensions.set_size(xy(SCREEN_LOGICAL_WIDTH,SCREEN_LOGICAL_HEIGHT));
	screen_dimensions.set_topleftcorner(xy(0,0));

	//initialize the Arduino SPI Pins as outputs 
	DDRB |= _BV(PIN_CLK_PORTB);
	DDRB |= _BV(PIN_MOSI_PORTB);
	DDRB |= _BV(PIN_CS_PORTB);
	DDRB |= _BV(PIN_RST_PORTB);

	PORTB |= _BV(PIN_RST_PORTB); //set reset to high (low is active)
	PORTB |= _BV(PIN_CS_PORTB);  //set Chip Select pin high (low is active)
	PORTB &= ~(_BV(PIN_CLK_PORTB)); //set clock low - get ready for first write
      
	lcd_reset();
}

     
void ArdGrafix6100_Philips::lcd_reset()
{
    PORTB &= ~(_BV(PIN_RST_PORTB));    //take the reset pin low
	_delay_us(4);
    //delayMicroseconds(4);              // wait for a short while
    PORTB |= _BV(PIN_RST_PORTB);       //then take it back high
    //delay(1);                          // wait for reset to complete
    _delay_ms(1);

    WriteCommand(PHILIPS_SLEEPOUT);	         //Turn sleep mode off
      
    WriteCommand(PHILIPS_INVOFF);
      
    WriteCommand(PHILIPS_COLMOD);              //set the color mode
    WriteData(0x03);                   //to 0x03 = 12 bits-per-pixel - 4R 4G 4B

    WriteCommand(PHILIPS_MADCTL);              //establish how the memory should be read Memory access controler (command 0x36)
    WriteData(0xC0);                   // mirror x & mirror y so that top,left is 0,0 - scan in x direction top to bottom 
                                        //and color order is RGB

    WriteCommand(PHILIPS_SETCON);              // Write contrast (command 0x25)
    WriteData(0x38);                   // contrast 0x30
    _delay_ms(1);
      
    WriteCommand(PHILIPS_DISPON);
}
        
void ArdGrafix6100_Philips::WriteCommand(uint8_t command)
{
    PORTB &= ~(_BV(PIN_CS_PORTB));      //Pull the Chip Select pin low to enable chip communication
    PORTB &= ~(_BV(PIN_MOSI_PORTB));    //THis is a command - so first D/C data bit of 9 bits must be LOW - pull MOSI low
      
    PORTB |= _BV(PIN_CLK_PORTB);        //Pulse the clock HIGH
    asm("nop"); asm("nop"); 		  //Wait for 4 clock cycles by nop assembly instruction
    asm("nop"); asm("nop");
    PORTB &= ~(_BV(PIN_CLK_PORTB));     //THen pulse the clock back to LOW to complete transfer of first D/C bit
      
    SPI_Transfer(command);              //Transfer the actual command with hardware SPI
    PORTB |= _BV(PIN_CS_PORTB);         //Pull the Chip Select pin bach HIGH to disable chip communication
}
    
void ArdGrafix6100_Philips::WriteData(uint8_t data)
{
    PORTB &= ~(_BV(PIN_CS_PORTB));      //Pull the Chip Select pin low to enable chip communication
    PORTB |= _BV(PIN_MOSI_PORTB);       //THis is a data - so first D/C data bit of 9 bits must be HIGH - pull MOSI HIGH 

    PORTB |= _BV(PIN_CLK_PORTB);        //Pulse the clock HIGH
    asm("nop"); asm("nop"); 		  //Wait for 4 clock cycles by nop assembly instruction
    asm("nop"); asm("nop");
    PORTB &= ~(_BV(PIN_CLK_PORTB));     //THen pulse the clock back to LOW to complete transfer of first D/C bit

    SPI_Transfer(data);                 //Transfer the actual data with hardware SPI 
    PORTB |= _BV(PIN_CS_PORTB);         //Pull the Chip Select pin bach HIGH to disable chip communication
}
    

bool ArdGrafix6100_Philips::set_position_for_write(rectangle rect)
{
    //The actual LCD is 128 x 128 with row/col 0,1 & 131,132 not visible 
    //so we check for inputs within 0-128/0-128 

	if(!rect.lies_fully_within(screen_dimensions))
		return false;
	else
	{
	    WriteCommand(PHILIPS_CASET);
		WriteData(rect.get_topleftcorner().getx()+PHYSICAL_ADJUST_X); 
		WriteData(rect.get_bottomrightcorner().getx()+PHYSICAL_ADJUST_X);
      
		WriteCommand(PHILIPS_PASET);
		WriteData(rect.get_topleftcorner().gety()+PHYSICAL_ADJUST_Y); 
		WriteData(rect.get_bottomrightcorner().gety()+PHYSICAL_ADJUST_Y);
      
		return true;
    }
} 
   
bool ArdGrafix6100_Philips::setpixel(xy point, uint16_t color)
{
	if(set_position_for_write(rectangle(point,xy(1,1))))          //set the write position to x,y with width/height = 1
	{
		WriteCommand(PHILIPS_RAMWR);                      //set RAM WRITE mode on
		WriteData(((unsigned char)(color>>8)));   //Send the R (Bits 15 to 12) & G (bits 11 to 8) of the 12 bit color coded in 16 bit int
		WriteData(((unsigned char)(color)));      //Send the B (Bits 7 to 4) of the color. Bits 3 to 1 should point to next pixel's R
		WriteCommand(PHILIPS_NOP);                        //but because we send a NOP immediately, the next pixel is ignored and the given pixel is written
		return true;
	}
	else
		return false;
}
   
  
bool ArdGrafix6100_Philips::fillrect(rectangle rect,uint16_t color)
{
    if(set_position_for_write(rect))  //set the positions for writing
	{
		uint16_t byte_count = rect.get_numberofpixels() *3/2+1;
      
		uint8_t buf[3];                              //3 bytes for 2 pixels in the 12 bits per pixel mode - ie 2 pixel color buffe
		buf[0]=((color>>8)&0xFF);                          //Fill up the First byte with R and G of the first pixel in the color
		buf[1]=(((color>>4)&0xF)<<4)|((color>>12)&0xF);    //Fill the second byte with of B of first pixel & R of second pixel
		buf[2]=((color>>4)&0xFF);                          //Fill the third byte with of G and B of second pixel
		uint8_t bufptr = 0;                          //we will use the pointer to keep track of writes

		WriteCommand(PHILIPS_RAMWR);                               //start writing to display RAM
     
		PORTB &= ~(_BV(PIN_CS_PORTB));                     //Pull the Chip Select pin low to enable chip communication
      
		for(uint16_t j = 0; j<byte_count;j++)
		{
			PORTB |= _BV(PIN_MOSI_PORTB);                   //THis is a data - so first D/C data bit of 9 bits must be HIGH - pull MOSI HIGH 
			PORTB |= _BV(PIN_CLK_PORTB);                    //Pulse the clock HIGH
			asm("nop"); asm("nop"); 		        //Wait for 4 clock cycles by nop assembly instruction
			asm("nop"); asm("nop");
			PORTB &= ~(_BV(PIN_CLK_PORTB));                 //THen pulse the clock back to LOW to complete transfer of first D/C bit

			SPI_Transfer(buf[bufptr]);                      //transfer byte from the color buffer
			bufptr++;                                       //and increment the byte pointer in color buffer 
        
			if(bufptr>2)                                    //at end of 2 pixels transferred in the color buffer
				bufptr = 0;                                   //reset the buffer to 0
		}
		PORTB |= _BV(PIN_CS_PORTB);                         //Pull the Chip Select pin bach HIGH to disable chip communication
		WriteCommand(PHILIPS_NOP);                                  //Write the NOP to close out data transfer
		return true;
	}
	else
		return false;
}
   
//rect version 1 - draws rectangle with lines
void ArdGrafix6100_Philips::draw_rect(rectangle rect,uint16_t bordercolor, int16_t thickness)
{
	if(thickness>0 && thickness <= rect.get_height() && thickness <= rect.get_width())
	{
		fillrect(rectangle(rect.get_topleftcorner(),xy(thickness,rect.get_height())),bordercolor);
		fillrect(rectangle(rect.get_topleftcorner(),xy(rect.get_width(),thickness)),bordercolor);
		fillrect(rectangle(rect.get_bottomleftcorner()-xy(0,thickness-1),xy(rect.get_width(),thickness)),bordercolor);
		fillrect(rectangle(rect.get_toprightcorner()-xy(thickness-1,0),xy(thickness,rect.get_height())),bordercolor);
	}
}
   
void ArdGrafix6100_Philips::draw_rect(rectangle rect,uint16_t bordercolor, uint16_t thickness,uint16_t fillcolor)
{
    fillrect(rect,fillcolor);
    draw_rect(rect,bordercolor,thickness);
}
   
void ArdGrafix6100_Philips::showbitmap(rectangle rect,uint8_t *data)
{
    set_position_for_write(rect);
    uint16_t byte_count = rect.size.getx()*rect.size.gety()*3/2+1; 
     
    WriteCommand(PHILIPS_RAMWR);                               //start writing to display RAM
     
    PORTB &= ~(_BV(PIN_CS_PORTB));                     //Pull the Chip Select pin low to enable chip communication
      
    for(uint16_t j = 0; j<byte_count;j++)
    {
		PORTB |= _BV(PIN_MOSI_PORTB);                   //THis is a data - so first D/C data bit of 9 bits must be HIGH - pull MOSI HIGH 
        
		PORTB |= _BV(PIN_CLK_PORTB);                    //Pulse the clock HIGH
		asm("nop"); asm("nop"); 		        //Wait for 4 clock cycles by nop assembly instruction
		asm("nop"); asm("nop");
		PORTB &= ~(_BV(PIN_CLK_PORTB));                 //THen pulse the clock back to LOW to complete transfer of first D/C bit

		SPI_Transfer(data[j]);                      //transfer byte from the color buffer
	}
	PORTB |= _BV(PIN_CS_PORTB);                         //Pull the Chip Select pin bach HIGH to disable chip communication
	WriteCommand(PHILIPS_NOP);                                  //Write the NOP to close out data transfer
}



void ArdGrafix6100_Philips::_rawprintchar(rectangle position, uint8_t *buf, uint16_t forecolor, uint16_t backcolor, uint8_t transparent_back)
{
	uint16_t bitpos = 0;
	int16_t x,y;
	for(y = position.get_topleftcorner().gety(); y <= position.get_bottomrightcorner().gety(); y++)
		for(x = position.get_topleftcorner().getx(); x <=position.get_bottomrightcorner().getx(); x++)
		{
			if(getbit_fromdatablock(buf,bitpos))
				setpixel(xy(x,y), forecolor);
			else
				if(transparent_back==false)
					setpixel(xy(x,y), backcolor);
			
			bitpos++;
		}
}

//prints characters
uint16_t ArdGrafix6100_Philips::printchars(char *string, genericfont *font, xy topleftcorner, uint16_t forecolor, uint16_t backcolor, bool transparent_back)
{
	uint8_t *buf = new uint8_t(font->get_max_buf_size());	//create a buffer of the maximum size required by the font

	font_return_codes ret;

	uint16_t len = strlen(string);
	uint16_t charswritten = 0;

	rectangle curcharpos;
	curcharpos.set_topleftcorner(topleftcorner);
	xy size;

	//curpos = topleftcorner;		//uses overloaded assigment operator to copy values

	for( uint16_t j = 0; j < len; j++)
	{
		ret = font->getcharacter(string[j],buf,&size);

		if(ret == FONT_CHARACTER_DOES_NOT_EXIST)
		{		font->getcharacter(32,buf,&size);	}   //replace any non-existant character with a "space"
		
		curcharpos.set_size(size);
		if(curcharpos.lies_fully_within(screen_dimensions))
		{
			_rawprintchar( curcharpos,buf,forecolor,backcolor,transparent_back);
			charswritten++;
			curcharpos.set_topleftcorner( curcharpos.get_toprightcorner()+xy(1,0));
			curcharpos.set_size(xy(font->get_character_spacing(),curcharpos.get_size().gety()));
					
			//now give a 1 pixel space
			if(!transparent_back)
			{	fillrect(curcharpos,backcolor);	}

			curcharpos.set_topleftcorner( curcharpos.get_toprightcorner()+xy(1,0));
		}
	}
	delete buf;
	return charswritten;

}

   
void ArdGrafix6100_Philips::draw_line(xy point1, xy point2,uint16_t color)
{
    int16_t dx,dy,sx,sy,err,e2;
       
	dx = abs(point2.getx()-point1.getx());
	dy = abs(point2.gety()-point1.gety()); 
    sx = (point1.getx() < point2.getx())?1:-1;
    sy = (point1.gety() < point2.gety())?1:-1;
       
    err = dx-dy;
 
    while(1)
    {
		setpixel(point1,color);
        if( point1 == point2)
			break; 

        e2 = err<<1;
        if( e2 > -dy)
        { 
			err = err - dy;
			point1.setx(point1.getx()+ sx);
        }
        if( e2 <  dx)
        { 
			err = err + dx;
			point1.sety(point1.gety() + sy);
        } 
    }     
}
   
   

void ArdGrafix6100_Philips::draw_circle(xy focus, uint16_t radius, uint16_t color)
{
    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;
 
    setpixel(focus+xy(0,radius),color);
    setpixel(focus-xy(0,radius),color);
    setpixel(focus+xy(radius,0),color);
    setpixel(focus-xy(radius,0),color);
 
    while(x < y)
    {

    if(f >= 0) 
    {
        y--;
        ddF_y += 2;
        f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    


	setpixel(focus+xy(x,y),color);
    setpixel(focus+xy(-x,y),color);
    setpixel(focus+xy(x,- y),color);
    setpixel(focus+xy(-x,-y),color);
	setpixel(focus+xy(y,x),color);
    setpixel(focus+xy(-y, x),color);
    setpixel(focus+xy(y,-x),color);
    setpixel(focus+xy(-y,-x),color);

    }
}


    
