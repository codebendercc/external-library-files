/*********************************************************
VIZIC TECHNOLOGIES. COPYRIGHT 2013.
THE DATASHEETS, SOFTWARE AND LIBRARIES ARE PROVIDED "AS IS." 
VIZIC EXPRESSLY DISCLAIM ANY WARRANTY OF ANY KIND, WHETHER 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
OR NONINFRINGEMENT. IN NO EVENT SHALL VIZIC BE LIABLE FOR 
ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF 
PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, 
ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO 
ANY DEFENCE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION,
OR OTHER SIMILAR COSTS.
*********************************************************/

/********************************************************
 IMPORTANT : This library is created for the Arduino 1.0 Software IDE
********************************************************/

#ifndef MINISMARTGPU_h
#define MINISMARTGPU_h

#include <inttypes.h>

//Arduino - miniSmartGPU pin definitions
#define RX     0
#define TX     1
#define RESET  2

//General definitions
#define OFF 0
#define ON  1
#define GND 0
#define VCC 1

//Graphics functions definitions
#define LCDWIDTH  160
#define LCDHEIGHT 128
#define UNFILL 0
#define FILL   1
#define TRANS  0
#define COLOUR 1
#define LANDSCAPEL  0x00  //left
#define PORTRAITL   0x01  //low
#define LANDSCAPER  0x02  //right
#define PORTRAITT   0x03  //top
#define HORIZONTAL  0
#define VERTICAL    1
#define WIDTHLANDSCAPE  160
#define HEIGHTLANDSCAPE 128
#define WIDTHPORTRAIT   128
#define HEIGHTPORTRAIT  160

//basic colours definition
#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define YELLOW  0xFFE0
#define CYAN    0x07FF
#define MAGENTA 0xF81F

//fonts definition
#define FONT0   0x00
#define FONT1   0x01
#define FONT2   0x02
#define FONT3   0x03
#define FONT4   0x04
#define FONT5   0x05

//File access definitions
#define READONLY     0x01
#define WRITEONLY    0x02
#define READWRITE    0x03
#define BEGINNING    0
#define ALLCONTENTS  0

//miniSMARTGPU Command Execution responses definitions
#define OK                  0
#define DISK_ERROR          1
#define INT_ERROR           2
#define NOT_READY           3
#define NO_FILE             4
#define NO_PATH             5
#define INVALID_NAME        6
#define DENIED              7
#define ALREADY_EXIST       8
#define INVALID_OBJECT      9
#define WRITE_PROTECTED    10
#define INVALID_DRIVE      11
#define NOT_ENABLED        12
#define NO_FILE_SYSTEM     13
#define MAKE_FILE_ABORTED  14
#define TIMEOUT            15
#define LOCKED             16
#define NOT_ENOUGHT_CORE   17
#define TOO_MANY_FILES     18
#define INVALID_PARAMETER  19

class MINISMARTGPU{
	
public:
/****************************************************************/
//Arduino exclusive Functions
/****************************************************************/
    MINISMARTGPU();
	
	void init();

	void reset();
	
	unsigned char start();  

/****************************************************************/
//Master Functions
/****************************************************************/		
	unsigned char erase();
	
	unsigned char sleep(unsigned char);
	
	unsigned char orientation(unsigned char);
	
	unsigned char bright(unsigned char);
	
	unsigned char baudChange(unsigned long val);
	
	unsigned char setEraseBackColour(unsigned int);
	
/****************************************************************/
//Geometric Functions
/****************************************************************/			
	unsigned char putPixel(unsigned char, unsigned char, unsigned int);
	
	unsigned char drawLine(unsigned char, unsigned char, unsigned char, unsigned char, unsigned int);
	
	unsigned char drawRectangle(unsigned char, unsigned char, unsigned char, unsigned char, unsigned int, unsigned char);
	
	unsigned char drawTriangle(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned int, unsigned char);
	
	unsigned char drawCircle(unsigned char, unsigned char, unsigned char, unsigned int, unsigned char);
	
/****************************************************************/
//String Functions
/****************************************************************/				
	unsigned char putLetter(unsigned char, unsigned char, char);
	
	unsigned char string(unsigned char, unsigned char, unsigned char, unsigned char, char[], unsigned int*);
	
	unsigned char stringSD(unsigned char, unsigned char, unsigned char, unsigned char, unsigned int, unsigned int, char[], unsigned int*);
	
	unsigned char setTextColour(unsigned int);
	
	unsigned char setTextBackColour(unsigned int);

	unsigned char setTextSize(unsigned char);

	unsigned char setTextBackFill(unsigned char);	
	
/****************************************************************/
//Image Functions
/****************************************************************/			
	unsigned char drawIcon(unsigned char, unsigned char, unsigned char, unsigned char, char[]);
	
	unsigned char imageBMPSD(unsigned char, unsigned char, char[]);

	unsigned char getImageFromMemory(unsigned char, unsigned char, unsigned char, unsigned char, char[]); //Read the internal memory of the MINISMARTGPU, This command returns 24bit pixels (3 bytes)
	
/****************************************************************/
//SD FAT management Functions
//Those next SDF - SD Functions return file execution status instead of ACK 'O' or NAK 'F'
/****************************************************************/
	unsigned char SDFgetList(unsigned int*); //get number of files

	unsigned char SDFgetFileName(unsigned int, char[]); //searches for the "itemNumber" on the SD root path and updates the buffer with the File name ended with NULL character

	unsigned char SDFnewFile(char[]); //create a new File, fails if already exist

	unsigned char SDFopenFile(char[], unsigned char); //opens an existing file in READONLY, WRITEONLY or READWRITE mode

	unsigned char SDFcloseFile(); //close and save file
 
	unsigned char SDFsaveFile(); //sync/save file

	unsigned char SDFsetFilePointer(unsigned long); // set/move file pointer

	unsigned char SDFgetFilePointer(unsigned long*); // get file pointer

	unsigned char SDFreadFile(char[], unsigned int, unsigned int*); //Buffer, Bytes to Read, Succesfully Read Bytes

	unsigned char SDFwriteFile(char[], unsigned int, unsigned int*); //Buffer, Bytes to Write, Succesfully Written Bytes

	unsigned char SDFtruncateFile();  //truncates the file size to the current file read/write pointer

	unsigned char SDFeraseFile(char[]); //Erases an existing Dir or File

	unsigned char SDFgetFileSize(char[], unsigned long *); //Get Size of an existing File

};

#endif
