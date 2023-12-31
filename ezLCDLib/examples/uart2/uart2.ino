
#include <ezLCDLib.h>

#define LED_PIN 13

ezLCD3 lcd;

int x1Pos = 10; // horizontal position for buttton 1 
int x2Pos = 210; // horizontal position for buttton 2 
int yPos = 70; // vertical position of both buttons
int width = 100;
int height = 100;
int radius = 20;
int alignment = 0; // 0=centered, 1=right, 2=left, 3=bottom, 4=top 
int option = 1; // 1=draw, 2=disabled, 3=toggle pressed, 4=toggle not pressed,
// 5=toggle pressed disabled, 6=toggle not pressed disabled.
int touch=0;
int result;

void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.cls( BLACK, WHITE );
  lcd.font("0");
  pinMode( LED_PIN, OUTPUT );
  digitalWrite( LED_PIN, LOW );
  digitalWrite( LED_PIN, HIGH );
  
//many examples of same thing for printing the banner
//  lcd.printString("               Button Test and UART2");
//using sendCommand
//  lcd.sendCommand( "XY CT");
//  lcd.printString("Button Test and UART2\" \"ct");
//using escape sequences
//  lcd.printString("\\[60x\\[0yButton Test and UART2");
//using newer library with XY string
  lcd.xy("ct");
  lcd.printString("Button Test and UART2\" \"ct");

  lcd.fontw( 1, "sans24" );
  lcd.theme( 1, 9, 3, 0, 0, 0, 8, 8, 8, 1, 1 );
  lcd.theme( 2, 5, 20, 3, 3, 3, 4, 4, 4, 2, 1 );
  lcd.string( 1, "ON" ); // stringId 1
  lcd.string( 2, "OFF" ); // stringId 2
  lcd.button( 1, x1Pos, yPos, width, height, option, alignment, radius, 1, 1 );
  lcd.button( 2, x2Pos, yPos, width, height, option, alignment, radius, 2, 2 );

  digitalWrite( LED_PIN, LOW );
  lcd.drawLed( 160, 50, 12, BLACK, WHITE );
  lcd.xy(120,70);
  lcd.printString("ID");
  lcd.xy(120,110);
  lcd.printString("Info");
  lcd.xy(120,150);
  lcd.printString("Data");
  lcd.xy(110,210);
  lcd.printString("Serial");
  result = lcd.flush( 11 );//see if was received
}

void printInfo( int location, int result ) {
      lcd.font("0");
      lcd.color(BLACK);
      switch( location) {
        case 1:
          lcd.xy(20,200);
          lcd.box(90,20,FILL);
          lcd.xy(20,200);
          break;
        case 2:
          lcd.xy(220,200);
          lcd.box(90,20,FILL);
          lcd.xy(220,200);
          break;
        case 3://ID
          lcd.xy(170,70);
          lcd.box(40,20,FILL);
          lcd.xy(170,70);
          break;
        case 4://Info
          lcd.xy(170,110);
          lcd.box(40,20,FILL);
          lcd.xy(170,110);
          break;
        case 5://Data
          lcd.xy(170,150);
          lcd.box(40,20,FILL);
          lcd.xy(170,150);
          break;
        default:
          lcd.xy(180,210);
          lcd.box(40,20,FILL);
          lcd.xy(180,210);
          break;
      }
      lcd.color(AQUA);
      if ( result == -1 )
        lcd.print( result );
      if ( result < 256 ) 
        lcd.print( result );
}

void loop()
{
    touch = lcd.wstack(0);
    if ( lcd.currentWidget != 0 ) {
      printInfo( 3, lcd.currentWidget );
      printInfo( 4, lcd.currentInfo );
      printInfo( 5, lcd.currentData );
    }
    if (( lcd.currentWidget == 1 ) && ( lcd.currentInfo == 4 ) && ( lcd.currentData == 0 )) {// if Button 1 is pressed:
      digitalWrite( LED_PIN, HIGH ); // turn LED on
      lcd.drawLed( 160, 50, 12, LIME, WHITE);
      lcd.TxUART( 10, 55 );//send a known character
//      lcd.TxUART( 13, 0x66 );//send a known character
    }else if (( lcd.currentWidget == 2  ) && ( lcd.currentInfo == 4 ) && ( lcd.currentData == 0 )) { // if Button 2 was pressed:
      digitalWrite( LED_PIN, LOW ); // turn LED off
      lcd.drawLed( 160, 50, 12, BLACK, WHITE);
      lcd.TxUART( 10, 77 );
   lcd.theme( 16, 5, 20, 3, 3, 3, 4, 4, 4, 2, 1 );
//      lcd.TxUART( 10, "Hello" );
//      lcd.TxUART( 13, 0x88 );//send a known character
    }
    delay(10);//give it time to receive
   lcd.theme( 16, 5, 20, 3, 3, 3, 4, 4, 4, 2, 1 );
   result = lcd.Available( 11 );//see if was received
    if ( result == 1 ) {
      printInfo( 1, result);
      result = lcd.RecvUART( 11 );//see if was received
      printInfo( 2, result);
    }
//    result = lcd.Available( 11 );//see if was received
//    printInfo( 0, result);
}
