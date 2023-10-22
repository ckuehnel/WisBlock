/*
 * File: RAK4631-R_Sensor-Test.ino
 *
 *
 */
#include <Wire.h>
#include <NCP5623.h>	          //http://librarymanager/All#NCP5623 By:RAKWireless
#include <ClosedCube_OPT3001.h> //http://librarymanager/All#OPT3001

ClosedCube_OPT3001 opt3001;
#define OPT3001_ADDRESS 0x44

NCP5623 rgb;

uint8_t AIN1 = WB_A1;   // for RAK19007

#define DEBUG 0

void setup() 
{
  uint32_t t = millis();
  uint32_t timeout = 5000;

  // waits for input in seial monitor
  Serial.begin(115200);
  while (!Serial) if (millis() - t > timeout) break;

  Wire.begin();
  Wire.setClock(100000);

  initADC();
  initRAK1401();
  initRAK1903();
}

void loop() 
{
  Serial.printf("VSolar : %5.3f V\r\n", readADC(AIN1));
  Serial.printf("VBat   : %5.3f V\r\n", api.system.bat.get());
  opt3001_get();            // read OPT3001 sensor
  Serial.println();
  blinkGreen(20);
  delay(1000);
  blinkRed(20);
  delay(1000);

}
