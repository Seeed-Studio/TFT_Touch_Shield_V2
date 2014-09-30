#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>

void setup()
{
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  // init TFT library

    Tft.fillScreen(0, 100, 0, 100, RED);
    //Tft.fillRectangle(30, 120, 100,65,YELLOW);
    //Tft.drawRectangle(100, 170, 120,60,BLUE);
}

void loop()
{
  
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/