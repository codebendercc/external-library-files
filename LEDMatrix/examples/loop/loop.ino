/**
 * LED Matrix library for http://www.seeedstudio.com/depot/ultrathin-16x32-red-led-matrix-panel-p-1582.html
 * The LED Matrix panel has 32x16 pixels. Several panel can be combined together as a large screen.
 *
 * Coordinate & Connection (Arduino -> panel 0 -> panel 1 -> ...)
 *   (0, 0)                                     (0, 0)
 *     +--------+--------+--------+               +--------+--------+
 *     |   5    |    3   |    1   |               |    1   |    0   |
 *     |        |        |        |               |        |        |<----- Arduino
 *     +--------+--------+--------+               +--------+--------+
 *     |   4    |    2   |    0   |                              (64, 16)
 *     |        |        |        |<----- Arduino
 *     +--------+--------+--------+
 *                             (96, 32)
 *
 */

#include "LEDMatrix.h"

#define WIDTH   64
#define HEIGHT  16

// LEDMatrix(a, b, c, d, oe, r1, stb, clk);
LEDMatrix matrix(4, 5, 6, 7, 8, 9, 10, 11);

// Display Buffer 128 = 64 * 16 / 8
uint8_t displaybuf[WIDTH *HEIGHT / 8] = {
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
    0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0xE0, 0x07, 0x8F, 0xC7, 0xC7, 0xC7, 0xE0,
    0x00, 0x40, 0x0C, 0xCE, 0x6C, 0x6C, 0x6C, 0xE0, 0x00, 0xE0, 0x0C, 0x0C, 0x6C, 0x6C, 0x6C, 0x60,
    0x01, 0xF0, 0x07, 0x8C, 0x6F, 0xEF, 0xEC, 0x60, 0x23, 0xF8, 0x00, 0xCC, 0x6C, 0x0C, 0x0C, 0x60,
    0x33, 0xF8, 0x0C, 0xCE, 0x6C, 0x6C, 0x6C, 0xE0, 0x3B, 0xF8, 0x07, 0x8F, 0xC7, 0xC7, 0xC7, 0xE0,
    0x3B, 0xF8, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x1B, 0xF8, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
    0x0B, 0xF8, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// 16 * 8 digital font
const uint8_t digitals[] = {
    0x00, 0x1C, 0x36, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00, 0x00, 0x00, 0x00, // 0
    0x00, 0x18, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00, 0x00, // 1
    0x00, 0x3E, 0x63, 0x63, 0x63, 0x06, 0x06, 0x0C, 0x18, 0x30, 0x63, 0x7F, 0x00, 0x00, 0x00, 0x00, // 2
    0x00, 0x3E, 0x63, 0x63, 0x06, 0x1C, 0x06, 0x03, 0x03, 0x63, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, // 3
    0x00, 0x06, 0x0E, 0x1E, 0x36, 0x36, 0x66, 0x66, 0x7F, 0x06, 0x06, 0x1F, 0x00, 0x00, 0x00, 0x00, // 4
    0x00, 0x7F, 0x60, 0x60, 0x60, 0x7C, 0x76, 0x03, 0x03, 0x63, 0x66, 0x3C, 0x00, 0x00, 0x00, 0x00, // 5
    0x00, 0x1E, 0x36, 0x60, 0x60, 0x7C, 0x76, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00, 0x00, 0x00, 0x00, // 6
    0x00, 0x7F, 0x66, 0x66, 0x0C, 0x0C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, // 7
    0x00, 0x3E, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x63, 0x63, 0x3E, 0x00, 0x00, 0x00, 0x00, // 8
    0x00, 0x1C, 0x36, 0x63, 0x63, 0x63, 0x37, 0x1F, 0x03, 0x03, 0x36, 0x3C, 0x00, 0x00, 0x00, 0x00, // 9
};

// (x, y) top-left position, x should be multiple of 8
void drawDigital(uint16_t x, uint16_t y, uint8_t n)
{
    if ((n >= 10) || (0 != (x % 8))) {
        return;
    }

    uint8_t *pDst = displaybuf + y * (WIDTH / 8) + x / 8;
    const uint8_t *pSrc = digitals + n * 16;
    for (uint8_t i = 0; i < 16; i++) {
        *pDst = *pSrc;
        pDst += WIDTH / 8;
        pSrc++;
    }
}


void setup()
{
    matrix.begin(displaybuf, WIDTH, HEIGHT);
}

void loop()
{
    static uint32_t lastCountTime = 0;
    static uint8_t count = 0;

    matrix.scan();

    if ((millis() - lastCountTime) > 1000) {
        lastCountTime = millis();
        matrix.clear();
        drawDigital(40, 0, count);
        count = (count + 1) & 0x7;
    }
}

