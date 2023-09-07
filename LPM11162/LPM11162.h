/*
  LPM11162.h - Arduino Library for LPM11162 audio module.
  Created by LP ELETTRONICA - www.lpelettronica.it, March 12, 2011.
  Released into the public domain.
*/
#ifndef LPM11162_H
#define LPM11162_H


#define LPM_NO_PIN		0xFF

class LPM11162
{
	private:
	  SoftwareSerial* pSoftSer;
	  uint8_t SoftSerTx;
	  uint8_t SoftSerRx;
	  uint8_t BusyPin;
	  uint8_t ResetPin;
	  void loop(uint8_t a_rx, uint8_t a_tx, uint8_t b_rx, uint8_t b_tx);
		
	public:
	  // public methods
	  LPM11162(uint8_t tx, uint8_t rx = LPM_NO_PIN, uint8_t reset = LPM_NO_PIN, uint8_t busy = LPM_NO_PIN);
	  ~LPM11162();
	  
	  void begin(uint32_t baudrate);
	  void end();
	  void play(char* pFileName);
	  void stop();
	  void volume(char volume);
	  bool isBusy();
	  void reset(bool resetLevel);
	  void bridge();
	  bool synch();
};

#endif
