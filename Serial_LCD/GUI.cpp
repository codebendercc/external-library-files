//
// 4D Systems uLCD-uLED-uVGA Serial_LCD Library Suite
// Arduino 0023 and 1.0, chipKIT MPIDE 0023, Wiring 1.0
// ----------------------------------
//
// Jan 15, 2013 release 320
// See README.txt
//
// © Rei VILO, 2010-2013
//   CC = BY NC SA
//   http://embeddedcomputing.weebly.com/serial-lcd.html
//   http://github.com/rei-vilo/Serial_LCD
//
// For
//   4D Systems Goldelox and Picaso SGC Command Set
//   http://www.4dsystems.com.au/
//

// Library header
#include "GUI.h"

// Other libraries
#include "Serial_LCD.h"

// ---- utility
item setItem(uint8_t index0, String text0) {
    item item0;
    item0.index = index0;
    text0.toCharArray(item0.label, 16);
    return item0;
}


// ---- area
area::area() {
    ;
}

void area::dDefine(Serial_LCD  * lcd,
                   uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                   uint16_t index)
{
    _pscreen = lcd;
    
    _x0 = x0;
    _y0 = y0;
    _dx = dx;
    _dy = dy;
    
    _index = index;
    _enable = false;
    _option = false;
}


bool area::state() {
    return _enable;
}

void area::enable(bool flag)
{
    _enable = flag;
}

void area::setOption(bool flag)
{
    _option = flag;
}

bool area::check(bool instant)
{
    if (!_enable) return false;
    
    uint16_t x, y, z;
    
    // down
    if ((_pscreen->getTouchActivity() == 1) || (_pscreen->getTouchActivity() == 3)) {
        _pscreen->getTouchXY(x, y);
        
        // pressed
        if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
            if (instant) return true;
            
            // debounce
            do {
                z = _pscreen->getTouchActivity();
                delay(10);
            } while (z != 2);
            
            // up
            if (z == 2)  {
                _pscreen->getTouchXY(x, y);
                if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) return true;
            } // if up
        } // if pressed
    } // if down
    return false;
}


uint16_t area::getIndex()
{
    return _index;
}



// ---- button
button::button() {
    ;
}

// string-based button
void button::dStringDefine(Serial_LCD * lcd,
                           uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                           String text0, uint16_t textColour1, uint16_t highColour1)
{
    dStringDefine(lcd, x0, y0, dx, dy, text0, textColour1, highColour1, lcd->halfColour(highColour1), 9);
}


void button::dStringDefine(Serial_LCD * lcd,
                           uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                           String text0, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, uint8_t size0)
{
    dDefine(lcd, x0, y0, dx, dy, setItem(0, text0), textColour1, highColour1, lowColour1, size0);
}


void button::dDefine(Serial_LCD * lcd,
                     uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     item item0, uint16_t textColour1, uint16_t highColour1)
{
    dDefine(lcd, x0, y0, dx, dy, item0, textColour1, highColour1, _pscreen->halfColour(highColour1), 9);
}


void button::dDefine(Serial_LCD  * lcd,
                     uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     item item0, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, uint8_t size0)
{
    area::dDefine(lcd, x0, y0, dx, dy, item0.index);
    
    _text = item0.label;
    _textColour = textColour1;
    _highColour = highColour1;
    _lowColour  = lowColour1;
    _size = size0;
    _value = false;
    
    setOption(false);
}


void button::draw(bool flag)
{
    // 4 states
    // enable = true,  bool flag = true  : pressed button
    // enable = true,  bool flag = false : available button
    // enable = false, bool flag = true  : statement button
    // enable = false, bool flag = false : unavailable button
    uint16_t colourText, colourLabel, colourRectangle;
    
    colourText = _textColour;
    colourRectangle = _highColour;
    colourLabel = _highColour;
    
    if (_enable) {
        if ( flag || ( _option && _value ) ) {
            colourRectangle = _lowColour;
        } else {
            colourLabel = _lowColour;
        }
    } else {
        if (flag) {
            ;
        } else {
            colourText = _pscreen->halfColour(_textColour);
            colourLabel = _pscreen->halfColour(_lowColour);
            colourRectangle = _pscreen->halfColour(_highColour);
        }
    }
    
    dLabel(_pscreen, _x0, _y0, _dx, _dy, _text, colourText, colourLabel, 0, 0, _size);
    _pscreen->setPenSolid(false);
    _pscreen->dRectangle(_x0+2, _y0+2, _dx-4, _dy-4, colourRectangle);
}


//void button::enable(bool flag)
//{
//    _enable = flag;
//}

bool button::check(bool instant)
{
    uint16_t x, y, z;
    bool flag = false;
    
    if (!_enable) return false;
    
    // down
    if ((_pscreen->getTouchActivity() == 1) || (_pscreen->getTouchActivity() == 3)) {
        
        _pscreen->getTouchXY(x, y);
        
        // pressed
        if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
            // instant
            if (instant) {
                if (_option) _value = ~_value;
                flag = true;
            } else {
                draw(true);
                
                // debounce
                do {
                    z = _pscreen->getTouchActivity();
                    delay(10);
                } while (z != 2);
                
                // released
                delay(50);
                
                // up
                if (z == 2)  {
                    _pscreen->getTouchXY(x, y);
                    if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
                        flag = true;
                        if (_option) _value = ~_value;
                    }
                } // if up
            } // if instant
            
            draw(false || (_option && _value) );
        } // if pressed
    } // if down
    return flag;
}

void button::setValue(bool value)
{
    _value = value;
}

bool button::getValue()
{
    return _value;
}


// ---- cursor
cursor::cursor() {
    ;
}

void cursor::dDefine(Serial_LCD * lcd,
                     uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     int32_t minValue, int32_t maxValue,
                     uint16_t cursorColour, uint16_t highColour, uint16_t lowColour)
{
    area::dDefine(lcd, x0, y0, dx, dy, 0);
    _cursorColour = cursorColour;
    _highColour = highColour;
    _lowColour = lowColour;
    
    _minValue = minValue;
    _maxValue = maxValue;
    setOption(false);
}


void cursor::setOption(bool flag)
{
    area::setOption(flag);
    
    if (_option) {              // vertical
        //        Serial.println("cursor::setOption vertical");
        //
        _gMin = _y0 + 7;
        _gMax = _y0 + _dy - 7;
        _gLine = _x0 + _dx/2;
    } else {                    // horizontal
        //        Serial.println("cursor::setOption horizontal");
        //
        _gMin = _x0 + 7;
        _gMax = _x0 + _dx - 7;
        _gLine = _y0 + _dy/2;
    }
    
    _gPosition = _gMin;
}


void cursor::draw(bool flag)
{
    // 4 states
    // enable = true,  bool flag = true  : pressed button
    // enable = true,  bool flag = false : available button
    // enable = false, bool flag = true  : statement button
    // enable = false, bool flag = false : unavailable button
    uint16_t colourCursor, colourBackground, colourRectangle;
    
    colourCursor = _cursorColour;
    colourRectangle = _highColour;
    colourBackground = _highColour;
    
    if (_enable) {
        if (flag) {
            colourRectangle = _lowColour;
        } else {
            colourBackground = _lowColour;
        }
    } else {
        if (flag) {
            ;
        } else {
            colourCursor = _pscreen->halfColour(_cursorColour);
            colourBackground = _pscreen->halfColour(_lowColour);
            colourRectangle = _pscreen->halfColour(_highColour);
        }
    }
    
    _pscreen->setPenSolid(true);
    _pscreen->dRectangle(_x0, _y0, _dx, _dy, colourBackground);
    _pscreen->setPenSolid(false);
    _pscreen->dRectangle(_x0+2, _y0+2, _dx-4, _dy-4, colourRectangle);
    
    _pscreen->setPenSolid(true);
    if (_option) {              // vertical
        //        Serial.println("cursor::draw vertical");
        //
        _pscreen->dRectangle(_gLine, _gMin, 2, _gMax-_gMin, colourCursor);
        _pscreen->dRectangle(_x0+4, _gPosition - 3, _dx-8, 6, colourCursor);
    } else {                    // horizontal
        //        Serial.println("cursor::draw horizontal");
        //
        _pscreen->dRectangle(_gMin, _gLine, _gMax-_gMin, 2, colourCursor);
        _pscreen->dRectangle(_gPosition - 3, _y0+4, 6, _dy-8, colourCursor);
    }
}


bool cursor::check(bool instant)
{
    uint16_t x, y, z, _gLast, _gOld;
    bool flag = false;
    
    if (!_enable) return false;
    
    // down
    if ((_pscreen->getTouchActivity() == 1) || (_pscreen->getTouchActivity() == 3)) {
        
        _pscreen->getTouchXY(x, y); // acquisition
        if (_option) {              // vertical
            if (y < min(_gMin, _gMax)) y = min(_gMin, _gMax);
            if (y > max(_gMin, _gMax)) y = max(_gMin, _gMax);
        } else {                    // horizontal
            if (x < min(_gMin, _gMax)) x = min(_gMin, _gMax);
            if (x > max(_gMin, _gMax)) x = max(_gMin, _gMax);
        }
        
        // pressed
        if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
            _gOld = _gPosition;
            _gLast = _gOld;
            
            // instant
            if (instant) {
                if (_option) {              // vertical
                    if (y != _gOld) {
                        _gPosition = y;
                        flag = true;
                    }
                } else {                    // horizontal
                    if (x != _gOld) {
                        _gPosition = x;
                        flag = true;
                    }
                }
            } else {
                
                // debounce
                do {
                    z = _pscreen->getTouchActivity();
                    
                    _pscreen->getTouchXY(x, y); // acquisition
                    if (_option) {              // vertical
                        if (y < min(_gMin, _gMax)) y = min(_gMin, _gMax);
                        if (y > max(_gMin, _gMax)) y = max(_gMin, _gMax);
                    } else {                    // horizontal
                        if (x < min(_gMin, _gMax)) x = min(_gMin, _gMax);
                        if (x > max(_gMin, _gMax)) x = max(_gMin, _gMax);
                    }
                    
                    if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
                        if (_option) {              // vertical
                            if (y != _gPosition) _gPosition = y;
                        } else {                    // horizontal
                            if (x != _gPosition) _gPosition = x;
                        }
                        
                    } else {
                        _gPosition = _gOld;
                    }
                    
                    if (_gPosition != _gLast) {
                        _gLast = _gPosition;
                        draw(true);
                    }
                } while (z != 2);
                
                // up
                if (z == 2)  {
                    if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
                        if (_option) {              // vertical
                            if (y != _gOld) {
                                _gPosition = y;
                                flag = true;
                            }
                        } else {                    // horizontal
                            if (x != _gOld) {
                                _gPosition = x;
                                flag = true;
                            }
                        }
                    } else {
                        _gPosition = _gLast;
                    }
                } // if up
            } // if instant
            draw();
        } // if pressed
    } // if down
    return flag;
}

void cursor::setValue(int32_t value)
{
    if (value < min(_minValue, _maxValue)) value = min(_minValue, _maxValue);
    if (value > max(_minValue, _maxValue)) value = max(_minValue, _maxValue);
    
    _gPosition = map(value, _minValue, _maxValue, _gMin, _gMax);
    
    //    Serial.print(value, DEC);
    //    Serial.print(" => ");
    //    Serial.println(_gPosition, DEC);
}

int32_t cursor::getValue()
{
    return map(_gPosition, _gMin, _gMax, _minValue, _maxValue);
}


// ---- dialog
uint8_t dialog(Serial_LCD * lcd,
               String text0, uint8_t kind0, uint16_t textColour0, uint16_t highColour0, uint16_t lowColour0,
               String text1, String button1, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1,
               String text2, String button2, uint16_t textColour2, uint16_t highColour2, uint16_t lowColour2,
               String text3, String button3, uint16_t textColour3, uint16_t highColour3, uint16_t lowColour3) {
    
    uint8_t a;
    Serial_LCD * _pscreen = lcd;
    
    // Check SD
    if ( !_pscreen->checkSD() ) a = _pscreen->initSD();
    
    // landscape and portrait coordinates
    uint16_t _x0 = (_pscreen->maxX() - 200) /2;
    uint16_t _y0 = (_pscreen->maxY() - 200) /2;
    
    // Save initial screen
    if ( _pscreen->checkSD() ) {
        if ( _pscreen->checkRAW() ) {
            a = _pscreen->dSaveScreenRAW((uint32_t)0, _x0, _y0, 200, 200);
            //      Serial.print("\r\n dialog dSaveScreenRAW \t");
            //      Serial.print(a, HEX);
            //      Serial.print("\r\n");
        }
        else {
            a = _pscreen->dSaveScreenFAT("dialog.tmp", _x0, _y0, 200, 200);
            //      Serial.print("\r\n dialog dSaveScreenFAT \t");
            //      Serial.print(a, HEX);
            //      Serial.print("\r\n");
        }
    }
    
    // Draw window
    _pscreen->setPenSolid(true);
    _pscreen->dRectangle(_x0, _y0, 200, 200, lowColour0);
    delay(10);    // delay required by chipKIT
    _pscreen->setPenSolid(false);
    _pscreen->dRectangle(_x0+2, _y0+2, 200-2-2, 200-2-2, highColour0);
    
    // String &result0, String text0, uint8_t kind0
    // uint16_t textColour0, uint16_t highColour0, uint16_t lowColour0,
    dLabel(_pscreen, _x0+ 60, _y0+ 20, 200-60-10, 20, text0, textColour0, lowColour0, 1, 1, 3);
    dLabel(_pscreen, _x0+ 10, _y0+ 70, 200-10-10, 20, text1, textColour0, lowColour0, 1, 1, 1);
    dLabel(_pscreen, _x0+ 10, _y0+ 90, 200-10-10, 20, text2, textColour0, lowColour0, 1, 1, 1);
    dLabel(_pscreen, _x0+ 10, _y0+110, 200-10-10, 20, text3, textColour0, lowColour0, 1, 1, 1);
    
    // uint8_t kind0,
    uint16_t colour;
    char label;
    switch (kind0) {
        case 1:
            colour = blueColour;  // blue !
            label ='!';
            break;
        case 2:
            colour = yellowColour;  // yellow ?
            label ='?';
            break;
        case 3:
            colour = redColour;  // red X
            label ='X';
            break;
        default:
            colour = greenColour;  // green +
            label ='+';
    }
    
    _pscreen->setPenSolid(false);
    _pscreen->circle(_x0+30, _y0+30, 18, colour);
    _pscreen->circle(_x0+30, _y0+30, 16, colour);
    _pscreen->circle(_x0+30, _y0+30, 15, colour);
    
    _pscreen->setFont(3);
    _pscreen->setFontSolid(false);
    _pscreen->gText(_x0+30- (_pscreen->fontX()>>1)   , _y0+30- (_pscreen->fontY()>>1)   , String(label), colour);
    _pscreen->gText(_x0+30- (_pscreen->fontX()>>1) +1, _y0+30- (_pscreen->fontY()>>1)   , String(label), colour);
    _pscreen->gText(_x0+30- (_pscreen->fontX()>>1)   , _y0+30- (_pscreen->fontY()>>1) +1, String(label), colour);
    _pscreen->gText(_x0+30- (_pscreen->fontX()>>1) +1, _y0+30- (_pscreen->fontY()>>1) +1, String(label), colour);
    
    button b1;
    button b2;
    button b3;
    
    b1.dDefine(_pscreen, _x0+130, _y0+150, 60, 40, setItem( 1, button1), textColour1, highColour1, lowColour1, 9);
    b2.dDefine(_pscreen, _x0+ 70, _y0+150, 60, 40, setItem( 2, button2), textColour2, highColour2, lowColour2, 9);
    b3.dDefine(_pscreen, _x0+ 10, _y0+150, 60, 40, setItem( 3, button3), textColour3, highColour3, lowColour3, 9);
    
    b1.enable(true);
    b1.draw();
    if (button2.length()>0) {
        b2.enable(true);
        b2.draw();
    }
    if (button3.length()>0) {
        b3.enable(true);
        b3.draw();
    }
    
    // dialog main loop
    uint8_t answer = 0;
    while ( answer==0 ) {
        if      (b1.check()) answer = b1.getIndex();
        else if (b2.check()) answer = b2.getIndex();
        else if (b3.check()) answer = b3.getIndex();
    }
    
    if ( _pscreen->checkSD() ) {
        if ( _pscreen->checkRAW() ) a=_pscreen->readScreenRAW((uint32_t)0, _x0, _y0);
        else a=_pscreen->readScreenFAT("dialog.tmp", _x0, _y0);
    }
    
    return answer;
}


// ---- textBox
textBox::textBox()
{
    ;
}

void textBox::dDefine(Serial_LCD * lcd,
                      uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                      uint16_t textColour, uint16_t backColour,
                      uint8_t size)
{
    area::dDefine(lcd, x0, y0, dx, dy, 0);
    _textColour = textColour;
    _backColour = backColour;
    
    _size = size;
    _pscreen->setFont(size);
    _tx = _pscreen->fontX() + 0;
    _ty = _pscreen->fontY() + 1;
    _maxColumns = dx / _tx - 0;
    _maxLines = dy / _ty   - 1;
    
    _line = 0;
    _column = 0;
    setOption(false);
}

void textBox::clear()
{
    _pscreen->setPenSolid(true);
    _pscreen->dRectangle(_x0, _y0, _dx, _dy, _backColour);
    _line = 0;
    _column = 0;
}


void textBox::scroll()
{
    Serial.println("scroll");
    _line++;
    if (_line > _maxLines) {
//        Serial.print("scroll - copyPaste");
//        Serial.print("\t source:");
//        Serial.print(_x0, DEC);
//        Serial.print("\t");
//        Serial.print(_y0+_ty, DEC);
//        Serial.print("\t target");
//        Serial.print(_x0, DEC);
//        Serial.print("\t");
//        Serial.print(_y0, DEC);
//        Serial.print("\t size:");
//        Serial.print(_dx-1, DEC);
//        Serial.print("\t");
//        Serial.println(_dy-_ty-1);

        
        uint8_t a = _pscreen->copyPaste(_x0, _y0+_ty, _x0, _y0, _dx-1, _dy-_ty-1);
        _pscreen->setPenSolid(true);
        _pscreen->rectangle(_x0, _y0+(_maxLines*_ty), _x0+_dx-1, _y0+_dy-1, _backColour);
        _line = _maxLines;
    }
    _column = 0;
}

void textBox::print(String text)
{
    int8_t i;
    

    Serial.println("print");

    _pscreen->setFont(_size);
    _pscreen->setFontSolid(true);
    _pscreen->setBackGroundColour(_backColour);
    
    while (text.length()) {
        i = text.indexOf(' ');
        if (i<0) i=text.length();
        i++;
        
        //        Serial.print("text.indexOf = ");
        //        Serial.print(text.indexOf(' '), DEC);
        //        Serial.print("    text.length = ");
        //        Serial.print(text.length(), DEC);
        //        Serial.print("    i = ");
        //        Serial.println(i, DEC);
        
        
        if (_column+i > _maxColumns) scroll();
        
        //        Serial.print("_column = ");
        //        Serial.print(_column, DEC);
        //        Serial.print("    _line = ");
        //        Serial.println(_line, DEC);
        //        Serial.print("x = ");
        //        Serial.print(_x0+_column*_tx, DEC);
        //        Serial.print("    y = ");
        //        Serial.print(_y0+_line*_ty, DEC);
        //        Serial.print("    >");
        //        Serial.print(text.substring(0, i+1));
        //        Serial.println("<");
        
        //        _pscreen->gText(_x0+_column*_tx, _y0+_line*_ty, text.substring(0, i+1), _textColour);
        //        _column += i;
        
        for (uint8_t k=0; k<i; k++) {
            char c = text[k];
            if (c == 0x0d) {
                clear();
            } else if (c == 0x0a) {
                scroll();
            } else {
//                Serial.print("print - gText");
//                Serial.print("\t source:");
//                Serial.print(_x0+_column*_tx, DEC);
//                Serial.print("\t");
//                Serial.print(_y0+_line*_ty, DEC);
//                Serial.println();

                // Goldelox requires some time
               if (_pscreen->checkControllerType()==__Goldelox__) delay(10);
                
                
                uint8_t a = _pscreen->gText(_x0+_column*_tx, _y0+_line*_ty, String(c), _textColour);
                _column++;
            }
        }
        text = text.substring(i);
        
        //        Serial.print(">");
        //        Serial.print(text);
        //        Serial.println("<");
        //        Serial.println();
        
        
    }
}


// ---- menu
uint16_t menu(Serial_LCD * lcd,
              item menuItem0[], uint8_t nItems0,
              uint16_t textColour0, uint16_t highColourMain0, uint16_t highColourSub0)
{
    uint8_t i, k, nMain, nSub, nItems;
    uint16_t current, mask;
    button main[8];
    button sub[8];
    bool mainFlag, subFlag;
    bool oldState0, oldState7;
    uint32_t sizeRAW = 0;
    Serial_LCD * _pscreen = lcd;
    
    // Check SD
    nItems = nItems0;
    if ( !_pscreen->checkSD() ) _pscreen->initSD();
    
    // landscape and portrait coordinates
    
    // Save initial screen
    if ( _pscreen->checkSD() ) {
        if ( _pscreen->checkRAW() ) {
            sizeRAW = _pscreen->getSectors(80, 30);
            for (i=0; i<8; i++) {
                _pscreen->dSaveScreenRAW((uint32_t) sizeRAW*(i+0),  0, i*30, 80, 30 ); // main
                _pscreen->dSaveScreenRAW((uint32_t) sizeRAW*(i+8), 80, i*30, 80, 30 ); // sub
            }
            _pscreen->dSaveScreenRAW((uint32_t) sizeRAW*(i+9), 0, _pscreen->maxY()-60, 80, 60 ); // OK and Cancel
        }
        else {
            for (i=0; i<8; i++) {
                _pscreen->dSaveScreenFAT( "main" + String(i, HEX) + ".tmp",  0, i*30, 80, 30 );
                _pscreen->dSaveScreenFAT( "sub"  + String(i, HEX) + ".tmp", 80, i*30, 80, 30 );
            }
            _pscreen->dSaveScreenFAT( "low.tmp" , 0, _pscreen->maxY()-60, 80, 60 ); // OK and Cancel
        }
    }
    
    //    Serial.print("\r\n ! end of saveScreen... \n");
    
    main[0].dDefine(      _pscreen, 0, 0*30, 80, 30, menuItem0[0], whiteColour, highColourMain0);
    main[6].dStringDefine(_pscreen, 0, _pscreen->maxY()-2*30, 80, 30, "Cancel", whiteColour, yellowColour);
    main[7].dStringDefine(_pscreen, 0, _pscreen->maxY()-30, 80, 30, "OK", whiteColour, greenColour);
    
    current = 0x0000;
    nMain=0;
    
    mainFlag = false;
    
    // persistent buttons
    main[0].enable(false);
    oldState0 = false;
    main[6].enable(true);
    main[7].enable(false);
    oldState7 = false;
    
    main[0].draw();
    main[6].draw();
    main[7].draw();
    
    // main loop
    while ( !mainFlag ) {
        for (i=0; i<8; i++) sub[i].enable(false);
        for (i=0; i<6; i++) main[i].enable(false);
        
        // main menu column
        // menu title
        nMain = 0;
        main[nMain].dDefine(_pscreen,  0, nMain*30,  80, 30, menuItem0[nMain], whiteColour, highColourMain0);
        nMain++;
        
        // other menus
        for (k=0; k<4; k++) {
            mask = (current >> (4*(3-k)));
            if ( (mask & 0xf) >0 ) {
                for (i=1; i<nItems; i++) {
                    if ( menuItem0[i].index == (mask << (4*(3-k))) ) {
                        main[nMain].dDefine(_pscreen,  0, nMain*30,  80, 30, menuItem0[i], whiteColour, highColourMain0);
                        main[nMain].enable(true);
                        main[nMain].draw();
                        nMain++;
                    }
                }
            }
        }
        
        if ( _pscreen->checkRAW() )
            for (i=1; i<6; i++) {
                if (i>=nMain) _pscreen->readScreenRAW( sizeRAW*(i+0),  0, i*30 ); // main
            }
        else for (i=1; i<6; i++) {
            if (i>=nMain) _pscreen->readScreenFAT( "main"  + String(i, HEX) + ".tmp", 0, i*30 );
        }
        
        // sub-menu column
        mask = 0x0000;
        for (uint8_t k=0; k<4; k++) {
            if ( (current & (0xf << (4*k))) > 0 ) mask |= ((uint16_t)0xf << (4*k));
        }
        mask ^= 0xffff;
        
        nSub = 0;
        for (i=0; i<nItems; i++) {
            if (nSub<8) {
                
                if ( ((menuItem0[i].index & (mask >> 4)) == 0) && (menuItem0[i].index > current) && (menuItem0[i].index < current+mask) ) {
                    sub[nSub].dDefine(_pscreen, 80, nSub*30, 80, 30, menuItem0[i], whiteColour, highColourSub0);
                    sub[nSub].enable(true);
                    sub[nSub].draw();
                    nSub++;
                }
            }
        }
        
        for (i=0; i<8; i++) {
            if (i>=nSub) {
                if (_pscreen->checkRAW()) _pscreen->readScreenRAW( sizeRAW*(i+8), 80, i*30 ); // sub
                else _pscreen->readScreenFAT( "sub"  + String(i, HEX) + ".tmp", 80, i*30 );
            }
        }
        
        // final preparation
        // hide / show menu Cancel OK buttons
        if (nMain>1) {
            main[nMain-1].enable(false);
            main[nMain-1].draw(true);
        }
        // menu title
        main[0].enable(nMain>1);
        if ( oldState0 != main[0].state() ) {
            main[0].draw();
            oldState0 = main[0].state();
        }
        
        // OK button
        main[7].enable(nSub==0);
        if ( oldState7 != main[7].state() ) {
            main[7].draw();
            oldState7 = main[7].state();
        }
        
        // touch loop
        subFlag=false;
        
        do {
            for (i=0; i<nSub; i++) {
                if (sub[i].check()) {
                    current=sub[i].getIndex();
                    subFlag = true;
                };
            };
            
            for (i=0; i<nMain; i++) {
                if (main[i].check()) {
                    current = main[i].getIndex();
                    subFlag = true;
                }
            }
            
            if (main[6].check()) {          // Cancel
                mainFlag = true;
                current = 0x0000;
            }
            else if (main[7].check()) {     // OK
                mainFlag = true;
            }
        }
        while ( !(subFlag || mainFlag) );
    }
    
    // Restore initial screen
    if ( _pscreen->checkSD() ) {
        if ( _pscreen->checkRAW() ) {
            for (i=0; i<8; i++) {
                _pscreen->readScreenRAW( sizeRAW*(i+0),  0, i*30 ); // main
                _pscreen->readScreenRAW( sizeRAW*(i+8), 80, i*30 ); // sub
            }
            _pscreen->readScreenRAW( sizeRAW*(i+9), 0, _pscreen->maxY()-60 ); // OK and Cancel
            
        } else {
            for (i=0; i<8; i++) {
                _pscreen->readScreenFAT( "main" + String(i, HEX) + ".tmp",  0, i*30 );
                _pscreen->readScreenFAT( "sub"  + String(i, HEX) + ".tmp", 80, i*30 );
            }
            _pscreen->readScreenFAT( "low.tmp" , 0, _pscreen->maxY()-60 ); // OK and Cancel
        }
    }
    return current;
}


// ---- label
void dLabel(Serial_LCD  * lcd,
            uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
            String text0, uint16_t textColour0, uint16_t backColour0,
            uint8_t horizontal0, uint8_t vertical0, uint8_t size0)
{
    uint8_t _size;
    uint16_t _xt, _yt;
    String _text;
    Serial_LCD * _pscreen = lcd;
    
    if ( size0==9 ) {
        uint16_t i=4;
        do {
            i--;
            _pscreen->setFont(i);
        }
        //    while ( ( (text0.length() * _pscreen->fontX() > (x2-x1)) || (_pscreen->fontY() > (y2-y1)) ) && (i>0) );
        while ( ( (text0.length() * _pscreen->fontX() > (dx-6)) || (_pscreen->fontY() > (dy-6)) ) && (i>0) );
        _size = i;
    }
    else {
        _size = size0;
    }
    _pscreen->setFont(_size);
    _text = text0.substring(0, min(text0.length(), (dx-6) / _pscreen->fontX()));
    
#if (defined(CHIPKIT) || (defined(ARDUINO) && (ARDUINO < 100)) ) //  Arduino 23 and chipKIT specific
    _text = _text.trim();
#else // Wiring, Maple, Energia specific
    _text.trim();
#endif
    
    //  // horizontal
    //  // default = 0 = center, 1 = left, 2 = right
    //  if ( horizontal0==1 ) _xt = x1;
    //  else if ( horizontal0==2 ) _xt = x2 - _text.length() * _pscreen->fontX();
    //  else _xt = x1 + ( (x2-x1) - (_text.length() * _pscreen->fontX()) ) /2;
    //
    //  // vertical
    //  // default = 0 = center, 1 = up, 2 = down
    //  if (vertical0==1)   _yt = y1;
    //  else if (vertical0==2)   _yt = y2 - _pscreen->fontY();
    //  else   _yt = y1 + ( (y2-y1) - _pscreen->fontY() ) /2;
    //
    //  _pscreen->setPenSolid(true);
    //  _pscreen->rectangle(x1, y1, x2, y2, backColour0);
    //  delay(10);    // delay required by chipKIT
    
    // horizontal
    // default = 0 = center, 1 = left, 2 = right
    if ( horizontal0==1 ) _xt = x0;
    else if ( horizontal0==2 ) _xt = x0 + dx - _text.length() * _pscreen->fontX();
    else _xt = x0 + ( dx - (_text.length() * _pscreen->fontX()) ) /2;
    
    // vertical
    // default = 0 = center, 1 = up, 2 = down
    if (vertical0==1)   _yt = y0;
    else if (vertical0==2)   _yt = y0 + dy - _pscreen->fontY();
    else   _yt = y0 + ( dy - _pscreen->fontY() ) /2;
    
    _pscreen->setPenSolid(true);
    _pscreen->dRectangle(x0, y0, dx, dy, backColour0);
    delay(10);    // delay required by chipKIT
    
    _pscreen->setFont(_size);
    _pscreen->setFontSolid(false);
    _pscreen->gText(_xt, _yt, _text, textColour0);
}


// ---- slider
bool slider(Serial_LCD * lcd,
               uint16_t &value, uint16_t min, uint16_t max, uint16_t step,
               uint16_t minColour0, uint16_t maxColour0,
               String okText0, uint16_t okTextColour0, uint16_t okColour0,
               String cancelText0, uint16_t cancelTextColour0, uint16_t cancelColour0)
{
    Serial_LCD * _pscreen = lcd;
    
    char a = 0x00;
    uint16_t i, j;
    uint16_t k = _pscreen->maxY()-80;
    
    // Check SD
    if ( !_pscreen->checkSD() ) _pscreen->initSD();
    
    // landscape and portrait coordinates
    uint16_t _x0 = _pscreen->maxX() - 60;
    uint16_t _y0 = 0;
    
    // save initial screen
    if ( _pscreen->checkSD() ) {
        if ( _pscreen->checkRAW() ) a = _pscreen->dSaveScreenRAW((uint32_t) 0, _x0,  _y0, 60, _pscreen->maxY() );
        else a = _pscreen->dSaveScreenFAT( "slider.tmp", _x0, _y0, 60, _pscreen->maxY() );
    }
    
    //    Serial.print("\r\n dSaveScreenRAW/FAT ");
    //    Serial.print(a, HEX);
    
    // buttons
    button sb1;
    sb1.dDefine(_pscreen, _x0, _y0, 60, 40-2, setItem( 1, okText0), okTextColour0, okColour0);
    sb1.enable(true);
    sb1.draw();
    
    button sb2;
    sb2.dDefine(_pscreen, _x0, _pscreen->maxY()-40+2, 60, 40-2, setItem( 2, cancelText0), cancelTextColour0, cancelColour0);
    sb2.enable(true);
    sb2.draw();
    
    // variables for value management
    uint16_t x;
    uint16_t y = map(value, min, max, _y0+40, _y0+40+k);
    uint16_t initialY = y;
    uint16_t oldY = y;
    
    // colour min, max and value
    uint8_t minRed, maxRed, valueRed;
    uint8_t minGreen, maxGreen, valueGreen;
    uint8_t minBlue, maxBlue, valueBlue;
    
    _pscreen->splitColour(minColour0, minRed, minGreen, minBlue);
    _pscreen->splitColour(maxColour0, maxRed, maxGreen, maxBlue);
    
    for ( i=0; i<k; i++ ) {
        valueRed   = map(i, 0, k, minRed, maxRed);
        valueGreen = map(i, 0, k, minGreen, maxGreen);
        valueBlue  = map(i, 0, k, minBlue, maxBlue);
        
        j = _pscreen->setColour(valueRed, valueGreen, valueBlue);
        if ( (_y0+40+i>initialY-4) && (_y0+40+i<initialY+4) ) j = _pscreen->reverseColour(j);
        
        _pscreen->dLine(_x0+4, _y0+40+i, 60-8, 1, j);
    }
    
    // slider main loop
    bool answer = false;
    
    while ( answer==0 ) {
        a = _pscreen->getTouchActivity();
        //        Serial.print(a, DEC);
        
        if ( a ) {
            if      (sb1.check()) answer = sb1.getIndex();
            else if (sb2.check()) answer = sb2.getIndex();
            else {
                _pscreen->getTouchXY(x, y);
                
                if ( (x>_x0) && (x<_pscreen->maxX()) && (y>_y0+40+4) && (y<_y0+40+k-4) ) {
                    for (i=min(oldY, y)-4+1; i<max(oldY, y)+4; i++) {
                        if ( ((i>oldY-4) && (i<oldY+4)) ^ ((i>y-4) && (i<y+4)) )
                            _pscreen->dLine(_x0+4, i, 60-8, 1, _pscreen->reverseColour(_pscreen->readPixel(_x0+4, i)));
                    }
                    oldY = y;
                }
            }
        }
        delay(100);
    }
    
    // restore initial screen
    a = 0;
    if ( _pscreen->checkSD() ) {
        if ( _pscreen->checkRAW() ) a = _pscreen->readScreenRAW((uint32_t)0, _x0, _y0 );
        else a = _pscreen->readScreenFAT("slider.tmp", _x0, _y0 );
    }
    //    Serial.print("\r\n readScreenRAW/FAT ");
    //    Serial.print(a, HEX);
    
    if ( answer==sb1.getIndex() ) {
        if ( oldY!=initialY ) value = map(oldY, _y0+40, _y0+40+k, min, max);
        return true;
    } else {
        return false;
    }
}


