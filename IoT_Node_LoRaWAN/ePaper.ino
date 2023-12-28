// RAK14000  ePaper Functions

void drawtext(int16_t x, int16_t y, char *text, uint16_t text_color, uint32_t text_size)
{
  display.setCursor(x, y);
  display.setTextColor(text_color);
  display.setTextSize(text_size);
  display.setTextWrap(true);
  display.print(text);
}

void printValues()
{
  display.clearBuffer();
  display.drawRoundRect(0, 0, 250, 122, 10, EPD_BLACK);
  
  String str = "Temp = " + String(temperature,1) + " *C";
  strcpy(arr, str.c_str()); 
  drawtext(37, 35, arr, EPD_BLACK, 2);

  str = "Humi = " + String(humidity,0) + " %rH";
  strcpy(arr, str.c_str());
  drawtext(37, 55, arr, EPD_BLACK, 2);

  str = "Press = " + String(pressure,0) + " hPa";
  strcpy(arr, str.c_str()); 
  drawtext(25, 75, arr, EPD_BLACK, 2);
  //display.display(true);

}