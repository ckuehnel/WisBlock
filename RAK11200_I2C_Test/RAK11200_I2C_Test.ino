/*
 * File: RAK11200_I2C_Test.ino
 * Test I2C bus on RAK11200
 * 
 */
#include <Wire.h>
#include <i2cdetect.h>

#define DEBUG 1


void setup() 
{
  // Sensor power on.
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH); // set 3V3_S 
 
  Serial.begin(115200);
  delay(2000);              // wait for serial monitor
  if (DEBUG) Serial.println(F("Initializing..."));
  Serial.println("Test I2C Bus...");
  Wire.begin();
  Wire.setClock(400000);
   if (DEBUG) Serial.println(F("Running...")); //last line in setup()
}

void loop() 
{
  Serial.println("\nScanning I2C Bus");
  i2cdetect(1,127);
  delay(5000);
}
