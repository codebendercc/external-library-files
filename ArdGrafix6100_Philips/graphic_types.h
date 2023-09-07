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
 * This file defines basic graphic types like xy & rectangle which are used by the library
 */

#ifndef __STDINT_H_
	#include <stdint.h>
#endif

#ifndef GRAPHICS_TYPES_H
#define GRAPHICS_TYPES_H

enum point_comparisons { COMPARE_BOTH_EQUAL, COMPARE_LEFT_TOP,COMPARE_LEFT_EQUAL,COMPARE_LEFT_BOTTOM, COMPARE_RIGHT_TOP, COMPARE_RIGHT_EQUAL,COMPARE_RIGHT_BOTTOM,COMPARE_EQUAL_TOP,COMPARE_EQUAL_BOTTOM,COMPARE_ERROR};

//defines xy class for points on screen etc. NOTE this uses (top, left) point = (0,0) convention
class xy
{

protected:
	int16_t x;
	int16_t y;
	
public:

//constructors
	xy(int16_t x_in, int16_t y_in);		//create the object setting the values of x & y
	xy();									//empty constructor, sets x & y values to 0

//utility functions
	point_comparisons compare( xy *comparepoint);	//returns one of 9 possible values denoting relative position of the comparepoint vs. current point

//getters & setters of x & y
	inline int16_t getx()				{	return x;	}		//get the x value
	inline int16_t gety()				{	return y;	}		//get the y value
	inline void setx(int16_t x_in)		{	x = x_in;	}		//set the x value
	inline void sety(int16_t y_in)		{	y = y_in;	}		//get the y value
	
//overloaded operators
	xy operator+(const xy& rhs) const;		//addx x & y of supplied point with current values & returns a fresh copy of the point
	xy operator-(const xy& rhs) const;		//subtract x & y of supplied point with current values & returns a fresh copy of the point
	xy & operator=(const xy &rhs);			//assignment - copies x & y
	xy & operator+=(const xy &rhs);			//adds x & y of supplied point to current point
	xy & operator-=(const xy &rhs);			//adds x & y of supplied point to current point
	bool operator==(const xy &rhs) const;	//equality comparison oeprator - both x & y must match
	bool operator!=(const xy &rhs) const {	return !(*this == rhs);	}	//not equalt to operator

};

//implements a rectangle class using topleft corner & size as xy objects
class rectangle
{

public:	
	xy topleftcorner;
	xy size;

	rectangle(xy topleftcorner_in, xy size_in)	//constructor with topleft corner & size being set by xy objects
	{
		topleftcorner = topleftcorner_in;
		size = size_in;
	}

	rectangle()		//default constructor
	{
		topleftcorner = xy(0,0);
		size = xy(0,0);
	}

	bool lies_fully_within(rectangle outer_rectangle);	//checks whether the this rectangle lies fully within the given outer rectangle, frequently requred for "within screen" checks

	xy get_topleftcorner();	//returns the top left corner	as an xy object
	xy get_size()			//returns the size of the rectangle as an xy object
	{return size;}	

	int16_t get_width()		//returns the width of the rectangle as an integer (int16_t)
	{	return size.getx();	}

	int16_t get_height()	//returns the height of the rectangle as an integer (int16_t)
	{	return size.gety();	}

	xy get_bottomleftcorner();	//returns the bootom left corner as an xy object
	xy get_bottomrightcorner();	//returns the bottom right corner as an xy object
	xy get_toprightcorner();

	void set_topleftcorner(xy topleftcorner_in)	//sets the topleft corner to given xy
	{	topleftcorner = topleftcorner_in;}	

	void set_size(xy size_in) //sets the topleft corner to given xy
	{	size = size_in;	}

	void set_topleft_bottomright(xy topleftcorner_in, xy bottomrightcorner_in) //use with caution - no error checking
	{
		topleftcorner = topleftcorner_in;
		size.setx(bottomrightcorner_in.getx() - topleftcorner_in.getx()+1);
		size.sety(bottomrightcorner_in.gety() - topleftcorner_in.gety()+1);
	}

	uint16_t get_numberofpixels()	//returns width * height
	{	return size.getx()*size.gety();	}
};


#endif