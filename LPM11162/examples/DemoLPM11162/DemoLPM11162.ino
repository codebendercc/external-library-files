
#include <SoftwareSerial.h>
#include <LPM11162.h>


#define LPM11162_RX     4 //Arduino TX (out)
#define LPM11162_TX     5 //Arduino RX (in)
#define LPM11162_RESET  3 //(out)
#define LPM11162_BUSY   2 //(in)


LPM11162 audio(LPM11162_RX, 
               LPM11162_TX, 
               LPM11162_RESET, 
               LPM11162_BUSY);


void setup()
{
  if ( audio.synch() )
  {
    audio.bridge();
  }

  audio.begin(9600);
  audio.volume(50);
}


void loop()
{
  audio.play("test.wav");
  
  while( audio.isBusy() );
  
  delay( 2000 );
}
