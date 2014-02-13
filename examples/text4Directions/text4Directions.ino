// Draw Texts in the four directions of the TFT by Frankie.Chu
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
/*
    Modified record:
    
*/
#include <stdint.h>
#include <TouchScreen.h> 
#include <TFT.h>

#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 14   // can be a digital pin, this is A0
  #define XP 17   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif 
void setup()
{

  Tft.init();  //init TFT library
  Tft.setDisplayDirect(LEFT2RIGHT);
  Tft.drawString("UP",20,20,4,WHITE);
  Tft.setDisplayDirect(RIGHT2LEFT);
  Tft.drawString("DOWN",220,220,4,WHITE);
  Tft.setDisplayDirect(DOWN2UP);
  Tft.drawString("LEFT",20,220,4,WHITE);
  Tft.setDisplayDirect(UP2DOWN);
  Tft.drawString("RIGHT",220,20,4,WHITE);
}

void loop()
{
  

}
