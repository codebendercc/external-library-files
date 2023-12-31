/* 
 * Copyright (c) 2011, Jan Clement
 * licenced under the GPL
 *
 * Author: Jan Clement <jan.clement@audiokits.de>
 *
 * This file is part of the arduiNode library
 * http://code.google.com/p/arduinode/
 */


#include "rf12_ctrl.h"
#include "rf12_driver/rf12_driver/rfm12_config.h"
#include "rf12_driver/rf12_driver/rfm12.h"
#include "rf12_driver/rf12_driver/rfm12.c"// FIXME I AM A DIRTY HACK..

static struct pt pt_rcvPkt;
unsigned char rxbuff[RX_BUFFER_SIZE], rcvPktType;
bool NEWPACKETRECEIVED, NODE_IR_ACTIVE, PHY_CHANNEL_FREE;

void init_pt_rcv() {
  PT_INIT(&pt_rcvPkt); 		// init proto rcv var called in isr
  
}

void rxtx_ctrl_init() {
  rfm12_init();
  sei();
}

void enableIROut(int khz) { // FIXME I AM A DIRTY HACK..

  NODE_IR_ACTIVE = true;
}

void enable_IR_rx(){	// FIXME I AM A DIRTY HACK..	
   
}

unsigned char datab[13], buffcnt;
void sendByte(unsigned char data) {
  if (NODE_IR_ACTIVE){	// connection is open
// Serial.println("#");
    datab[buffcnt++] = data;
  }
}

void rf12_send_buffer() {
//   Serial.println("!");
  rfm12_tx (sizeof(datab), 0, datab);
  buffcnt = 0;
}

bool rf12_getData(){
// FIXME I AM A DIRTY HACK..
   /** RFM12 DATA Injection  **/
    rfm12_poll();
    rfm12_tick();
  uint8_t *bufcontents;
  byte i;
  if(rfm12_rx_status() == STATUS_COMPLETE){
    Serial.print("new packet: ");
    NEWPACKETRECEIVED = true;
    bufcontents = rfm12_rx_buffer();			
    for(i=0; i<bufcontents[1];i++){
      rxbuff[i] = bufcontents[i+1];
      Serial.print(rxbuff[i],HEX);
    }
    Serial.println(" ");
    rcvPktType = rxbuff[3];
    rfm12_rx_clear();

  }
  /** END RFM12 DATA Injection  **/
  return false;
}


void carrierSense() {	// FIXME HACK
//   static unsigned long tstamp;
//   if( !(IRRECVPORT & (1<<IRRECVPIN)) ) { // pin = low = rcv
// 		tstamp = millis();
// 		PHY_CHANNEL_FREE = false; 
//   }
//   else if (millis() - tstamp > CSMA_TIME) {
    PHY_CHANNEL_FREE = true; 
//   }
}
