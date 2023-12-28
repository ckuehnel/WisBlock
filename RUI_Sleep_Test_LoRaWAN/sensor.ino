// Simulated sensor routines for core modules

void initSensor(void)
{
  // initialize the used sensor
  Serial.println("Sensor initialized.");
}

bool readSensor(void)
{
  // read the used sensor, here simulated only
  temperature = 22.2;
  humidity = 55.5;
  pressure = 999.9;
  Serial.println("Sensor read");
  if (DEBUG) 
  {
    Serial.printf("Temperature = %3.1f °C\r\n", temperature);	
    Serial.printf("Humidity    = %3.1f °C\r\n", humidity);	
    Serial.printf("Pressure    = %5.1f °C\r\n", pressure);	 
  }
  return true;
}