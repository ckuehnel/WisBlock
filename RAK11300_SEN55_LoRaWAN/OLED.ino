// OLED RAK1921

void initOLED(void)
{
  Wire.begin();
  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print()
  //u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
  u8g2.setFont(u8g2_font_profont10_tr);
}

void printOnOLED()
{
  u8g2.clearBuffer();    // clear the internal memory
  u8g2.sendBuffer();     // transfer internal memory to the display
  u8g2.setCursor(0, 7);
  u8g2.print("Temperature "); u8g2.print(String(ambientTemperature,1)); u8g2.print(" *C");
  u8g2.setCursor(0, 15);
  u8g2.print("Humidity    "); u8g2.print(String(ambientHumidity,0)); u8g2.print(" rH");
  u8g2.setCursor(0, 23);
  u8g2.print("  PM1.0     "); u8g2.print(String(massConcentrationPm1p0,1)); u8g2.print(" ug/m3");
  u8g2.setCursor(0, 31);
  u8g2.print("  PM2.5     "); u8g2.print(String(massConcentrationPm2p5,1)); u8g2.print(" ug/m3");
  u8g2.setCursor(0, 39);
  u8g2.print("  PM4.0     "); u8g2.print(String(massConcentrationPm4p0,1)); u8g2.print(" ug/m3");
  u8g2.setCursor(0, 47);
  u8g2.print("  PM10      "); u8g2.print(String(massConcentrationPm10p0,1)); u8g2.print(" ug/m3");
  u8g2.setCursor(0, 55);
  u8g2.print("  iVOC      "); u8g2.print(String(vocIndex,0)); 
  u8g2.setCursor(0, 63);
  u8g2.print("  iNOx      "); u8g2.print(String(noxIndex,0)); 
  u8g2.sendBuffer();    // transfer internal memory to the display
}
