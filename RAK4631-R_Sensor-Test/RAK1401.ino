//

void initRAK1401(void)
{
  // enable RAK14001
  pinMode(WB_IO6, OUTPUT);
  digitalWrite(WB_IO6, HIGH);

  if (!rgb.begin())
  {
    Serial.println("RAK14001 not found on the I2C line");
    while (1);
  }
  else
  {
    Serial.println("RAK14001 Found. Set brightness to a low value.");
  }

  // set the current output level max, the range is 1 to 31
  rgb.setCurrent(5);
}

void blinkGreen(uint8_t ms)
{
  rgb.setColor(0,255,0);    // GREEN
  delay(ms);
  rgb.setColor(0,0,0);     // OFF
}

void blinkRed(uint8_t ms)
{
  rgb.setColor(255,0,0);    // RED
  delay(ms);
  rgb.setColor(0,0,0);     // OFF
}