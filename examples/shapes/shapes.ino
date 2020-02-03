// Draw Boxes - Demonstrate drawRectangle and fillRectangle

#include <stdint.h>
#include <TFTv2.h>

void setup() {
    SERIAL.begin(115200);
    TFT_BL_ON;                                          // turn on the background light
    Tft.TFTinit();                                      // init TFT library
    SERIAL.println("TFT init ....");
}

void loop() {
    for (int r = 0; r < 115; r = r + 2) {               //set r : 0--115
        Tft.drawCircle(119, 160, r, random(0xFFFF));    //draw circle, center:(119, 160), color: random
    }
    delay(10);
    SERIAL.println("loop ....");
}
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
