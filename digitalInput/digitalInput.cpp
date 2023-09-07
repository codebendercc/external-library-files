/*
 * pulsante.cpp
 *
 *  Created on: 11/giu/2011
 *      Author: fabio
 *
 *	Classe per gestire un ingresso digiale HIGH/LOW
 *
  */

/*! \mainpage Classe digitalInput per Arduino
*
* \section intro_sec Introduzione
*
* Permette di gestire pi&ugrave; facilmente un input pin anche con l'operatore '==', di verificarne il cambio di stato e altro
*
* \section usage_sec Utilizzo
*
* @code
*
digitalInput inTest(8,true); // pulsante collegato tra il pin 8 e gnd (attiva anche la resistenza di pull-up)
pinMode(13, OUTPUT); // Led Pin
void setup()
{
  	Serial.begin(9600);
}

void loop()
{
	if (inTest == LOW) {
 		digitalWrite(13,HIGH); accende il led
 	}
 	else {
 		digitalWrite(13,LOW); spegne il led
 	}

	if (inTest.isChanged()) {
		Serial.print("Stato cambiato in ");
		Serial.println(inTest);
	}
}
* @endcode
*/

#include "digitalInput.h"

//! Costruttore 1
/**
 * @param digitalPin	->	pin da utilizzare come input
 */
digitalInput::digitalInput(byte digitalPin)
{
	this->pin=digitalPin;
	statoPin=0;
	statoPinPrec=0;
	pinMode(this->pin,INPUT);
}

//! Costruttore 2
/**
 * @param digitalPin	->	pin da utilizzare come input
 * @param pullUp		->	se true viene attivata la resistena interna di pull-up
 */
digitalInput::digitalInput(byte digitalPin, bool pullUp)
{
	this->pin=digitalPin;
	statoPin=0;
	statoPinPrec=0;
	pinMode(this->pin,INPUT_PULLUP);
}

//! Uso interno: verifica se lo stato del pin &egrave; cambiato rispetto ultimo controllo
/**
 * @param stato	->	lo stato da verificare: LOW o HIGH
 * @return
 * true		se &egrave; cambiato
 * false	se non &egrave; cambiato
 */
bool digitalInput::isChangedTo(int stato)
{
	bool ret=false;

	statoPin = digitalRead(pin);
	if (statoPin != statoPinPrec){
		if (statoPin == HIGH){
			if (statoPin == stato) ret=true;
			else ret=false;
		}
		else{
			if (statoPin == stato) ret=true;
			else ret=false;
		}
	}
	statoPinPrec=statoPin;
	return ret;
}

//! Attiva la resistenza interna di pull-up
void digitalInput::PullUp(void)
{
	digitalWrite(pin,HIGH);
}

//! Verifica se lo stato del pin &egrave; cambiato rispetto all'ultimo controllo
/**
 * @return
 * true		se &egrave; cambiato
 * false	se &egrave; non &egrave; cambiato
 */
bool digitalInput::isChanged(void)
{
	if(isChangedTo(LOW) || isChangedTo(HIGH)) return true;
	else return false;
}

//! Verifica se lo stato del pin &egrave; cambiato da HIGH in LOW rispetto all'ultimo controllo
/**
 * @return
 * true		se &egrave; cambiato in LOW
 * false	se &egrave; non &egrave; cambiato
 */
bool digitalInput::isChangedToLow(void)
{
	return(isChangedTo(LOW));
}

//! Verifica se lo stato del pin &egrave; cambiato da LOW in HIGH rispetto all'ultimo controllo
/**
 * @return
 * true		se &egrave; cambiato in HIGH
 * false	se &egrave; non &egrave; cambiato
 */
bool digitalInput::isChangedToHigh(void)
{
	return(isChangedTo(HIGH));
}

//! Legge lo stato del pin
/**
 * @return lo stato logico dell'ingresso: HIGH o LOW
 */
int digitalInput::read()
{
    return digitalRead(pin);
}

//! overload operatore ==
digitalInput::operator int()
{
	return read();
}
