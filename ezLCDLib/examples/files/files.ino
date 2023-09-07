/*
 * files.ino example
 */

#include <ezLCDLib.h>
#define LED_PIN 12

ezLCD3 lcd;

#define count 50

void setup()
{
    long tell;
    int junk, i, bcount;
    char buffer[ 1024 ];
    
  lcd.begin( EZM_BAUD_RATE ); 
  pinMode( LED_PIN, OUTPUT );
  digitalWrite( LED_PIN, LOW );
  lcd.font( 0 ); 
  lcd.cls( WHITE, BLACK ); // clear screen to white
//start here
  junk = lcd.FSchdir( "\\\\" );//change to root
  lcd.xy( 20, 20 );
  junk = lcd.FSchdir( "Files" );
  if ( junk == 0 ) {
    lcd.printString( "Directory change OK" );
    junk = lcd.FSremove( "junkdata.bin" );//remove it if its there
  }else{
    lcd.printString( "Directory change Failed" );
    junk = lcd.FSmkdir( "Files" );
    junk = lcd.FSchdir( "Files" );
  }
  lcd.xy( 20, 40);
  lcd.printString( "Open File: " );
  junk = lcd.FSopen( "junkdata.bin", "w" );
  if ( junk == 0 ) 
    lcd.printString( "File Opened OK" );
  else
    lcd.printString( "File Open Failed" );

  lcd.xy( 20, 60 );
  bcount = count;//set the count
  lcd.printString( "Write File: junkdata.bin, bcount=" );
  lcd.print( bcount );
  //fill a buffer with data for writes
  for ( i = 0; i < bcount; i++ ) {
    buffer[ i ] = i+1;//fill with valid data
    buffer[ i ] = buffer[ i ] & 0xFF;//filter
    if ( buffer[ i ] == 0x22 ) buffer[ i ] = 0x3F;//make it a ?
    buffer[ i+1 ] = 0;//fill with end
  }
  lcd.xy( 20, 80 );
  junk = lcd.FSwrite( buffer, bcount );
//test pulse for trigger
  digitalWrite( LED_PIN, HIGH );
  delay( 1 );
  digitalWrite( LED_PIN, LOW );
  lcd.printString( "Status=" );
  lcd.print( junk );
  lcd.printString( " No. Bytes Written=" );
  lcd.print( junk );
 
//rewind command
  lcd.xy( 20, 100 );
  junk = lcd.FSrewind();//go to start of file
  if ( junk == 0 ) 
    lcd.printString( "Rewind success" );
  else
    lcd.printString( "Rewind Failed" );

//seek command  SEEK_SET 0  SEEK_CUR 1  SEEK_END 2
  lcd.xy( 20, 120 );
  bcount = 5;
  junk = lcd.FSseek( bcount, SEEK_SET );
  if ( junk == 0 ) 
    lcd.printString( "Seek success" );
  else
    lcd.printString( "Seek Failed" );

//tell command  Where are we again?
  lcd.xy( 20, 140 );
  tell = lcd.FStell( );
  lcd.printString( "Location is:" );
  lcd.printLong( tell );

//attribute command 	* 1=ReadOnly, 2=Hidden, 4=System, 0x20=Archive
  lcd.xy( 20, 160 );
  junk = lcd.FSattrib( 0x21 );
  lcd.printString( "Set attribute to: Read Only" );
  if ( junk == 0 ) 
    lcd.printString( "OK" );
  else
    lcd.printString( "Error" );

//get attribute command 	* 1=ReadOnly, 2=Hidden, 4=System, 0x20=Archive
  lcd.xy( 20, 180 );
  junk = lcd.GetFSattrib( );
  lcd.printString( "Get attribute:" );
  lcd.print( junk );//should be 33
  
//eof command 
  lcd.xy( 200, 100 );
  junk = lcd.FSeof( );
  lcd.printString( "EOF:" );
  if ( junk == 0 ) 
    lcd.printString( "NO" );
  else
    lcd.printString( "YES" );
  
//dir command 
  lcd.xy( 20, 200 );
  lcd.FSgetcwd( );
  lcd.printString( "CWD: " );
  lcd.printString( lcd.localbuffer );
    
//error command 
  lcd.xy( 200, 120 );
  junk = lcd.FSerror( );
  lcd.printString( "Error: " );
  lcd.print( junk );
  
//close file command
  junk = lcd.FSclose();

//file read
  lcd.xy( 20, 0);
  lcd.printString( "Open File: " );
  junk = lcd.FSopen( "junkdata.bin", "r" );
  if ( junk == 0 ) 
    lcd.printString( "OK" );
  else
    lcd.printString( "Failed" );

  lcd.xy( 200, 0);
  bcount = 15;//set the count
  junk = lcd.FSread( buffer, bcount );
  lcd.print( buffer );//question mark means non ascii string

//close file command
  junk = lcd.FSclose();

//test pulse for trigger
//  digitalWrite( LED_PIN, HIGH );
//  delay( 1 );
//  digitalWrite( LED_PIN, LOW );
}

void loop()
{
 
}


