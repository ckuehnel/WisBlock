// Use internal ADC

float ref = 3.3;        // for RAK4631

void initADC(void)
{
  //analogReference(UDRV_ADC_MODE_DEFAULT); // for RAK4631 reference is 3.6 V
  analogReadResolution(12);
}

float readADC(uint8_t pin)
{
  uint16_t adc_value = analogRead(pin);
  float adc_voltage = ref*(((float)adc_value)/4096.)*2.5/1.5;
  if (false) 
  {
    Serial.printf("ADC value = %d\r\n", adc_value);	
    // This is the formula to get the input voltage of RAK5811:
    Serial.printf("Voltage value = %f V\r\n", adc_voltage);
  }
  return adc_voltage;
}