/*
  TFT Touch Shield 2.0 examples - tftbmp2
  
  loovee
  2013-1-21
  
  This demo can show all bmp files in root Directory of SD card,
  regardless of the image size. 

  MAX_BMP specifies the maximum number of files to display
  FILENAME_LEN specifies the maximum length of the file name
  

  Modified by DFBrown (dfbrown@rtx.com)
  2016-1-19
  
  This modified demo program will display virtually any appropriate BMP
  image file, including those with bit depth of 1 byte/pixel (monochrome)
  and those with bit depth of 3 bytes/pixel (color).
  
  If the BMP image is smaller than the display screen
  (width and height), it will be centered on the screen.
  
  If the BMP image is larger than the display screen 
  (width and/or height), it will be shrunk to fit.  Shrinking
  is done by reducing image resolution (skipping over pixles)
  in such a way as to maintain the image aspect ratio.
  
  In addition to UP and DOWN image display, IN and OUT displaying
  are new options.  IN causes the screen to be swept from the top 
  and bottom of the screen at the same time so that the image sweeps
  in toward the center.  OUT is the reverse of IN.
  
*/

#include <SD.h>
#include <SPI.h>
#include <Streaming.h>

#include "TFTv2.h"

#define MAX_BMP         25      // Maximum number of BMP files
#define FILENAME_LEN    16      // Maximum file name length

#define SKETCH "TFT_Mod1 / Rev2" 

// Determine how a file will be displayed, sweeping the screen vertically:
#define DOWN 0           // display from top to bottom
#define UP   1           // display from bottom to top
#define IN   2           // display from top/bottom in to center
#define OUT  3           // display from center out to top/bottom

const int PIN_SD_CS = 4;           // pin of sd card

// Display pixel size, as defined in TFTv2.h
uint32_t DspHeight = MAX_Y+1;
uint32_t DspWidth  = MAX_X+1;
float    DspAr = (float)DspHeight/(float)DspWidth;  // Aspect Ratio

// Specify the BMP file read buffer  
#define BUFFPIXEL 400
uint8_t  SdBuffer[BUFFPIXEL*3];    // Allow for color depth of 3 bytes/pixel
uint16_t PixelColor[MAX_X+1];

// BMP File specifcations
uint16_t NumBmpFiles = 0;              // Number of BMP files
char BmpFiles[MAX_BMP][FILENAME_LEN];  // List of image files
char *fileName;                        // Current file on display
uint32_t fileSize;
uint32_t BmpImageOffset  = 0;          // Offset of image in the BMP file
uint32_t BmpHeight;                    // BMP image pixel height
uint32_t BmpWidth;                     // BMP image pixel width
uint8_t  BmpDepth;
File BmpFile;

// search root to find bmp file
void searchDirectory()
{
  File root = SD.open("/");                       // root

  while (NumBmpFiles < MAX_BMP) 
  {
    File entry =  root.openNextFile();
    if (!entry)  break;

    if (!entry.isDirectory())
    {
      String name = entry.name();
      int len = name.length();

      if (len >= 5 && len <= 12)
      {
        strcpy(BmpFiles[NumBmpFiles], name.c_str());
        name.toUpperCase();
        if (name.endsWith(".BMP"))  NumBmpFiles++;
      }
    }

    entry.close();
  }
  
  Serial.print("get ");
  Serial.print(NumBmpFiles);
  Serial.println(" files: ");
  
  for (int i = 0; i < NumBmpFiles; i++)
    Serial.println(BmpFiles[i]);
    
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  Serial.println(F(SKETCH));
  
  pinMode(PIN_SD_CS,OUTPUT);

  Tft.TFTinit();

  Sd2Card card;
  card.init(SPI_FULL_SPEED, PIN_SD_CS); 
  
  if (!SD.begin(PIN_SD_CS)) 
  { 
    Serial.println("SD failed!");
    while(1);                               // init fail, die here
  }
  
  Serial.println("SD OK!");
  
  searchDirectory();

  TFT_BL_ON;
}

void loop()
{
  static int dirCtrl = UP;  
  
  for (unsigned char i = 0; i < NumBmpFiles; i++)
  {
    fileName = BmpFiles[i];
    BmpFile = SD.open(fileName);
    
    if (!BmpFile)
    {
      Serial.print("didnt find image: ");
      Serial.println(fileName);
      break;
    }

    if (!bmpReadHeader(BmpFile)) 
    {
      Serial.println("bad bmp");
      break;
    }

    bmpdraw(BmpFile, dirCtrl);
    BmpFile.close();
    dirCtrl = (dirCtrl+1)%4;
  }
}

/*********************************************/
// This procedure reads a bitmap and draws it to the screen
// its sped up by reading many pixels worth of data at a time
// instead of just one pixel at a time. Increasing the buffer takes
// more RAM but makes the drawing a little faster. 20 pixels' worth
// is probably a good place

// dir - DOWN: sweep top to bottom
// dir - UP:   sweep bottom to top
// dir - IN:   sweep from edge to center
// dir - OUT:  sweep from center to edge

void bmpdraw(File f, int my_dir)
{
/*
 Display the BMP file.

 If the image is narrower than the screen (BmpWidth < DspWidth),
 then center the display on the width.  

 If the image is shorter than the display (BmpHeight < DspHeight),
 then center the image on the display height.

 If the image is wider than the screen (BmpWidth > DspWidth),
 then crop the image width from both sides.
 
 If the image is taller than the display (BmpHeight > DspHeight),
 then crop the image width from both top and bottom
*/
  int dir = my_dir == IN || my_dir == OUT ? UP : my_dir;

// Compute scanline padding
  uint8_t padding = 0;
  uint16_t scanlinebytes = BmpWidth*BmpDepth;
  while ((scanlinebytes+padding) % 4 != 0)  padding++;

  uint32_t time = millis();    // Milliseconds taken to display the image
           
  uint16_t offset_x = 0,       // Display Width Margin
           offset_y = 0;       // Display Height Margin
           
  float    skip = 1.0;         // Image Shrink Factor
           
  float    BmpAr = (float)BmpHeight/(float)BmpWidth;  // Image Aspect Ratio
  bool     done_y = false;
  
// First check to see if the image needs to shrink to fit onto the display:
  if (BmpWidth > DspWidth || BmpHeight > DspHeight)
  {
    if (BmpAr >= DspAr)
    {
      skip = (float)BmpHeight/(float)DspHeight;
      offset_x = (uint32_t)((float)DspWidth-(float)DspHeight/BmpAr)/2;
    }
    else
    {
      skip = (float)BmpWidth/(float)DspWidth;
      offset_y = (uint32_t)((float)DspHeight-(float)DspWidth*BmpAr)/2;
    }
  }
  else
  {
    if (BmpWidth < DspWidth)  offset_x = (DspWidth-BmpWidth)/2;
    if (BmpHeight < DspHeight)  offset_y = (DspHeight-BmpHeight)/2;
  }

#if(1)
  Serial.print(F("\nFile: "));
  Serial.println(fileName);
  Serial.print(F("Size: "));
  Serial.println(fileSize);
  Serial.print(F("dir = "));
  switch (my_dir)
  {
    case UP:   {Serial.println(F("UP"));   break;}
    case DOWN: {Serial.println(F("DOWN")); break;}
    case IN:   {Serial.println(F("IN"));   break;}
    case OUT:  {Serial.println(F("OUT"));  break;}
  }
#endif
  
#if(1)
Serial.print(F("padding: "));
Serial.println(padding);

Serial.print(F("DspWidth, DspHeight = "));
Serial.print(DspWidth);
Serial.print(F(", "));
Serial.println(DspHeight);

Serial.print(F("BmpDepth = "));
Serial.println(BmpDepth); 

Serial.print(F("BmpWidth, BmpHeight = "));
Serial.print(BmpWidth);
Serial.print(F(", "));
Serial.println(BmpHeight);

Serial.print(F("BmpAr, DspAr = "));
Serial.print(BmpAr);
Serial.print(F(", "));
Serial.println(DspAr);

Serial.print(F("skip = "));
Serial.println(skip);

Serial.print(F("offset_x, offset_y = "));
Serial.print(offset_x);
Serial.print(F(", "));
Serial.println(offset_y);
#endif

  uint16_t ydn = DspHeight - 1,
           yup = 0;
  
  float    fx = 0.0, 
           fyup = 0.0,
           fydn = (float)BmpHeight-1;
         
  if (my_dir == OUT)
  {
    yup = ydn = DspHeight/2;
    fyup = fydn = (float)BmpHeight/2;
  }

// Sweep the display vertically (up or down)
  for (uint16_t y = 0; y <= DspHeight; y++)
  {
    if (my_dir == IN || my_dir == OUT)  dir = 1-dir;

// Prefill the pixel colors with the background color (black in this case):
    memset (PixelColor, 0, DspWidth*sizeof(*PixelColor));  // Fill background black

// Display_image is true if the vertical sweep is withing the image boundaries:
    bool display_image;

  switch (my_dir)
  {
  case UP:
  case DOWN:
    display_image = y >= offset_y && y <= DspHeight-1-offset_y;
    break;
  case IN:
    display_image = yup >= offset_y && ydn <= DspHeight-1-offset_y;
    break;
  case OUT:
    display_image = ydn >= offset_y && yup <= DspHeight-1-offset_y;
  }

// Don't gather image pixels if we're not within the image boundaries.
    if (display_image)
    {
      bool done_x = false;
      uint32_t i = (dir == UP ? (uint32_t)(fyup+0.5) : (uint32_t)(fydn-0.5));
      uint32_t r = BmpImageOffset+(BmpHeight-i-1)*(BmpWidth*BmpDepth + padding);

// Sweep the image horizontally
      BmpFile.seek(r);
      
      for (uint16_t h = 0, i = 0; !done_x; h += BUFFPIXEL)
      {
        uint16_t size = BUFFPIXEL;
        fx = 0.0;

        if (h + size >= BmpWidth)  
        {
          size = BmpWidth - h;
          done_x = true;
        }

        BmpFile.read(SdBuffer, size*BmpDepth);
        
        for (uint16_t p = 0; p < size && fx+skip < (float)size; i++, p++)
        {
          fx += skip;
          uint16_t buffidx = BmpDepth*((uint16_t)fx);
          uint16_t x = i + offset_x;
        
          switch (BmpDepth)
          {
          case 1:
            PixelColor[x] = SdBuffer[buffidx]>>3;                       // red
            PixelColor[x] = PixelColor[x]<<6 | (SdBuffer[buffidx]>>2);  // green
            PixelColor[x] = PixelColor[x]<<5 | (SdBuffer[buffidx]>>3);  // blue
            break;
          case 3:
            PixelColor[x] = SdBuffer[buffidx+2]>>3;                       // red
            PixelColor[x] = PixelColor[x]<<6 | (SdBuffer[buffidx+1]>>2);  // green
            PixelColor[x] = PixelColor[x]<<5 | (SdBuffer[buffidx+0]>>3);  // blue
            break;
          }
        }
      }

      if (dir == DOWN)
        fydn -= skip;
      else
        fyup += skip;
    }

    Tft.setCol(0, DspWidth);
      
    if (dir == UP)
    {
      Tft.setPage(yup, yup);
      yup++;
    }
    else
    {
      Tft.setPage(ydn, ydn);
      ydn--;
    }

    Tft.sendCMD(0x2c);                                                  
    
    TFT_DC_HIGH;
    TFT_CS_LOW;

    for (int m = 0; m < DspWidth; m++)
    {
      SPI.transfer(PixelColor[m]>>8);
      SPI.transfer(PixelColor[m]);
    }

    TFT_CS_HIGH;
  }

  float time_sec = (millis() - time)/1000.0;
  Serial.print(time_sec);
  Serial.println(F(" seconds"));
}

boolean bmpReadHeader(File f)
{
// read header
  uint32_t tmp;

  if (read16(f) != 0x4D42) 
  {
    Serial.println(F("Incorrect BMP file signature"));
    return false;
  }

  // read file size
  fileSize = read32(f);

  // read and ignore creator bytes
  read16(f);
  read16(f);

  BmpImageOffset = read32(f);

  // read DIB header
  tmp = read32(f);
  BmpWidth  = read32(f);
  BmpHeight = read32(f);
  
  if (read16(f) != 1)
  {
    Serial.println("Planes != 1");
    return false;
  }

  BmpDepth = read16(f)/8;  // Bytes / Pixel
  if (BmpDepth != 1 && BmpDepth != 3)
  {
    Serial.println(F("Unsupported color depth"));
    return false;
  }

  if (read32(f) != 0) 
  {
    Serial.println(F("compression not supported!"));
    return false;
  }

  return true;
}

/*********************************************/
// These read data from the SD card file and convert them to big endian
// (the data is stored in little endian format!)

// LITTLE ENDIAN!
uint16_t read16(File f)
{
  uint16_t d;
  uint8_t b;
  b = f.read();
  d = f.read();
  d <<= 8;
  d |= b;
  return d;
}

// LITTLE ENDIAN!
uint32_t read32(File f)
{
  uint32_t d;
  uint16_t b;
  b = read16(f);
  d = read16(f);
  d <<= 16;
  d |= b;
  return d;
}
