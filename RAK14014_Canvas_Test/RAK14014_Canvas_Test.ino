/*
 * File: RAK14014_Canvas_Test.ino
 * based on graphictest_ST7789.ino in library examples
 */

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Fonts/FreeSerifBold18pt7b.h>

// Pins for RAK14014 
#define CS            SS
#define BL            WB_IO3
#define RST           WB_IO5
#define DC            WB_IO4

#define GFXCANVAS16  // comment, if GFXCanvas1 is used

Adafruit_ST7789 tft = Adafruit_ST7789(CS, DC, RST);

#ifdef GFXCANVAS16
  GFXcanvas16 canvas(120, 40);  // 16-bit, 120x40 pixels -> needs 120 x 40 x 16 = 76800 Bit RAM
#else
  GFXcanvas1 canvas(120, 40);   //  1-bit, 120x40 pixels -> needs 120 x 40 x  1 =  4800 Bit RAM
#endif

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

  Serial.println(F("ST7789 Canvas Test"));
  tft.init(240, 320); // Init ST7789 240x320.
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK); // clear display
  canvas.setFont(&FreeSerifBold18pt7b); // Use custom font
  canvas.setTextWrap(false); // Clip text within canvas
  Serial.println(F("Initialized"));

  tft.setCursor(20, 20);
#ifdef GFXCANVAS16
  tft.print("Using GFXCanvas16");
#else
  tft.print("Using GFXCanvas1");
#endif

  delay(500);
}


void loop(void) 
{
#ifdef GFXCANVAS16
  canvas.fillScreen(ST77XX_GREEN); // Clear canvas (not display)
#else
 canvas.fillScreen(ST77XX_BLACK); // Clear canvas (not display)
#endif

 canvas.setCursor(0, 30); // Pos. is BASE LINE when using fonts!
 canvas.print(millis()); // Print elapsed time in milliseconds
 // Copy canvas to position (100,100)
#ifdef GFXCANVAS16
  tft.drawRGBBitmap(100, 100, canvas.getBuffer(), canvas.width(), canvas.height());
#else
  tft.drawBitmap(100, 100, canvas.getBuffer(), canvas.width(), canvas.height(), ST77XX_WHITE, ST77XX_BLACK);
#endif
  delay(100);
}

