/*
 * File RAK11200_Modbus.ino
 * 
 * based on
 * Modbus Library for Arduino Example - Modbus RTU Client
 * Read Holding Registers from Modbus RTU Server 
 *
 * (c)2020 Alexander Emelianov (a.m.emelianov@gmail.com)
 * https://github.com/emelianov/modbus-esp8266
 *
 * 2023-07-15 Claus Kühnel info@ckuehnel.ch 
 */

#include <ModbusRTU.h>  // https://github.com/emelianov/modbus-esp8266

#define SLAVE_ID 1
#define FIRST_REG 0
#define REG_COUNT 2

ModbusRTU mb;

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) 
{
  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  return true;
}

void setup() 
{
  // WisBLOCK RAK5802 Power On
  pinMode(WB_IO2, OUTPUT);
	digitalWrite(WB_IO2, HIGH);

	Serial.begin(115200);
  delay(2000); // wait for serial monitor
  Serial1.begin(9600,SERIAL_8N1);    //Data bit 8, check bit None, Stop bit 1
  while (!Serial1);
  mb.begin(&Serial1);
  mb.master();
  Serial.println("MODBUS RTU-Master Init Succeed.");
}

void loop() 
{
  uint16_t res[REG_COUNT];
  if (!mb.slave())  // Check if no transaction in progress
  {    
    mb.readHreg(SLAVE_ID, FIRST_REG, res, REG_COUNT, cbWrite); // Send Read HOLDING reg from Modbus Server
    while(mb.slave()) // Check if transaction is active
    { 
      mb.task();
      delay(10);
    }
    Serial.printf("Humidity    %5.1f %%rH\n",res[0]/10.);
    Serial.printf("Temperature %5.1f °C\n",res[1]/10.);
  }
  delay(1000);
}