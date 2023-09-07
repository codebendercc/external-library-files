/*
 * digitalInput.h
 *
 *  Created on: 11/giu/2011
 *      Author: fabio
 */

#ifndef digitalInput_H_
#define digitalInput_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "wiring.h"
	#include "WProgram.h"
#endif

//! Gestisce un ingresso digitale compreso il cambio di stato
class digitalInput{

	public:
	digitalInput(byte digitalPin);
	digitalInput(byte digitalPin, bool pullUp);
	void PullUp(void);
	bool isChanged(void);
	bool isChangedToLow(void);
	bool isChangedToHigh(void);
	int read();
	operator int();

	protected:
	byte pin;
	bool statoPin;
	bool statoPinPrec;

	bool isChangedTo(int stato);

};
#endif /* PULSANTE_H_ */
