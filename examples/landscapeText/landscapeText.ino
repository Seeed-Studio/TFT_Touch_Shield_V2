/* drawString example sketch: display strings all over the place */

#include <SPI.h>
#include <stdint.h>
#include <TFTv2.h>

void setup() {
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  // init TFT library
    Tft.fillScreen(0, 240, 0, 320, BLUE);

    Tft.drawString("original", 0, 14, 2, GREEN);

    TextOrientation orientation;
    Tft.drawString("portrait", 140, 300, 2, YELLOW, orientation);
    orientation = PORTRAIT_BACKWARDS;
    Tft.drawString("backwards", 128, 280, 2, YELLOW, orientation);
    orientation = PORTRAIT_UPSIDE_DOWN_BACKWARDS;
    Tft.drawString("downback", 128, 240, 2, YELLOW, orientation);
    orientation = PORTRAIT_UPSIDE_DOWN;
    Tft.drawString("upside down", 100, 200, 2, YELLOW, orientation);
    orientation = PORTRAIT_VERTICAL;
    Tft.drawString("vertical", 8, 220, 2, YELLOW, orientation);



    orientation = LANDSCAPE;
    Tft.drawString("landscape normal", 100, 18, 2, WHITE, orientation);
    orientation = LANDSCAPE_UPSIDE_DOWN;
    Tft.drawString("landscape updown", 100, 0, 2, WHITE, orientation);
    orientation = LANDSCAPE_BACKWARDS;
    Tft.drawString("landscape back", 120, 64, 2, WHITE, orientation);
    orientation = LANDSCAPE_UPSIDE_DOWN_BACKWARDS;
    Tft.drawString("landscape downback", 100, 70, 2, WHITE, orientation);
    orientation = LANDSCAPE_VERTICAL;
    Tft.drawString("landscape vertical", 0, 0, 2, WHITE, orientation);
}

void loop() {

}
