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
 * This file defines basic graphic types like xy & rectangle which are used by the library
 */

#include "graphic_types.h"

xy::xy(int16_t x_in, int16_t y_in)
{
	x = x_in;
	y = y_in;
}

xy::xy()
{
	x= 0;
	y = 0;
}



//tells where the comparepoint is with respect to this point - 9 possible return values
point_comparisons  xy::compare( xy *comparepoint)
{
	if(comparepoint->x == x && comparepoint->y ==y)
		return COMPARE_BOTH_EQUAL;
		
	if(comparepoint->x == x && comparepoint->y >y)
		return COMPARE_EQUAL_BOTTOM;

	if(comparepoint->x == x && comparepoint->y < y)
		return COMPARE_EQUAL_TOP;

	if(comparepoint->x < x && comparepoint->y > y)
		return COMPARE_LEFT_BOTTOM;

	if(comparepoint->x < x && comparepoint->y ==y)
		return COMPARE_LEFT_EQUAL;

	if(comparepoint->x < x && comparepoint->y < y)
		return COMPARE_LEFT_TOP;

	if(comparepoint->x > x && comparepoint->y > y)
		return COMPARE_RIGHT_BOTTOM;

	if(comparepoint->x > x && comparepoint->y == y)
		return COMPARE_RIGHT_EQUAL;

	if(comparepoint->x > x && comparepoint->y < y)
		return COMPARE_RIGHT_TOP;

	return COMPARE_ERROR; //should never happen really - mainly to suppress a compiler warning
}

xy & xy::operator-=(const xy &rhs)			//adds x & y of supplied point to current point
{
	if( this != &rhs)
	{
		x = x-rhs.x;
		y = y-rhs.y;
	}
	return *this;
}

xy & xy::operator+=(const xy &rhs)			//adds x & y of supplied point to current point
{
	if( this != &rhs)
	{
		x = x+rhs.x;
		y = y+rhs.y;
	}
	return *this;
}

xy & xy::operator=(const xy &rhs)			//assignment - copies x & y
{
	if( this != &rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}
	return *this;
}

xy xy::operator+(const xy& rhs) const		//addx x & y of supplied point with current values & returns a fresh copy of the point
{	return xy(x+rhs.x,y+rhs.y);		}

xy xy::operator-(const xy& rhs) const		//subtract x & y of supplied point with current values & returns a fresh copy of the point
{	return xy(x-rhs.x,y-rhs.y);		}

bool xy::operator==(const xy &rhs) const {
	if(x == rhs.x && y == rhs.y )
		return true;
	else
		return false;
}


bool rectangle::lies_fully_within(rectangle outer_rectangle)
{
	if(topleftcorner.getx()>=outer_rectangle.topleftcorner.getx())
		if(topleftcorner.gety()>=outer_rectangle.topleftcorner.gety())
			if((topleftcorner.getx()+size.getx()-1) <= (outer_rectangle.topleftcorner.getx()+outer_rectangle.size.getx()-1))
				if((topleftcorner.gety()+size.gety()-1) <= (outer_rectangle.topleftcorner.gety()+outer_rectangle.size.gety()-1))
					return true;

	return false;
}

xy rectangle::get_topleftcorner()
{	return xy(topleftcorner);	}

xy rectangle::get_bottomleftcorner()
{	return xy(topleftcorner.getx(),topleftcorner.gety()+size.gety()-1);	}

xy rectangle::get_bottomrightcorner()
{	return xy(topleftcorner.getx()+size.getx()-1,topleftcorner.gety()+size.gety()-1);	}

xy rectangle::get_toprightcorner()
{	return xy(topleftcorner.getx()+size.getx()-1,topleftcorner.gety());	}

