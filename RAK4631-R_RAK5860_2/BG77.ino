// Quectel BG77 Support

// Read the return value of BG77.
void BG77_read(time_t timeout)
{
  time_t timeStamp = millis();
  while((millis() - timeStamp) < timeout)
  {
    if(Serial1.available()>0) 
    {
      Serial.print((char)Serial1.read());
      delay(1);
    }
  }
}

// Write commands to BG77.
void BG77_write(const char *command) 
{
  while(*command)
  {
    Serial1.write(*command);
    command++;
  }
  Serial1.println();
}

void modemOn(void)
{
    // Module slept, wake it up
    pinMode(BG77_POWER_KEY, OUTPUT);
    digitalWrite(BG77_POWER_KEY, 0);
    delay(1000);
    digitalWrite(BG77_POWER_KEY, 1);
    delay(2000);
    digitalWrite(BG77_POWER_KEY, 0);
    delay(1000);
}

void modemOff(void)
{
     // Module active, switch off
  command = "AT+QPOWD=0";  // Immediate power down
	BG77_write(command.c_str());
  BG77_read(1000);
}

uint8_t getRSSI(void)
{
  uint8_t signalStrength = 99;
  do
  {
    // Send the "AT+CSQ" command to the modem
    command = "AT+CSQ\r";
	  BG77_write(command.c_str());
    delay(1000);

    // Read the response from the modem
    String response = "";
    while (Serial1.available()) 
    {
      char c = Serial1.read();
      response += c;
    }
    Serial.print("CSQ: "); Serial.println(response);
/*
    // Check if the response contains "+CSQ:"
    if (response.indexOf("+CSQ:") != -1) 
    {
      // Extract the signal strength value from the response
      signalStrength = response.substring(response.indexOf(":") + 1).toInt();

      // Check if the signal strength is not equal to 99
      if (signalStrength != 99) {
      Serial.println("Signal strength is not equal to 99. Exiting loop.");
    } else {
      Serial.println("Signal strength is still 99. Will query again.");
    }
    } else Serial.println("Error: No +CSQ response found.");
*/
    signalStrength = response.substring(response.indexOf(":") +1).toInt();
    delay(1000); // Wait for a few seconds before the next query
  } while (signalStrength == 99);
  return signalStrength;
}

void modemConnect(void)
{
  command = "AT+CFUN=0\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+CGDCONT=1,\"IP\",\"emnify\"\r";
	BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+CFUN=1\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+CGATT=1\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+CEREG=4\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+CEREG?\r";
  BG77_write(command.c_str());
  BG77_read(2000);
  
  command = "AT+CPIN?\r";
  BG77_write(command.c_str());
  BG77_read(2000);
  
  command = "AT+QNWINFO\r";
  BG77_write(command.c_str());
  BG77_read(2000);
  
  /*command = "AT+QCSQ\r";
  BG77_write(command.c_str());
  BG77_read(2000);*/

  command = "AT+COPS?\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  /*command = "AT+CSQ\r";
  BG77_write(command.c_str());
  BG77_read(2000);*/

  int RSSI = getRSSI();
  Serial.printf("RSSI: %d\n", RSSI);

  /*command = "AT+QMTOPEN=?\r";
  BG77_write(command.c_str());
  BG77_read(2000);*/

  command = "AT+QMTOPEN=0,\"broker.hivemq.com\",1883\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+QMTOPEN?\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  /*command = "AT+QMTCONN=?\r";
  BG77_write(command.c_str());
  BG77_read(2000);*/

  command = "AT+QMTCONN=0,\"RAK5860_MQTT\"\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  /*command = "AT+QMTSUB=?\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+QMTSUB=0,1,\"topic/example\",2\r";
  BG77_write(command.c_str());
  BG77_read(2000);*/

  command = "AT+QMTSUB=0,1,\"topic/pub\",0\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  /*command = "AT+QMTUNS=0,1,\"topic/example\"\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  command = "AT+QMTPUB=?\r";
  BG77_write(command.c_str());
  BG77_read(2000);*/
}