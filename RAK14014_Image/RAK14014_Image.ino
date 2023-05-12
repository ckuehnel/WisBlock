/*
 * File RAK14014_Image.ino
 */

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "bmp.h"            // Sketch tab header for bmp image

// Pins for RAK14014 
#define CS            SS
#define BL            WB_IO3
#define RST           WB_IO5
#define DC            WB_IO4

Adafruit_ST7789 tft = Adafruit_ST7789(CS, DC, RST);

void setup(void) 
{
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH);
  
  pinMode(BL, OUTPUT);
  digitalWrite(BL, HIGH); // Enable the backlight, you can also adjust the backlight brightness through PWM.

  time_t timeout = millis();
  Serial.begin(115200);
  while (!Serial)
  {
    if ((millis() - timeout) < 5000) delay(100);
    else break;
  }

  Serial.println(F("RAK14014 ST7789 Image Test"));
  tft.init(240, 320); // Init ST7789 240x320.
  tft.setRotation(3);
  Serial.println(F("Initialized"));

  delay(500);

  tft.fillScreen(ST77XX_BLACK);
  drawBmp(&bmopenweather,0,0);

}

void loop() 
{
  delay(2000);
  tft.fillScreen(ST77XX_BLACK);
  delay(1000);
  drawBmp(&bmopenweather,0,0);
}

static void drawBmp(const GUI_BITMAP *bmp , uint8_t x, uint8_t y) 
{
  uint16_t color = bmp->date[0];
  uint32_t count = 0;
  uint64_t bufSize = bmp->xSize * bmp->ySize;
  tft.startWrite();
  tft.setAddrWindow(x, y, bmp->xSize, bmp->ySize);

  for ( uint64_t i = 0 ; i < bufSize ; i++ ) 
  {
    if(color == bmp->date[i])
    {
      count++;
    }
    else
    {
      tft.writeColor(color,count); 
      count = 1;
      color = bmp->date[i];
    }
  }
  tft.writeColor(color,count); 
  tft.endWrite();
}