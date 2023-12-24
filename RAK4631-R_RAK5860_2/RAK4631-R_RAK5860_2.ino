/*
 * File: RAK4631-R_RAK5860_2.ino
 *
 * Monitoring the voltage of the solar cell and battery 
 * to get information on the battery's state of charge
 * Sleep time is 5 minutes.
 * After wakeup the MCU is reset.
 *
 * 2023-11-06 Claus Kühnel info@ckuehnel.ch
 */

#include "Arduino.h"

#define BG77_POWER_KEY WB_IO1

const uint8_t AIN1 = WB_A1;   // for RAK19007

String command; // String to store BG77 commands.

void setup()
{
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, LOW);
  
  time_t serial_timeout = millis();
  Serial.begin(115200);
  while (!Serial)
  {
    if ((millis() - serial_timeout) < 5000) delay(100);
    else break;
  }

  Serial.println("\nMonitoring Vsol & Vbat - NB-IoT output");
  Serial.println("---------------------------------------");
  // To avoid the device is set to LoRaWAN mode and it is registered on a LNS and can connect to it.
  // After that the device will put the LoRa transceiver in its lowest power mode (sleep).
  Serial.printf("Set Node device work mode %s\r\n", api.lorawan.nwm.set(0) ? "Success" : "Fail");
  Serial.printf("P2P set Rx mode %s\r\n",api.lorawan.precv(0) ? "Success" : "Fail");
  Serial.printf("Set Low Power Mode %s\r\n",api.system.lpm.set(1) ? "Success" : "Fail");

  // Create a unified timer
  if (api.system.timer.create(RAK_TIMER_0, (RAK_TIMER_HANDLER) handler, RAK_TIMER_PERIODIC) != true) 
  {
    Serial.printf("Creating timer failed.\r\n");
    return;
  }
  // Set timer periode to 5 min and start timer
  if (api.system.timer.start(RAK_TIMER_0, 1000 * 300 , NULL) != true) 
  {
    Serial.printf("Starting timer failed.\r\n");
    return;
  }
  
  // Check if the modem is already awake
  time_t timeout = millis();
  bool moduleSleeps = true;
  Serial1.begin(115200);
  delay(1000);

  initADC();

  Serial1.println("ATI");
  
  //BG77 init
  while ((millis() - timeout) < 4000)
  {
    if (Serial1.available())
    {
      String result = Serial1.readString();
      Serial.println("Modem response after start:");
      Serial.println(result);
      moduleSleeps = false;
    }
  }
  if (moduleSleeps) modemOn();
  
  Serial.println("BG77 power up!");
  delay(1000);

  modemConnect();

  float Vsolar = readADC(AIN1);
  float Vbat = api.system.bat.get();
  Serial.printf("VSolar : %5.3f V\r\n", Vsolar);
  Serial.printf("VBat   : %5.3f V\r\n", Vbat);

  command = "AT+QMTPUB=0,0,0,0,\"topic/pub\"\r";
  BG77_write(command.c_str());
  BG77_read(2000);

  //command = "Hello RAKWireless\r\032";
  command = "{\"Vsolar\":" + String(Vsolar,3) + "," + "\"Vbat\":" + String(Vbat, 3) + "}\r\032";
  BG77_write(command.c_str());
  BG77_read(2000);

  Serial.println("Switch modem off");
  modemOff();
}

void handler(void *)
{
  Serial.println("Wakeup by timer and restart device.");
  api.system.reboot();  // Reboot after 300 seconds
}

// This example is complete timer driven. 
// The loop() does nothing than sleep.
void loop()
{
  Serial.println("RAK4631-R goes to sleep...");
  delay(1000);
  api.system.sleep.all();
  api.system.scheduler.task.destroy();
}