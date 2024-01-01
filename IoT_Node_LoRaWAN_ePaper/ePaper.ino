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
  
  String str = "Temp = " + String(RAK1901_temp,1) + " *C";
  strcpy(arr, str.c_str()); 
  drawtext(37, 35, arr, EPD_BLACK, 2);

  str = "Humi = " + String(RAK1901_humi,0) + " %rH";
  strcpy(arr, str.c_str());
  drawtext(37, 55, arr, EPD_BLACK, 2);

  str = "VBat = " + String(Voltage,3) + " V";
  strcpy(arr, str.c_str()); 
  drawtext(37, 75, arr, EPD_BLACK, 2);
  //display.display(true);

}