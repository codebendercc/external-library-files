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

#include <avr/interrupt.h>
#include <Arduino.h> 

#include "MINISMARTGPU.h"

// MINI SMART GPU DEFAULT BAUD RATE: 9600bps
/****************************************************************/
//Arduino exclusive Functions
/****************************************************************/
MINISMARTGPU::MINISMARTGPU(){
	init();
}

void MINISMARTGPU::init(){      //configure the arduino board for MINISMARTGPU board
	Serial.begin (9600);	
	pinMode(TX,OUTPUT);
	pinMode(RX,INPUT);
	pinMode(RESET, OUTPUT); 
	digitalWrite(RESET, HIGH);   // set the pin to 5v to avoid reset 
}
 
void MINISMARTGPU::reset(){          //Reset the MINISMARTGPU board
	digitalWrite(RESET, LOW);    // set the pin to GND to reset 
	delay(500);
	digitalWrite(RESET, HIGH);   // set the pin to 5v to end reset
	delay(500);	
}

unsigned char MINISMARTGPU::start(){       //Init the MINISMARTGPU
  delay(500); 
  Serial.write('U');  
  while(Serial.available() == 0); 
  delay(2000);
  return Serial.read();
}


/****************************************************************/
//Master Functions
/****************************************************************/
unsigned char MINISMARTGPU::erase(){       //Erase the MINISMARTGPU screen
  Serial.write('M');             //Master function
  Serial.write('E'); 
  while(Serial.available() == 0);  
  return Serial.read();
}

unsigned char MINISMARTGPU::sleep(unsigned char mode){       //Send MINISMARTGPU to sleep mode
  Serial.write('M');             //Master function
  Serial.write('Z'); 
  Serial.write(mode);
  while(Serial.available() == 0);  
  return Serial.read();
}

unsigned char MINISMARTGPU::orientation(unsigned char side){       //Change display orientation
  Serial.write('M');             //Master function
  Serial.write('O'); 
  Serial.write(side);
  while(Serial.available() == 0);  
  return Serial.read();
}

unsigned char MINISMARTGPU::bright(unsigned char val){       //Change display brightness
  Serial.write('M');             //Master function
  Serial.write('B'); 
  Serial.write(val);
  while(Serial.available() == 0);  
  return Serial.read();
}

// mini SMART GPU DEFAULT/INITIAL BAUD RATE: 9600bps
unsigned char MINISMARTGPU::baudChange(unsigned long val){       //Change baud rate of arduino and MINISMARTGPU board
  unsigned char aux;
 
  switch(val){
	case 9600:
	  aux=0;
	break;
	case 19200:
	  aux=1;
	break;
	case 57600:
	  aux=2;
	break;
	case 115200:
	  aux=3;
	break;
	case 256000:
	  aux=4;
	break;	
	case 500000:
	  aux=5;
	break;
	case 1000000:
	  aux=6;
	break;
	case 2000000:
	  aux=7;
	break;
	default:
	  return 'F';
	break;
  } 
  Serial.write('M');             //Master function    
  Serial.write('X');
  Serial.write(aux);
  while(Serial.available() == 0);  
  aux=Serial.read();
  if(aux=='O'){
	Serial.end();
	delay(150);
	Serial.begin(val);
	delay(200);
	while(Serial.available() == 0);  
	return Serial.read();
  }else{
	return aux;
  }
}

unsigned char MINISMARTGPU::setEraseBackColour(unsigned int colour){       //Change the default screen background colour for erase function
  Serial.write('M');             //Master function
  Serial.write('C');             //Background Colour
  Serial.write(colour>>8);
  Serial.write(colour); 
  while(Serial.available() == 0);  
  return Serial.read();
}


/****************************************************************/
//Geometric Functions
/****************************************************************/
unsigned char MINISMARTGPU::putPixel(unsigned char x, unsigned char y, unsigned int colour){       //Draw a pixel on the screen
  Serial.write('G');             //Geometric function  
  Serial.write('P'); 
  Serial.write(x);
  Serial.write(y);
  Serial.write(colour>>8);
  Serial.write(colour);
  while(Serial.available() == 0);  
  return Serial.read();
}

unsigned char MINISMARTGPU::drawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned int colour){       //Draw a line on the screen
  Serial.write('G');             //Geometric function 
  Serial.write('L'); 
  Serial.write(x1);
  Serial.write(y1);
  Serial.write(x2);
  Serial.write(y2);  
  Serial.write(colour>>8);
  Serial.write(colour);
  while(Serial.available() == 0);  
  return Serial.read();
}

unsigned char MINISMARTGPU::drawRectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned int colour, unsigned char fill){       //Draw a rectangle on the screen
  Serial.write('G');             //Geometric function 
  Serial.write('R'); 
  Serial.write(x1);
  Serial.write(y1);
  Serial.write(x2);
  Serial.write(y2);  
  Serial.write(colour>>8);
  Serial.write(colour);
  Serial.write(fill);  
  while(Serial.available() == 0);  
  return Serial.read();
}

unsigned char MINISMARTGPU::drawTriangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char x3, unsigned char y3, unsigned int colour, unsigned char fill){       //Draw a triangle on the screen
  Serial.write('G');             //Geometric function 
  Serial.write('T'); 
  Serial.write(x1);
  Serial.write(y1);
  Serial.write(x2);
  Serial.write(y2);  
  Serial.write(x3);
  Serial.write(y3);    
  Serial.write(colour>>8);
  Serial.write(colour);
  Serial.write(fill);  
  while(Serial.available() == 0);  
  return Serial.read();
}

unsigned char MINISMARTGPU::drawCircle(unsigned char x, unsigned char y, unsigned char radius, unsigned int colour, unsigned char fill){       //Draw a circle on the screen
  Serial.write('G');             //Geometric function 
  Serial.write('C');  
  Serial.write(x);
  Serial.write(y);
  Serial.write(radius);
  Serial.write(colour>>8);
  Serial.write(colour);
  Serial.write(fill);  
  while(Serial.available() == 0);  
  return Serial.read();
}


/****************************************************************/
//String Functions
/****************************************************************/
unsigned char MINISMARTGPU::putLetter(unsigned char x, unsigned char y, char letter){  //Draw a letter on the screen on X,Y coords
  Serial.write('S');             //String Function 
  Serial.write('L');             //Letter - a simple letter 
  Serial.write(x);
  Serial.write(y);
  Serial.write(letter); 
  while(Serial.available() == 0);  
  return Serial.read();
}

unsigned char MINISMARTGPU::string(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, char text[], unsigned int *SPB){    //Draw a string on the screen on defined Text Box coords, and stores the successfully printed bytes on SPB
  unsigned int counter=0, sp=0;  
  Serial.write('S');             //String Function 
  Serial.write('S');             //String  
  Serial.write(x1);
  Serial.write(y1); 
  Serial.write(x2);
  Serial.write(y2);   
  while(1){
	Serial.write(text[counter]);
    if(text[counter]==0x00){
      break;
	}	
	counter++;
  }  
  while(Serial.available() < 3);  
  ((unsigned char*) &sp)[1]=Serial.read();
  ((unsigned char*) &sp)[0]=Serial.read(); 

  *SPB = sp;      
  return Serial.read();
}

unsigned char MINISMARTGPU::stringSD(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned int BS, unsigned int BR, char name[], unsigned int *SPB){       //Draw a String from a text file stored on the micro SD card
  unsigned char counter=0, sp=0;
  
  Serial.write('S');             //String function 
  Serial.write('F');             //text File from SD 
  Serial.write(x1);
  Serial.write(y1);
  Serial.write(x2);
  Serial.write(y2);  
  Serial.write(BS>>8);
  Serial.write(BS);  
  Serial.write(BR>>8);
  Serial.write(BR);  
  while(1){
	Serial.write(name[counter]);
    if(name[counter]==0x00){
      break;
	}	
	counter++;
  }
  while(Serial.available() < 3);  
  ((unsigned char*) &sp)[1]=Serial.read();
  ((unsigned char*) &sp)[0]=Serial.read(); 

  *SPB = sp; 
  return Serial.read();
}

unsigned char MINISMARTGPU::setTextColour(unsigned int colour){        //Set the default text colour for letters and strings
  Serial.write('S');             //String Function 
  Serial.write('C');             //Config  
  Serial.write('T');             //Text
  Serial.write(colour>>8);
  Serial.write(colour); 
  while(Serial.available() == 0);  
  return Serial.read();
}

unsigned char MINISMARTGPU::setTextBackColour(unsigned int colour){    //Set the default text background colour for letters and strings
  Serial.write('S');             //String Function 
  Serial.write('C');             //Config  
  Serial.write('B');             //Background
  Serial.write(colour>>8);
  Serial.write(colour); 
  while(Serial.available() == 0);  
  return Serial.read();
}

unsigned char MINISMARTGPU::setTextSize(unsigned char size){           //Set the default text size for letters and strings
  Serial.write('S');             //String Function 
  Serial.write('C');             //Config  
  Serial.write('S');             //Size
  Serial.write(size); 
  while(Serial.available() == 0);  
  return Serial.read();
}

unsigned char MINISMARTGPU::setTextBackFill(unsigned char fillState){  //Set the default text FILL or UNFILL background letters and strings
  Serial.write('S');             //String Function 
  Serial.write('C');             //Config  
  Serial.write('F');             //Fill
  Serial.write(fillState); 
  while(Serial.available() == 0);  
  return Serial.read();
}


/****************************************************************/
//Image Functions
/****************************************************************/
unsigned char MINISMARTGPU::drawIcon(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, char icon[]){            //Send and image or icon pixel by pixel to SMARTGPU, 16bit(2 bytes) each pixel RGB565
  unsigned int i,j,k=0; 
  
  Serial.write('I');             //Image function 
  Serial.write('I');             //Icon image received pixel by pixel
  Serial.write(x1);
  Serial.write(y1);
  Serial.write(x2);
  Serial.write(y2);
  
  //Send icon buffer pixel by pixel
  for(j=0;j<=(y2-y1);j++){
	for(i=0;i<=(x2-x1);i++){
		Serial.write(icon[k++]); //16bit per pixel - Upper 8bits part
		Serial.write(icon[k++]); //16bit per pixel - Lower 8bits part
	}
  }
  while(Serial.available() == 0);  
  return Serial.read();  
}

unsigned char MINISMARTGPU::imageBMPSD(unsigned char x, unsigned char y, char name[]){        //Draw an Image stored on the micro SD card on the screen, at X,Y top right corner coordinates
  unsigned char counter=0;
  
  Serial.write('I');             //Image function 
  Serial.write('B');             //BMP from SD card
  Serial.write(x);
  Serial.write(y);
  while(1){
	Serial.write(name[counter]);
    if(name[counter]==0x00){
      break;
	}	
	counter++;
  }
  while(Serial.available() == 0);  
  return Serial.read();
}

unsigned char MINISMARTGPU::getImageFromMemory(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, char buffer[]){ //Read the internal memory of the SMARTGPU, This command returns 24bit pixels (3 bytes)
  unsigned int i,j,k=0;
  
  Serial.write('I');             //Image function
  Serial.write('M');             //from SmartGPU internal Display Memory
  Serial.write(x1);
  Serial.write(y1);
  Serial.write(x2);
  Serial.write(y2); 

  while(Serial.available() == 0);//wait for pixels
  //receive all the pixels
  for(j=0;j<=(y2-y1);j++){
	for(i=0;i<=(x2-x1);i++){
		while(Serial.available() < 3);	 //wait to receive R, G, B
		buffer[k++]=Serial.read(); //Red
		buffer[k++]=Serial.read(); //Green		
		buffer[k++]=Serial.read(); //Blue
	}	
  }
  while(Serial.available() == 0);  
  return Serial.read();  
}


/****************************************************************/
//SD FAT management Functions
//Those next SDF - SD Functions return file execution status instead of ACK 'O' or NAK 'F'
/****************************************************************/
unsigned char MINISMARTGPU::SDFgetList(unsigned int *numOfFiles){ //get number of files
  unsigned char  aux=0;
  unsigned int files=0;

  //delay(1);      
  Serial.write('F');           //File function - memory card file management
  Serial.write('L');           //List/count files
  while(Serial.available() < 4);
  ((unsigned char *) &files)[1]= Serial.read();      //Get Upper part
  ((unsigned char *) &files)[0]= Serial.read();      //Get Lower part  

  *numOfFiles=files;       
  
  aux= Serial.read();          //Get SD file execution status
  Serial.read();               //Discard 'O' or 'F'
  return aux;                  //Return SD file execution status
}

unsigned char MINISMARTGPU::SDFgetFileName(unsigned int itemNumber, char name[]){ //searches for the "itemNumber" on the SD current folder and updates the buffer with the File name ended with NULL character
  unsigned char  aux=0;

  //delay(1);      
  Serial.write('F');           //File function - memory card file management
  Serial.write('G');           //Get name of given item file number
  Serial.write(itemNumber>>8); //Send Upper part of itemNumber
  Serial.write(itemNumber);    //Send Lower part of itemNumber
  
  aux=Serial.readBytesUntil(0x00,name,255); //if we find NULL character, means end of name  
  name[aux]=0x00;                           //Add the null character
  
  while(Serial.available() < 2);  
  aux= Serial.read();          //Get SD file execution status
  Serial.read();               //Discard 'O' or 'F'
  return aux;                  //Return SD file execution status
 }
 
unsigned char MINISMARTGPU::SDFnewFile(char name[]){ //create a new File, fails if already exist
  unsigned char aux = 0;
  unsigned int counter=0;  
 
  Serial.write('F');           //File function - memory card file management
  Serial.write('N');           //New

  while(1){
  	Serial.write(name[counter]);
  	if(name[counter]==0x00){
  	break;
  	}	
  	counter++;
  }

  while(Serial.available() < 2);  
  aux= Serial.read();          //Get SD file execution status
  Serial.read();               //Discard 'O' or 'F'
  return aux;                  //Return SD file execution status
}

unsigned char MINISMARTGPU::SDFopenFile(char name[], unsigned char mode){ //opens an existing file in READONLY, WRITEONLY or READWRITE mode
  unsigned char aux = 0;
  unsigned int counter=0;  
  
	Serial.write('F');           //File function - memory card file management
	Serial.write('O');           //Open file
	Serial.write(mode);          //Mode - READONLY,WRITEONLY,READWRITE
	while(1){
		Serial.write(name[counter]);
		if(name[counter]==0x00){
		break;
		}	
		counter++;
	}
	
	while(Serial.available() < 2);  
	aux= Serial.read();          //Get SD file execution status
	Serial.read();               //Discard 'O' or 'F'
    return aux;                  //Return SD file execution status
  
}

unsigned char MINISMARTGPU::SDFcloseFile(){ //close and save file object
  unsigned char aux = 0;

  //delay(50);
  Serial.write('F');           //File function - memory card file management
  Serial.write('C');           //Close File
  
  while(Serial.available() < 2);
  aux= Serial.read();          //Get SD file execution status
  Serial.read();               //Discard 'O' or 'F'
  return aux;                  //Return SD file execution status
}

unsigned char MINISMARTGPU::SDFsaveFile(){ //sync/save file object
  unsigned char aux = 0;
  
  Serial.write('F');           //File function - memory card file management
  Serial.write('S');           //Save/Sync file - Save changes on file
  
  while(Serial.available() < 2);
  aux= Serial.read();          //Get SD file execution status
  Serial.read();               //Discard 'O' or 'F'
  return aux;                  //Return SD file execution status 
}

unsigned char MINISMARTGPU::SDFsetFilePointer(unsigned long pointerPosition){ // set/move file pointer of file object
  unsigned char aux = 0;
  
  Serial.write('F');           //File function - memory card file management
  Serial.write('P');           //Pointer position
  Serial.write('S');           //Set  
  Serial.write(pointerPosition>>24);
  Serial.write(pointerPosition>>16);
  Serial.write(pointerPosition>>8);
  Serial.write(pointerPosition);
   
  while(Serial.available() < 2);
  aux= Serial.read();          //Get SD file execution status
  Serial.read();               //Discard 'O' or 'F'
  return aux;                  //Return SD file execution status 
}	

unsigned char MINISMARTGPU::SDFgetFilePointer(unsigned long *pointerPosition){ // get file pointer of file object
  unsigned char aux = 0;
  unsigned long pos = 0;
  
  Serial.write('F');           //File function - memory card file management
  Serial.write('P');           //Pointer position 
  Serial.write('G');           //Get
 
  while(Serial.available() < 6);  
  ((unsigned char *) &pos)[3]=Serial.read();
  ((unsigned char *) &pos)[2]=Serial.read();
  ((unsigned char *) &pos)[1]=Serial.read();  
  ((unsigned char *) &pos)[0]=Serial.read(); 

  *pointerPosition =pos;
  
  aux= Serial.read();          //Get SD file execution status
  Serial.read();               //Discard 'O' or 'F'
  return aux;                  //Return SD file execution status 
}	

unsigned char MINISMARTGPU::SDFreadFile(char buffer[], unsigned int BTR, unsigned int *SRB){ //Buffer, Bytes to Read, Succesfully Read Bytes
  unsigned char aux = 0;
  unsigned int sr=0;

  Serial.write('F');           //File function - memory card file management
  Serial.write('R');           //Read file
  Serial.write(BTR>>8);
  Serial.write(BTR);  

  Serial.readBytes(buffer,BTR); //Get BTR bytes
  
  while(Serial.available() < 4);  
  ((unsigned char*) &sr)[1]=Serial.read();
  ((unsigned char*) &sr)[0]=Serial.read(); 

  *SRB = sr;                   //store succesfully read bytes
  
  aux= Serial.read();          //Get SD file execution status
  Serial.read();               //Discard 'O' or 'F'
  return aux;                  //Return SD file execution status 
}	

unsigned char MINISMARTGPU::SDFwriteFile(char buffer[], unsigned int BTW, unsigned int *SWB){ //Buffer, Bytes to Write, Succesfully Written Bytes
  unsigned char aux = 0;
  unsigned int x=0, sw=0;

  if(BTW>512){
	*SWB=0;                   //Return 0 successfully written bytes
	return INVALID_PARAMETER; //512 bytes is the max to write in a single call
  }
  
  //delay(10);  
  Serial.write('F');           //File function - memory card file management
  Serial.write('W');           //Write file
  Serial.write(BTW>>8);
  Serial.write(BTW);    
  for(x=0;x<BTW;x++){
	Serial.write(buffer[x]);
  }
  
  while(Serial.available() < 4);  
  ((unsigned char*) &sw)[1]=Serial.read();
  ((unsigned char*) &sw)[0]=Serial.read();  

  *SWB = sw;                   //store succesfully written bytes   
  
  aux= Serial.read();          //Get SD file execution status
  Serial.read();               //Discard 'O' or 'F'
  return aux;                  //Return SD file execution status 
}

unsigned char MINISMARTGPU::SDFtruncateFile(){  //truncates the file size to the current file read/write pointer
  unsigned char aux=0;
  
  Serial.write('F');           //File function - memory card file management
  Serial.write('V');           //Truncate

  while(Serial.available() < 2);
  aux= Serial.read();          //Get SD file execution status
  Serial.read();               //Discard 'O' or 'F'
  return aux;                  //Return SD file execution status  
}

unsigned char MINISMARTGPU::SDFeraseFile(char name[]){ //Erases an existing File
  unsigned char aux = 0;
  unsigned int counter=0;  
  
	Serial.write('F');           //File function - memory card file management
	Serial.write('E');           //Erase Dir File	
	while(1){
		Serial.write(name[counter]);
		if(name[counter]==0x00){
		break;
		}	
		counter++;
	}
	
	while(Serial.available() < 2);  
	aux= Serial.read();          //Get SD file execution status
	Serial.read();               //Discard 'O' or 'F'
    return aux;                  //Return SD file execution status  
}

unsigned char MINISMARTGPU::SDFgetFileSize(char name[], unsigned long *fileSize){ //Get Size of an existing File
  unsigned char aux = 0;
  unsigned int counter=0;  
  unsigned long size=0;
  
	Serial.write('F');           //File function - memory card file management
	Serial.write('I');           //Info
	while(1){
		Serial.write(name[counter]);
		if(name[counter]==0x00){
		break;
		}	
		counter++;
	}	
	
  while(Serial.available() < 6); 	
  ((unsigned char *) &size)[3]=Serial.read();
  ((unsigned char *) &size)[2]=Serial.read();
  ((unsigned char *) &size)[1]=Serial.read();  
  ((unsigned char *) &size)[0]=Serial.read(); 
	
  *fileSize=size;
  
  aux= Serial.read();          //Get SD file execution status
  Serial.read();               //Discard 'O' or 'F'
  return aux;                  //Return SD file execution status  
}
