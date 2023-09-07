#include <ezLCDLib.h>

ezLCD3 lcd;

void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.cls(BLACK);
  lcd.color(WHITE);
  lcd.font( 0 );
  lcd.println("Hello, World!");
  lcd.println();
  lcd.println(65);
  lcd.print("65 in Dec ");
  lcd.println(65,DEC);
  lcd.print("65 in Hex ");
  lcd.println(65,HEX);
  lcd.print("65 in Oct ");
  lcd.println(65,OCT);
  lcd.print("65 in BIN ");
  lcd.println(65,BIN);
  lcd.print("Pi ");
  lcd.println(3.14159);
  lcd.print(3.14159);
}

void loop() {}
