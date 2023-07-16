// Sensirion SEN55 connected via RAK1920 to RAK5005-0 baseboard

boolean initSEN55()
{
  Wire.begin();
  sen5x.begin(Wire);

  uint16_t error;
  char errorMessage[256];
  error = sen5x.deviceReset();
  if (error) 
  {
    Serial.print("Error trying to execute deviceReset(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }

  // set a temperature offset in degrees celsius
  // Note: supported by SEN54 and SEN55 sensors
  // By default, the temperature and humidity outputs from the sensor
  // are compensated for the modules self-heating. If the module is
  // designed into a device, the temperature compensation might need
  // to be adapted to incorporate the change in thermal coupling and
  // self-heating of other device components.
  //
  // A guide to achieve optimal performance, including references
  // to mechanical design-in examples can be found in the app note
  // “SEN5x – Temperature Compensation Instruction” at www.sensirion.com.
  // Please refer to those application notes for further information
  // on the advanced compensation settings used
  // in `setTemperatureOffsetParameters`, `setWarmStartParameter` and
  // `setRhtAccelerationMode`.
  //
  // Adjust tempOffset to account for additional temperature offsets
  // exceeding the SEN module's self heating.
  float tempOffset = 0.0;
  error = sen5x.setTemperatureOffsetSimple(tempOffset);
  if (error) 
  {
    Serial.print("Error trying to execute setTemperatureOffsetSimple(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  } 
  else 
  {
    Serial.print("Temperature Offset set to ");
    Serial.print(tempOffset);
    Serial.println(" deg. Celsius (SEN54/SEN55 only");
  }

  // Start Measurement
  error = sen5x.startMeasurement();
  if (error) 
  {
    Serial.print("Error trying to execute startMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
  if (error) return false;
  else return true;
}

void readSEN55(void)
{
  uint16_t error;
  char errorMessage[256];

  digitalWrite(LED_GREEN, HIGH);
  error = sen5x.readMeasuredValues(
      massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
      massConcentrationPm10p0, ambientHumidity, ambientTemperature, vocIndex,
      noxIndex);

  if (error) 
  {
    Serial.print("Error trying to execute readMeasuredValues(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  } 
  else 
  {
    Serial.print("PM1.0: ");
    Serial.print(massConcentrationPm1p0,1);
    Serial.print(" ug/m3\t");
    Serial.print("PM2.5: ");
    Serial.print(massConcentrationPm2p5,1);
    Serial.print(" ug/m3\t");
    Serial.print("PM4.0: ");
    Serial.print(massConcentrationPm4p0,1);
    Serial.print(" ug/m3\t");
    Serial.print("PM10.0: ");
    Serial.print(massConcentrationPm10p0,1);
    Serial.print(" ug/m3\t");
    Serial.print("Humidity: ");
    if (isnan(ambientHumidity)) Serial.print("n/a");
    else Serial.print(ambientHumidity,0);
    Serial.print(" %rH\t");
    Serial.print("Temperature: ");
    if (isnan(ambientTemperature)) Serial.print("n/a");
    else Serial.print(ambientTemperature,1);
    Serial.print("°C\t");
    Serial.print("iVOC: ");
    if (isnan(vocIndex)) Serial.print("n/a");
    else Serial.print(vocIndex,0);
    Serial.print("\t");
    Serial.print("iNOx: ");
    if (isnan(noxIndex)) Serial.println("n/a");
    else Serial.println(noxIndex,0);
    digitalWrite(LED_GREEN, LOW);
    printOnOLED();
  }
  delay(5000);
}
