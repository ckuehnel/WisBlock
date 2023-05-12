// RAK14014 LCD Routines

void drawBackground(void)
{
  tft.fillScreen(ST77XX_BLACK);
  drawBmp(&bmopenweather,0,0);
  tft.setCursor(0,25);
  tft.println("Weather Conditions for");
  tft.print(city);
}

void drawCanvas(String str)
{
  canvas.fillScreen(ST77XX_BLACK); // Clear canvas (not display)
  canvas.setCursor(0, 20);  // Pos. is BASE LINE when using fonts!
  canvas.print(str);        // Print result string
  tft.drawBitmap(180, 60, canvas.getBuffer(), canvas.width(), canvas.height(), ST77XX_WHITE, ST77XX_BLACK);
}

// draw image saved in bmp.h
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