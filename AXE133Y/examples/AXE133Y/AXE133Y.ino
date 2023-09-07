//AXE133Y Example Sketch

#define oledPin 5

#include <AXE133Y.h>

AXE133Y OLED = AXE133Y(oledPin);

void setup() {
OLED.clearScreen();
OLED.splash("  Hello World!");
}
void loop() {
}

