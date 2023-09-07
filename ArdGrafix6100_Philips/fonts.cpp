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

#include "fonts.h"


font_return_codes font_prop_sans_height14px_font::getcharacter(uint8_t asciicode, uint8_t * buf, xy* size)
{
	switch(asciicode)
	{
	case 32:
		size->setx(3);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x0;
		buf[3]=0x0;
		buf[4]=0x0;
		buf[5]=0x0;
		break;
	case 33:
		size->setx(1);
		
		buf[0]=0x7f;
		buf[1]=0x40;
		break;
	case 34:
		size->setx(3);
		
		buf[0]=0x16;
		buf[1]=0xd0;
		buf[2]=0x0;
		buf[3]=0x0;
		buf[4]=0x0;
		buf[5]=0x0;
		break;
	case 35:
		size->setx(6);
		
		buf[0]=0x0;
		buf[1]=0x2;
		buf[2]=0x8a;
		buf[3]=0x7c;
		buf[4]=0xa5;
		buf[5]=0x3e;
		buf[6]=0x51;
		buf[7]=0x40;
		buf[8]=0x0;
		buf[9]=0x0;
		buf[10]=0x0;
		break;
	case 36:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x9d;
		buf[2]=0x18;
		buf[3]=0x30;
		buf[4]=0x41;
		buf[5]=0x8b;
		buf[6]=0x90;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 37:
		size->setx(9);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x18;
		buf[3]=0x52;
		buf[4]=0x49;
		buf[5]=0x43;
		buf[6]=0x40;
		buf[7]=0x2c;
		buf[8]=0x29;
		buf[9]=0x24;
		buf[10]=0xa1;
		buf[11]=0x80;
		buf[12]=0x0;
		buf[13]=0x0;
		buf[14]=0x0;
		buf[15]=0x0;
		break;
	case 38:
		size->setx(7);
		
		buf[0]=0x0;
		buf[1]=0x61;
		buf[2]=0x22;
		buf[3]=0x43;
		buf[4]=0xc;
		buf[5]=0x64;
		buf[6]=0xc6;
		buf[7]=0x8c;
		buf[8]=0xe4;
		buf[9]=0x0;
		buf[10]=0x0;
		buf[11]=0x0;
		buf[12]=0x0;
		break;
	case 39:
		size->setx(1);
		
		buf[0]=0x70;
		buf[1]=0x0;
		break;
	case 40:
		size->setx(2);
		
		buf[0]=0x16;
		buf[1]=0xaa;
		buf[2]=0xa5;
		buf[3]=0x0;
		break;
	case 41:
		size->setx(2);
		
		buf[0]=0x29;
		buf[1]=0x55;
		buf[2]=0x5a;
		buf[3]=0x0;
		break;
	case 42:
		size->setx(5);
		
		buf[0]=0x1;
		buf[1]=0x2a;
		buf[2]=0xea;
		buf[3]=0x90;
		buf[4]=0x0;
		buf[5]=0x0;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 43:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x42;
		buf[3]=0x13;
		buf[4]=0xe4;
		buf[5]=0x21;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 44:
		size->setx(2);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x16;
		buf[3]=0x0;
		break;
	case 45:
		size->setx(3);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x38;
		buf[3]=0x0;
		buf[4]=0x0;
		buf[5]=0x0;
		break;
	case 46:
		size->setx(1);
		
		buf[0]=0x0;
		buf[1]=0x40;
		break;
	case 47:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x42;
		buf[2]=0x21;
		buf[3]=0x10;
		buf[4]=0x84;
		buf[5]=0x42;
		buf[6]=0x21;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 48:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x1d;
		buf[2]=0x18;
		buf[3]=0xc6;
		buf[4]=0x31;
		buf[5]=0x8b;
		buf[6]=0x80;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 49:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x19;
		buf[2]=0x42;
		buf[3]=0x10;
		buf[4]=0x84;
		buf[5]=0x27;
		buf[6]=0xc0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 50:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x1d;
		buf[2]=0x10;
		buf[3]=0x84;
		buf[4]=0x44;
		buf[5]=0x47;
		buf[6]=0xc0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 51:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x1d;
		buf[2]=0x10;
		buf[3]=0xb8;
		buf[4]=0x21;
		buf[5]=0x8b;
		buf[6]=0x80;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 52:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0xc;
		buf[2]=0x65;
		buf[3]=0x2a;
		buf[4]=0x5f;
		buf[5]=0x10;
		buf[6]=0x80;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 53:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x3f;
		buf[2]=0x8;
		buf[3]=0x78;
		buf[4]=0x21;
		buf[5]=0x8b;
		buf[6]=0x80;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 54:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0xe;
		buf[2]=0x88;
		buf[3]=0x7a;
		buf[4]=0x31;
		buf[5]=0x8b;
		buf[6]=0x80;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 55:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x3e;
		buf[2]=0x11;
		buf[3]=0x8;
		buf[4]=0x84;
		buf[5]=0x42;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 56:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x1d;
		buf[2]=0x18;
		buf[3]=0xba;
		buf[4]=0x31;
		buf[5]=0x8b;
		buf[6]=0x80;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 57:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x1d;
		buf[2]=0x18;
		buf[3]=0xc5;
		buf[4]=0xe1;
		buf[5]=0x17;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 58:
		size->setx(1);
		
		buf[0]=0x8;
		buf[1]=0x40;
		break;
	case 59:
		size->setx(2);
		
		buf[0]=0x0;
		buf[1]=0x40;
		buf[2]=0x16;
		buf[3]=0x0;
		break;
	case 60:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x0;
		buf[3]=0x9b;
		buf[4]=0x6;
		buf[5]=0x8;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 61:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0xf;
		buf[3]=0x80;
		buf[4]=0x1f;
		buf[5]=0x0;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 62:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x8;
		buf[3]=0x30;
		buf[4]=0x6c;
		buf[5]=0x80;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 63:
		size->setx(4);
		
		buf[0]=0x6;
		buf[1]=0x91;
		buf[2]=0x16;
		buf[3]=0x44;
		buf[4]=0x4;
		buf[5]=0x0;
		buf[6]=0x0;
		break;
	case 64:
		size->setx(10);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x1;
		buf[3]=0xf0;
		buf[4]=0x82;
		buf[5]=0x4e;
		buf[6]=0x64;
		buf[7]=0x99;
		buf[8]=0x26;
		buf[9]=0x59;
		buf[10]=0x8b;
		buf[11]=0xa0;
		buf[12]=0x4;
		buf[13]=0x0;
		buf[14]=0xf8;
		buf[15]=0x0;
		buf[16]=0x0;
		buf[17]=0x0;
		break;
	case 65:
		size->setx(7);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x40;
		buf[3]=0x82;
		buf[4]=0x85;
		buf[5]=0x11;
		buf[6]=0x3e;
		buf[7]=0x83;
		buf[8]=0x4;
		buf[9]=0x0;
		buf[10]=0x0;
		buf[11]=0x0;
		buf[12]=0x0;
		break;
	case 66:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x39;
		buf[2]=0x29;
		buf[3]=0x7a;
		buf[4]=0x31;
		buf[5]=0x8f;
		buf[6]=0x80;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 67:
		size->setx(6);
		
		buf[0]=0x0;
		buf[1]=0x3;
		buf[2]=0x91;
		buf[3]=0x82;
		buf[4]=0x8;
		buf[5]=0x20;
		buf[6]=0x44;
		buf[7]=0xe0;
		buf[8]=0x0;
		buf[9]=0x0;
		buf[10]=0x0;
		break;
	case 68:
		size->setx(6);
		
		buf[0]=0x0;
		buf[1]=0xf;
		buf[2]=0x22;
		buf[3]=0x86;
		buf[4]=0x18;
		buf[5]=0x61;
		buf[6]=0x8b;
		buf[7]=0xc0;
		buf[8]=0x0;
		buf[9]=0x0;
		buf[10]=0x0;
		break;
	case 69:
		size->setx(4);
		
		buf[0]=0x0;
		buf[1]=0xf8;
		buf[2]=0x8f;
		buf[3]=0x88;
		buf[4]=0x8f;
		buf[5]=0x0;
		buf[6]=0x0;
		break;
	case 70:
		size->setx(4);
		
		buf[0]=0x0;
		buf[1]=0xf8;
		buf[2]=0x8f;
		buf[3]=0x88;
		buf[4]=0x88;
		buf[5]=0x0;
		buf[6]=0x0;
		break;
	case 71:
		size->setx(6);
		
		buf[0]=0x0;
		buf[1]=0x3;
		buf[2]=0x91;
		buf[3]=0x82;
		buf[4]=0x9;
		buf[5]=0xe1;
		buf[6]=0x44;
		buf[7]=0xe0;
		buf[8]=0x0;
		buf[9]=0x0;
		buf[10]=0x0;
		break;
	case 72:
		size->setx(6);
		
		buf[0]=0x0;
		buf[1]=0x8;
		buf[2]=0x61;
		buf[3]=0x87;
		buf[4]=0xf8;
		buf[5]=0x61;
		buf[6]=0x86;
		buf[7]=0x10;
		buf[8]=0x0;
		buf[9]=0x0;
		buf[10]=0x0;
		break;
	case 73:
		size->setx(1);
		
		buf[0]=0x3f;
		buf[1]=0xc0;
		break;
	case 74:
		size->setx(3);
		
		buf[0]=0x0;
		buf[1]=0x92;
		buf[2]=0x49;
		buf[3]=0x38;
		buf[4]=0x0;
		buf[5]=0x0;
		break;
	case 75:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x23;
		buf[2]=0x2a;
		buf[3]=0x62;
		buf[4]=0x92;
		buf[5]=0x94;
		buf[6]=0x40;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 76:
		size->setx(4);
		
		buf[0]=0x0;
		buf[1]=0x88;
		buf[2]=0x88;
		buf[3]=0x88;
		buf[4]=0x8f;
		buf[5]=0x0;
		buf[6]=0x0;
		break;
	case 77:
		size->setx(9);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x30;
		buf[3]=0x78;
		buf[4]=0x3a;
		buf[5]=0x2d;
		buf[6]=0x16;
		buf[7]=0x53;
		buf[8]=0x29;
		buf[9]=0x88;
		buf[10]=0xc4;
		buf[11]=0x40;
		buf[12]=0x0;
		buf[13]=0x0;
		buf[14]=0x0;
		buf[15]=0x0;
		break;
	case 78:
		size->setx(6);
		
		buf[0]=0x0;
		buf[1]=0xc;
		buf[2]=0x71;
		buf[3]=0xa6;
		buf[4]=0x99;
		buf[5]=0x65;
		buf[6]=0x8e;
		buf[7]=0x30;
		buf[8]=0x0;
		buf[9]=0x0;
		buf[10]=0x0;
		break;
	case 79:
		size->setx(7);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0xe2;
		buf[3]=0x28;
		buf[4]=0x30;
		buf[5]=0x60;
		buf[6]=0xc1;
		buf[7]=0x44;
		buf[8]=0x70;
		buf[9]=0x0;
		buf[10]=0x0;
		buf[11]=0x0;
		buf[12]=0x0;
		break;
	case 80:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x3d;
		buf[2]=0x18;
		buf[3]=0xc7;
		buf[4]=0xd0;
		buf[5]=0x84;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 81:
		size->setx(8);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x38;
		buf[3]=0x44;
		buf[4]=0x82;
		buf[5]=0x82;
		buf[6]=0x82;
		buf[7]=0x82;
		buf[8]=0x44;
		buf[9]=0x3c;
		buf[10]=0x3;
		buf[11]=0x0;
		buf[12]=0x0;
		buf[13]=0x0;
		break;
	case 82:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x3d;
		buf[2]=0x18;
		buf[3]=0xc7;
		buf[4]=0xd2;
		buf[5]=0x8c;
		buf[6]=0x40;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 83:
		size->setx(4);
		
		buf[0]=0x0;
		buf[1]=0x69;
		buf[2]=0x84;
		buf[3]=0x21;
		buf[4]=0x96;
		buf[5]=0x0;
		buf[6]=0x0;
		break;
	case 84:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x3e;
		buf[2]=0x42;
		buf[3]=0x10;
		buf[4]=0x84;
		buf[5]=0x21;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 85:
		size->setx(6);
		
		buf[0]=0x0;
		buf[1]=0x8;
		buf[2]=0x61;
		buf[3]=0x86;
		buf[4]=0x18;
		buf[5]=0x61;
		buf[6]=0x85;
		buf[7]=0xe0;
		buf[8]=0x0;
		buf[9]=0x0;
		buf[10]=0x0;
		break;
	case 86:
		size->setx(7);
		
		buf[0]=0x0;
		buf[1]=0x2;
		buf[2]=0xc;
		buf[3]=0x14;
		buf[4]=0x48;
		buf[5]=0x8a;
		buf[6]=0x14;
		buf[7]=0x10;
		buf[8]=0x20;
		buf[9]=0x0;
		buf[10]=0x0;
		buf[11]=0x0;
		buf[12]=0x0;
		break;
	case 87:
		size->setx(11);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x2;
		buf[3]=0x10;
		buf[4]=0xc2;
		buf[5]=0x14;
		buf[6]=0xa4;
		buf[7]=0x94;
		buf[8]=0x94;
		buf[9]=0x52;
		buf[10]=0x8a;
		buf[11]=0x20;
		buf[12]=0x84;
		buf[13]=0x10;
		buf[14]=0x0;
		buf[15]=0x0;
		buf[16]=0x0;
		buf[17]=0x0;
		buf[18]=0x0;
		buf[19]=0x0;
		break;
	case 88:
		size->setx(7);
		
		buf[0]=0x0;
		buf[1]=0x2;
		buf[2]=0xa;
		buf[3]=0x22;
		buf[4]=0x82;
		buf[5]=0x4;
		buf[6]=0x14;
		buf[7]=0x45;
		buf[8]=0x4;
		buf[9]=0x0;
		buf[10]=0x0;
		buf[11]=0x0;
		buf[12]=0x0;
		break;
	case 89:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x23;
		buf[2]=0x15;
		buf[3]=0x28;
		buf[4]=0x84;
		buf[5]=0x21;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 90:
		size->setx(6);
		
		buf[0]=0x0;
		buf[1]=0xf;
		buf[2]=0xc1;
		buf[3]=0x8;
		buf[4]=0x42;
		buf[5]=0x10;
		buf[6]=0x83;
		buf[7]=0xf0;
		buf[8]=0x0;
		buf[9]=0x0;
		buf[10]=0x0;
		break;
	case 91:
		size->setx(2);
		
		buf[0]=0x3a;
		buf[1]=0xaa;
		buf[2]=0xab;
		buf[3]=0x0;
		break;
	case 92:
		size->setx(5);
		
		buf[0]=0x4;
		buf[1]=0x20;
		buf[2]=0x84;
		buf[3]=0x10;
		buf[4]=0x84;
		buf[5]=0x10;
		buf[6]=0x82;
		buf[7]=0x10;
		buf[8]=0x0;
		break;
	case 93:
		size->setx(2);
		
		buf[0]=0x35;
		buf[1]=0x55;
		buf[2]=0x57;
		buf[3]=0x0;
		break;
	case 94:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x8;
		buf[2]=0xa5;
		buf[3]=0x46;
		buf[4]=0x20;
		buf[5]=0x0;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 95:
		size->setx(6);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x0;
		buf[3]=0x0;
		buf[4]=0x0;
		buf[5]=0x0;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x3f;
		buf[9]=0x0;
		buf[10]=0x0;
		break;
	case 96:
		size->setx(2);
		
		buf[0]=0x24;
		buf[1]=0x0;
		buf[2]=0x0;
		buf[3]=0x0;
		break;
	case 97:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0xf;
		buf[3]=0x5;
		buf[4]=0xf1;
		buf[5]=0x8b;
		buf[6]=0xc0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 98:
		size->setx(5);
		
		buf[0]=0x4;
		buf[1]=0x21;
		buf[2]=0xf;
		buf[3]=0x46;
		buf[4]=0x31;
		buf[5]=0x8f;
		buf[6]=0x80;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 99:
		size->setx(4);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x78;
		buf[3]=0x88;
		buf[4]=0x87;
		buf[5]=0x0;
		buf[6]=0x0;
		break;
	case 100:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x42;
		buf[2]=0x17;
		buf[3]=0xc6;
		buf[4]=0x31;
		buf[5]=0x8b;
		buf[6]=0xc0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 101:
		size->setx(4);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x69;
		buf[3]=0xf8;
		buf[4]=0x87;
		buf[5]=0x0;
		buf[6]=0x0;
		break;
	case 102:
		size->setx(4);
		
		buf[0]=0x3;
		buf[1]=0x44;
		buf[2]=0xf4;
		buf[3]=0x44;
		buf[4]=0x44;
		buf[5]=0x0;
		buf[6]=0x0;
		break;
	case 103:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x7;
		buf[3]=0xca;
		buf[4]=0x4c;
		buf[5]=0x83;
		buf[6]=0xa3;
		buf[7]=0xe0;
		buf[8]=0x0;
		break;
	case 104:
		size->setx(5);
		
		buf[0]=0x4;
		buf[1]=0x21;
		buf[2]=0xb;
		buf[3]=0x66;
		buf[4]=0x31;
		buf[5]=0x8c;
		buf[6]=0x40;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 105:
		size->setx(1);
		
		buf[0]=0x2f;
		buf[1]=0xc0;
		break;
	case 106:
		size->setx(3);
		
		buf[0]=0x0;
		buf[1]=0x82;
		buf[2]=0x49;
		buf[3]=0x24;
		buf[4]=0xe0;
		buf[5]=0x0;
		break;
	case 107:
		size->setx(4);
		
		buf[0]=0x8;
		buf[1]=0x88;
		buf[2]=0x9a;
		buf[3]=0xca;
		buf[4]=0xa9;
		buf[5]=0x0;
		buf[6]=0x0;
		break;
	case 108:
		size->setx(1);
		
		buf[0]=0x7f;
		buf[1]=0xc0;
		break;
	case 109:
		size->setx(8);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x0;
		buf[3]=0x0;
		buf[4]=0xb6;
		buf[5]=0xc9;
		buf[6]=0x89;
		buf[7]=0x89;
		buf[8]=0x89;
		buf[9]=0x89;
		buf[10]=0x0;
		buf[11]=0x0;
		buf[12]=0x0;
		buf[13]=0x0;
		break;
	case 110:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0xb;
		buf[3]=0x66;
		buf[4]=0x31;
		buf[5]=0x8c;
		buf[6]=0x40;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 111:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x7;
		buf[3]=0x46;
		buf[4]=0x31;
		buf[5]=0x8b;
		buf[6]=0x80;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 112:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0xf;
		buf[3]=0x46;
		buf[4]=0x31;
		buf[5]=0x8f;
		buf[6]=0xa1;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 113:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x7;
		buf[3]=0xc6;
		buf[4]=0x31;
		buf[5]=0x8b;
		buf[6]=0xc2;
		buf[7]=0x10;
		buf[8]=0x0;
		break;
	case 114:
		size->setx(4);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0xbc;
		buf[3]=0x88;
		buf[4]=0x88;
		buf[5]=0x0;
		buf[6]=0x0;
		break;
	case 115:
		size->setx(4);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x78;
		buf[3]=0xc3;
		buf[4]=0x1e;
		buf[5]=0x0;
		buf[6]=0x0;
		break;
	case 116:
		size->setx(4);
		
		buf[0]=0x0;
		buf[1]=0x44;
		buf[2]=0xf4;
		buf[3]=0x44;
		buf[4]=0x43;
		buf[5]=0x0;
		buf[6]=0x0;
		break;
	case 117:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x8;
		buf[3]=0xc6;
		buf[4]=0x31;
		buf[5]=0x9b;
		buf[6]=0x40;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 118:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x8;
		buf[3]=0xc5;
		buf[4]=0x4a;
		buf[5]=0x21;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 119:
		size->setx(9);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x0;
		buf[3]=0x0;
		buf[4]=0x8;
		buf[5]=0x8c;
		buf[6]=0x45;
		buf[7]=0x54;
		buf[8]=0xaa;
		buf[9]=0x22;
		buf[10]=0x11;
		buf[11]=0x0;
		buf[12]=0x0;
		buf[13]=0x0;
		buf[14]=0x0;
		buf[15]=0x0;
		break;
	case 120:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x8;
		buf[3]=0xa8;
		buf[4]=0x84;
		buf[5]=0x54;
		buf[6]=0x40;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	case 121:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0x8;
		buf[3]=0xc5;
		buf[4]=0x4a;
		buf[5]=0x21;
		buf[6]=0x10;
		buf[7]=0x80;
		buf[8]=0x0;
		break;
	case 122:
		size->setx(3);
		
		buf[0]=0x0;
		buf[1]=0xe;
		buf[2]=0x52;
		buf[3]=0x9c;
		buf[4]=0x0;
		buf[5]=0x0;
		break;
	case 123:
		size->setx(3);
		
		buf[0]=0x5;
		buf[1]=0x24;
		buf[2]=0xa2;
		buf[3]=0x49;
		buf[4]=0x10;
		buf[5]=0x0;
		break;
	case 124:
		size->setx(1);
		
		buf[0]=0x7f;
		buf[1]=0xf0;
		break;
	case 125:
		size->setx(3);
		
		buf[0]=0x11;
		buf[1]=0x24;
		buf[2]=0x8a;
		buf[3]=0x49;
		buf[4]=0x40;
		buf[5]=0x0;
		break;
	case 126:
		size->setx(5);
		
		buf[0]=0x0;
		buf[1]=0x0;
		buf[2]=0xdb;
		buf[3]=0x0;
		buf[4]=0x0;
		buf[5]=0x0;
		buf[6]=0x0;
		buf[7]=0x0;
		buf[8]=0x0;
		break;
	default:
		return FONT_CHARACTER_DOES_NOT_EXIST;
	}
	size->sety(14);
	return FONT_CHARACTER_EXISTS;
}
