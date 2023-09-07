/*
  LPM11162.cpp - Arduino Library for LPM11162 audio module.
  Created by LP ELETTRONICA - www.lpelettronica.it, November 27, 2012.
  Version 2.1
  Designed for Arduino IDE 1.x.
  Released into the public domain.

  Ver 2.1
  - Bug fix: isBusy() was not working
  ******************************
  Ver 2.0
  - Improvement: supported Arduino IDE 1.x
  - Improvement: supported Bridge mode
*/

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "LPM11162.h"


#define TRL			    10 	//ms
#define TRA			    600 //ms
#define TAC			    150 //ms
#define AUTOBAUD	    0x55
#define CMD_PLAY	    0x10
#define CMD_STOP	    0x11
#define CMD_VOL		    0x12

#define ARDUINO_SYNCH	0xCC
#define BRIDGE_BAUDRATE 38400


 
LPM11162::LPM11162(uint8_t tx, uint8_t rx /* = LPM_NO_PIN */, uint8_t reset /* = LPM_NO_PIN */, uint8_t busy /* = LPM_NO_PIN */)
{
    SoftSerTx = tx;
    SoftSerRx = rx;
    pSoftSer = new SoftwareSerial(SoftSerRx, SoftSerTx);

    BusyPin = busy;
    if ( BusyPin != LPM_NO_PIN )
	{
 		pinMode(BusyPin, INPUT);  	// Initialize BUSY pin.
 	}

    ResetPin = reset;
	if ( ResetPin != LPM_NO_PIN )
	{
	    pinMode(ResetPin, OUTPUT); 	// Initialize RESET pin.
	    digitalWrite(ResetPin, LOW);
	}
}


LPM11162::~LPM11162()
{
}


void LPM11162::begin(uint32_t baudrate)
{
    pSoftSer->begin(baudrate);			// Initialize serial port with proper baudrate.

	if ( ResetPin != LPM_NO_PIN )
	{
	    reset(LOW); 	// Put LPM11162 in reset.
	    delay(TRL);     // Delay 10ms (tRL from datasheet).
	    reset(HIGH); 	// Remove LPM11162 from reset.
	}
	
    delay(TRA); 
    pSoftSer->write(AUTOBAUD); 	    // Send autobaudrate character 0x55.
  
    delay(TAC);           			// Delay 150ms (tAC from datasheet).
}


void LPM11162::end()
{
	pSoftSer->end();				// Close serial port.
	delete pSoftSer;				// Free memory allocated for serial port.
	pSoftSer = NULL;
}


void LPM11162::play(char* pFileName)
{
    pSoftSer->write(CMD_PLAY); 	        // Play (cmd 0x10 from datasheet).
    pSoftSer->write((byte)0x0);         // Cmd Len high.
    pSoftSer->write(strlen(pFileName));	// Cmd Len low.
    pSoftSer->write(pFileName);	        // Wav file name.
}


void LPM11162::stop()
{
    pSoftSer->write(CMD_STOP);  // Stop (cmd 0x11 from datasheet).
    pSoftSer->write((byte)0x0); // Cmd Len high.
    pSoftSer->write((byte)0x0); // Cmd Len low.
}


void LPM11162::volume(char volume)
{
    pSoftSer->write(CMD_VOL); 	// Volume (cmd 0x12 from datasheet).
    pSoftSer->write((byte)0);	// Cmd Len high.
    pSoftSer->write(1);		    // Cmd Len low.
    pSoftSer->write(volume);	// Volume level.
}


bool LPM11162::isBusy()
{
	if ( BusyPin != LPM_NO_PIN )
	{
		return !digitalRead(BusyPin);// Read busy pin.
	}
	else
	{
		return false; //Busy pin not used.
	}
}


void LPM11162::reset(bool resetLevel)
{
	if ( ResetPin != LPM_NO_PIN )
	{
		digitalWrite(ResetPin, resetLevel); 	// Drive LPM11162 reset.
	}
}


#define a_tx_pin_write(pin_state) \
    if (!(pin_state)) \
      *a_tx_reg &= ~a_tx_mask; \
    else \
      *a_tx_reg |= a_tx_mask;

#define a_rx_pin_read() (*a_rx_reg & a_rx_mask)

#define b_tx_pin_write(pin_state) \
    if (!(pin_state)) \
      *b_tx_reg &= ~b_tx_mask; \
    else \
      *b_tx_reg |= b_tx_mask;

#define b_rx_pin_read() (*b_rx_reg & b_rx_mask)

void LPM11162::loop(uint8_t a_rx, uint8_t a_tx, uint8_t b_rx, uint8_t b_tx)
{
  uint8_t a_rx_mask;
  volatile uint8_t *a_rx_reg;
  uint8_t a_tx_mask;
  volatile uint8_t *a_tx_reg;

  uint8_t b_rx_mask;
  volatile uint8_t *b_rx_reg;
  uint8_t b_tx_mask;
  volatile uint8_t *b_tx_reg;

  pinMode(a_rx, INPUT);
  digitalWrite(a_rx, HIGH);
  pinMode(a_tx, OUTPUT);
  digitalWrite(a_tx, HIGH);

  pinMode(b_rx, INPUT);
  digitalWrite(b_rx, HIGH);
  pinMode(b_tx, OUTPUT);
  digitalWrite(b_tx, HIGH);

  a_rx_mask = digitalPinToBitMask(a_rx);
  a_rx_reg = portInputRegister(digitalPinToPort(a_rx));
  a_tx_mask = digitalPinToBitMask(a_tx);
  a_tx_reg = portOutputRegister(digitalPinToPort(a_tx));

  b_rx_mask = digitalPinToBitMask(b_rx);
  b_rx_reg = portInputRegister(digitalPinToPort(b_rx));
  b_tx_mask = digitalPinToBitMask(b_tx);
  b_tx_reg = portOutputRegister(digitalPinToPort(b_tx));
  
  for (;;)
  {
    b_tx_pin_write(a_rx_pin_read());
    a_tx_pin_write(b_rx_pin_read());
  }
}


void LPM11162::bridge()
{
    if ( !pSoftSer )
        return;

    if ( ResetPin == LPM_NO_PIN )
        return;
  
    pSoftSer->begin(BRIDGE_BAUDRATE);

    reset(HIGH); 	// Remove LPM11162 from reset.
    delay(100);
    reset(LOW); 	// Put LPM11162 in reset.
    delay(10);      // Delay 10ms (tRL from datasheet).
    reset(HIGH); 	// Remove LPM11162 from reset.

    Serial.begin(BRIDGE_BAUDRATE);
    while (!Serial); // wait for serial port to connect. Needed for Leonardo only

    while(1) 
    {
        if (Serial.available())
        {
            if (Serial.read() == AUTOBAUD)
                break;
            Serial.flush();
        }
    }
    
    Serial.end();

    pSoftSer->write(AUTOBAUD);
    pSoftSer->end();

    loop( SoftSerRx, SoftSerTx, 0, 1 ); //a_rx, a_tx, b_rx, b_tx
}


bool LPM11162::synch()
{
    int i;
    
    Serial.begin(9600);
    while (!Serial); // wait for serial port to connect. Needed for Leonardo only


    for( i=0; i<20; i++ )
    {
        if (Serial.available())
        {
            if (Serial.read() == ARDUINO_SYNCH)
            {
                Serial.write(ARDUINO_SYNCH);
                Serial.end();
                return true;
            }
            Serial.flush();
        }
        delay(10);
    }
    
    Serial.end();
    return false;
}