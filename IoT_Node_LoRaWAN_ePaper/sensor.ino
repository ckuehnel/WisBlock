// RAK1901 sensor routines for core modules

boolean initRAK1901(void)
{
  if (sht.init()) 
  {
       //Serial.print("init(): success\n");
       sht.setAccuracy(SHTSensor::SHT_ACCURACY_HIGH);
       RAK1901_ready = true;
  } 
  else 
  {
    Serial.println("RAK1901 not detected");
    RAK1901_ready = false;
  }
  return RAK1901_ready;
}

void readRAK1901(void)
{
  if (sht.readSample()) 
  {
    RAK1901_temp = sht.getTemperature();
    RAK1901_humi = sht.getHumidity();
   
    if (DEBUG) Serial.printf("RAK1901 : Temp = %3.1f °C, Humi = %3.0f %%rH\n", RAK1901_temp, RAK1901_humi);
  } 
  else Serial.println("Error in readSample()");
}