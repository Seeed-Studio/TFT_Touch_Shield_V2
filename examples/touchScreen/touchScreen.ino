#include <stdint.h>
#include <SeeedTouchScreen.h>

#define ShowSerial Serial

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // mega
    #define YP A2   // must be an analog pin, use "An" notation!
    #define XM A1   // must be an analog pin, use "An" notation!
    #define YM 54   // can be a digital pin, this is A0
    #define XP 57   // can be a digital pin, this is A3

#elif defined(__AVR_ATmega32U4__) // leonardo
    #define YP A2   // must be an analog pin, use "An" notation!
    #define XM A1   // must be an analog pin, use "An" notation!
    #define YM 18   // can be a digital pin, this is A0
    #define XP 21   // can be a digital pin, this is A3
#elif defined(ARDUINO_SAMD_VARIANT_COMPLIANCE) // samd21

    #define YP A2   // must be an analog pin, use "An" notation!
    #define XM A1   // must be an analog pin, use "An" notation!
    #define YM A4   // can be a digital pin, this is A0
    #define XP A3   // can be a digital pin, this is A3
    #undef ShowSerial
    #define ShowSerial SerialUSB

#else //168, 328, something else
    #define YP A2   // must be an analog pin, use "An" notation!
    #define XM A1   // must be an analog pin, use "An" notation!
    #define YM 14   // can be a digital pin, this is A0
    #define XP 17   // can be a digital pin, this is A3

#endif

//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1

#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2


// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// The 2.8" TFT Touch shield has 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM);

void setup(void) {
    ShowSerial.begin(9600);
}

void loop(void) {
    // a point object holds x y and z coordinates
    Point p = ts.getPoint();

    if (p.z > __PRESSURE) {
        ShowSerial.print("Raw X = "); ShowSerial.print(p.x);
        ShowSerial.print("\tRaw Y = "); ShowSerial.print(p.y);
        ShowSerial.print("\tPressure = "); ShowSerial.println(p.z);
    }


    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);

    // we have some minimum pressure we consider 'valid'
    // pressure of 0 means no pressing!
    if (p.z > __PRESSURE) {
        ShowSerial.print("X = "); ShowSerial.print(p.x);
        ShowSerial.print("\tY = "); ShowSerial.print(p.y);
        ShowSerial.print("\tPressure = "); ShowSerial.println(p.z);
    }

    delay(100);
}
