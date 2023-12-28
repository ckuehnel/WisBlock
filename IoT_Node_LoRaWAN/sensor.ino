// Simulated sensor routines for core modules

void initSensor(void)
{
  // initialize the used sensor
  randomSeed(analogRead(0)); // Starting random 
  Serial.println("Sensor initialized.");
}

bool readSensor(void)
{
  // read the used sensor, here simulated only
  temperature = 25 + random(-300, 300)/10.;
  humidity = 50 + random(-20, 20);
  pressure = 1000 + random(-100, 100);
  Serial.println("Sensor read");
  if (DEBUG) 
  {
    Serial.printf("Temperature = %3.1f °C\r\n", temperature);	
    Serial.printf("Humidity    = %3.1f °C\r\n", humidity);	
    Serial.printf("Pressure    = %5.1f °C\r\n", pressure);	 
  }
  return true;
}