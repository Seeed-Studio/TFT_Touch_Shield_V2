/*
    2012 Copyright (c) Seeed Technology Inc.

    Authors: Albert.Miao & Loovee,
    Visweswara R (with initializtion code from TFT vendor)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
#ifndef TFTv2_h
#define TFTv2_h

#if defined(ARDUINO) && ARDUINO >= 100
    #define SEEEDUINO
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif
#include <avr/pgmspace.h>
#if KENDRYTE_K210
    #include <SPIClass.h>
#else
    #include <SPI.h>
#endif

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
    #define SYS_VOL   3.3
#else
    #define SERIAL Serial
    #define SYS_VOL   5
#endif

//Basic Colors
#define RED		0xf800
#define GREEN	0x07e0
#define BLUE	0x001f
#define BLACK	0x0000
#define YELLOW	0xffe0
#define WHITE	0xffff

//Other Colors
#define CYAN		0x07ff
#define BRIGHT_RED	0xf810
#define GRAY1		0x8410
#define GRAY2		0x4208

//TFT resolution 240*320
#define MIN_X	0
#define MIN_Y	0
#define MAX_X	239
#define MAX_Y	319

#if defined(__LINKIT_ONE__) || defined(ARDUINO_SAMD_VARIANT_COMPLIANCE)

    #define TFT_DC_LOW      digitalWrite(6, LOW)
    #define TFT_DC_HIGH     digitalWrite(6, HIGH)
    #define TFT_CS_LOW      digitalWrite(5, LOW)
    #define TFT_CS_HIGH     digitalWrite(5, HIGH)
    #define TFT_BL_OFF      digitalWrite(7, LOW)
    #define TFT_BL_ON       digitalWrite(7, HIGH)
#elif defined(KENDRYTE_K210)
    #define TFT_DC_LOW      digitalWrite(22, LOW)
    #define TFT_DC_HIGH     digitalWrite(22, HIGH)
    #define TFT_CS_LOW      digitalWrite(35, LOW)
    #define TFT_CS_HIGH     digitalWrite(35, HIGH)
    #define TFT_BL_OFF      digitalWrite(21, LOW)
    #define TFT_BL_ON       digitalWrite(21, HIGH)

#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

    #define TFT_CS_LOW  {DDRE |= 0x08;PORTE &=~ 0x08;}
    #define TFT_CS_HIGH {DDRE |= 0x08;PORTE |=  0x08;}
    #define TFT_DC_LOW  {DDRH |= 0x08;PORTH &=~ 0x08;}
    #define TFT_DC_HIGH {DDRH |= 0x08;PORTH |=  0x08;}
    #define TFT_BL_OFF  {DDRH |= 0x10;PORTH &=~ 0x10;}
    #define TFT_BL_ON   {DDRH |= 0x10;PORTH |=  0x10;}

    #define YP A2   // must be an analog pin, use "An" notation!
    #define XM A1   // must be an analog pin, use "An" notation!
    #define YM 54   // can be a digital pin, this is A0
    #define XP 57   // can be a digital pin, this is A3

#elif defined(__AVR_ATmega32U4__)

    #define TFT_CS_LOW  {DDRC |= 0x40;PORTC &=~ 0x40;}
    #define TFT_CS_HIGH {DDRC |= 0x40;PORTC |=  0x40;}
    #define TFT_DC_LOW  {DDRD |= 0x80;PORTD &=~ 0x80;}
    #define TFT_DC_HIGH {DDRD |= 0x80;PORTD |=  0x80;}
    #define TFT_BL_OFF  {DDRE |= 0x40;PORTE &=~ 0x40;}
    #define TFT_BL_ON   {DDRE |= 0x40;PORTE |=  0x40;}

    #define YP A2   // must be an analog pin, use "An" notation!
    #define XM A1   // must be an analog pin, use "An" notation!
    #define YM 18   // can be a digital pin, this is A0
    #define XP 21   // can be a digital pin, this is A3

#elif defined(__arc__) /* Arduino101/Genuino101 specifics */

    #define TFT_DC_LOW      digitalWrite(6, LOW)
    #define TFT_DC_HIGH     digitalWrite(6, HIGH)
    #define TFT_CS_LOW      digitalWrite(5, LOW)
    #define TFT_CS_HIGH     digitalWrite(5, HIGH)
    #define TFT_BL_OFF      digitalWrite(7, LOW)
    #define TFT_BL_ON       digitalWrite(7, HIGH)

#else
    #define TFT_CS_LOW  {DDRD |= 0x20;PORTD &=~ 0x20;}
    #define TFT_CS_HIGH {DDRD |= 0x20;PORTD |=  0x20;}
    #define TFT_DC_LOW  {DDRD |= 0x40;PORTD &=~ 0x40;}
    #define TFT_DC_HIGH {DDRD |= 0x40;PORTD |=  0x40;}
    #define TFT_BL_OFF  {DDRD |= 0x80;PORTD &=~ 0x80;}
    #define TFT_BL_ON   {DDRD |= 0x80;PORTD |=  0x80;}

    #define YP A2   // must be an analog pin, use "An" notation!
    #define XM A1   // must be an analog pin, use "An" notation!
    #define YM 14   // can be a digital pin, this is A0
    #define XP 17   // can be a digital pin, this is A3

#endif

#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2

#ifndef INT8U
    #define INT8U unsigned char
#endif

#ifndef INT16U
    #define INT16U unsigned short
#endif

extern INT8U simpleFont[][8];

enum TextOrientation {
    PORTRAIT, // aka. normal mode
    PORTRAIT_BACKWARDS,
    PORTRAIT_UPSIDE_DOWN,
    PORTRAIT_UPSIDE_DOWN_BACKWARDS,
    PORTRAIT_VERTICAL,
    LANDSCAPE,
    LANDSCAPE_BACKWARDS,
    LANDSCAPE_UPSIDE_DOWN,
    LANDSCAPE_UPSIDE_DOWN_BACKWARDS,
    LANDSCAPE_VERTICAL,
};

class TFT {

  public:

    inline void sendCMD(INT8U index) {
        TFT_DC_LOW;
        TFT_CS_LOW;
        SPI.transfer(index);
        TFT_CS_HIGH;
    }

    inline void WRITE_DATA(INT8U data) {
        TFT_DC_HIGH;
        TFT_CS_LOW;
        SPI.transfer(data);
        TFT_CS_HIGH;
    }

    inline void sendData(INT16U data) {
        INT8U data1 = data >> 8;
        INT8U data2 = data & 0xff;
        TFT_DC_HIGH;
        TFT_CS_LOW;
        SPI.transfer(data1);
        SPI.transfer(data2);
        TFT_CS_HIGH;
    }

    void WRITE_Package(INT16U* data, INT8U howmany) {
        INT16U  data1 = 0;
        INT8U   data2 = 0;

        TFT_DC_HIGH;
        TFT_CS_LOW;
        INT8U count = 0;
        for (count = 0; count < howmany; count++) {
            data1 = data[count] >> 8;
            data2 = data[count] & 0xff;
            SPI.transfer(data1);
            SPI.transfer(data2);
        }
        TFT_CS_HIGH;
    }

    INT8U Read_Register(INT8U Addr, INT8U xParameter) {
        INT8U data = 0;
        sendCMD(0xd9);                                                      /* ext command                  */
        WRITE_DATA(0x10 + xParameter);                                      /* 0x11 is the first Parameter  */
        TFT_DC_LOW;
        TFT_CS_LOW;
        SPI.transfer(Addr);
        TFT_DC_HIGH;
        data = SPI.transfer(0);
        TFT_CS_HIGH;
        return data;
    }


    void TFTinit(void);
    void setCol(INT16U StartCol, INT16U EndCol);
    void setPage(INT16U StartPage, INT16U EndPage);
    void setXY(INT16U poX, INT16U poY);
    void setPixel(INT16U poX, INT16U poY, INT16U color);

    void fillScreen(INT16U XL, INT16U XR, INT16U YU, INT16U YD, INT16U color);
    void fillScreen(void);
    INT8U readID(void);

    void drawCharLandscape(INT8U ascii, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);
    void drawCharLandscapeBackwards(INT8U ascii, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);
    void drawCharLandscapeUpsideDown(INT8U ascii, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);
    void drawCharLandscapeUpsideDownBackwards(INT8U ascii, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);
    void drawCharLandscapeVertical(INT8U ascii, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);

    void drawCharPortrait(INT8U ascii, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);
    void drawCharPortraitUpsideDown(INT8U ascii, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);
    void drawCharPortraitVertical(INT8U ascii, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);
    void drawCharPortraitBackwards(INT8U ascii, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);
    void drawCharPortraitUpsideDownBackwards(INT8U ascii, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);

    // void drawChar(INT8U ascii,INT16U poX, INT16U poY,INT16U size, INT16U fgcolor, TextOrientation orientation);
    // void drawChar(INT8U ascii,INT16U poX, INT16U poY,INT16U size, INT16U fgcolor) {
    // TextOrientation orientation = PORTRAIT;
    // drawChar(ascii, poX, poY, size, fgcolor, orientation);
    // }
    void drawString(char* string, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor, TextOrientation orientation);
    void drawString(char* string, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor) {
        TextOrientation orientation = PORTRAIT;
        drawString(string, poX, poY, size, fgcolor, orientation);
    }
    // 	void drawString(const char *string,INT16U poX, INT16U poY,INT16U size,INT16U fgcolor){
    // 		TextOrientation orientation = PORTRAIT;
    // 		drawString((char*)string, poX, poY, size, fgcolor, orientation);
    // 	}

    void fillRectangle(INT16U poX, INT16U poY, INT16U length, INT16U width, INT16U color);

    void drawLine(INT16U x0, INT16U y0, INT16U x1, INT16U y1, INT16U color);
    void drawVerticalLine(INT16U poX, INT16U poY, INT16U length, INT16U color);
    void drawHorizontalLine(INT16U poX, INT16U poY, INT16U length, INT16U color);
    void drawRectangle(INT16U poX, INT16U poY, INT16U length, INT16U width, INT16U color);

    void drawCircle(int poX, int poY, int r, INT16U color);
    void fillCircle(int poX, int poY, int r, INT16U color);

    void drawTriangle(int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, INT16U color);
    INT8U drawNumber(long long_num, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);
    INT8U drawFloat(float floatNumber, INT8U decimal, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);
    INT8U drawFloat(float floatNumber, INT16U poX, INT16U poY, INT16U size, INT16U fgcolor);

  private:

    void reverse(char* string);

};

extern TFT Tft;

#endif

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
