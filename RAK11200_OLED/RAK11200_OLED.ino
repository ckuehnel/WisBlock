#include <Wire.h>
#include <U8g2lib.h>		   // Click to install library: http://librarymanager/All#u8g2

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

void setup()
{
  // Initialize Serial for debug output
  Serial.begin(115200);
  Wire.begin();
  u8g2.begin();
  //u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
  u8g2.setFont(u8g2_font_profont10_tr);
}

void loop()
{
  Serial.println("Test WisBlock Display");
  u8g2.clearBuffer();					        // clear the internal memory
  u8g2.sendBuffer();                  // transfer internal memory to the display
  delay(1000);
  u8g2.drawStr(0, 7, "WisBlock Display 0");
  u8g2.drawStr(0, 15, "WisBlock Display 1");
  u8g2.drawStr(0, 23, "WisBlock Display 2");
  u8g2.drawStr(0, 31, "WisBlock Display 3");
  u8g2.drawStr(0, 39, "WisBlock Display 4");
  u8g2.drawStr(0, 47, "WisBlock Display 5");
  u8g2.drawStr(0, 55, "WisBlock Display 6");
  u8g2.drawStr(0, 63, "WisBlock Display 7");
  u8g2.sendBuffer();                  // transfer internal memory to the display
  delay(1000);
}
