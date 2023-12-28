/*
 * File: IoT_Node_LoRaWAN.ino
 *
 * Monitoring the power consumption of LoRaWAN nodes
 * for different core modules running RUI3.
 * WisToolBox is used for Configuration of LoRaWAN parameters.
 * Sensor data are simulated. 
 * Sleep time is 30 seconds.
 * RAK14000 ePaper display is used for presenting results.
 *
 * Select the right core module and serial port for download.
 *
 * 2023-12-19 Claus Kühnel info@ckuehnel.ch
 */
#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_EPD.h>
#include "ePaper_defines.h"

// Simulated sensor data
float temperature, humidity, pressure;

uint8_t data_array[8] = {0};
char    arr[25];

#define DEBUG 0       // DEBUG controls serial output
#define SLEEPTIME 60  // Sleep time in seconds

// DEPG0213BNS800F41HP epaper marked on back-side
// 2.13" EPD with SSD1680, width=250 pixels, heigh=122 pixels
Adafruit_SSD1680 display(250, 122, EPD_MOSI,
                         EPD_SCK, EPD_DC, EPD_RESET,
                         EPD_CS, SRAM_CS, EPD_MISO,
                         EPD_BUSY);

void drawtext(int16_t x, int16_t y, char *text, uint16_t color);

void setup()
{
  uint32_t t = millis();
  uint32_t timeout = 5000;

	pinMode(LED_GREEN, OUTPUT);
	digitalWrite(LED_GREEN, LOW);

  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH);

  pinMode(POWER_ENABLE, INPUT_PULLUP);
  digitalWrite(POWER_ENABLE, HIGH);

	Serial.begin(115200);
  while (!Serial) if (millis() - t > timeout) break;

  Serial.println("\nMonitoring Power Consumption of IoT LoRaWAN Nodes");
  Serial.println("-------------------------------------------------");

  display.begin(false);
  display.clearBuffer();
  display.drawRoundRect(0, 0, 250, 122, 10, EPD_BLACK);
  drawtext(25, 55, "WisBlock IoT Node", (uint16_t)EPD_BLACK, 2);
  display.display(true);

  Serial.printf("Set adaptive data rate %s\r\n", api.lorawan.adr.set(1) ? "Success" : "Fail");
  Serial.printf("Set the low power mode %s\n\r", api.system.lpm.set(1) ? "Success" : "Fail");
  
  // Setup the callbacks for joined and send finished
  api.lorawan.registerRecvCallback(recv_cb);
  api.lorawan.registerSendCallback(send_cb);
  api.lorawan.registerJoinCallback(join_cb);

	// Create a unified timer
  if (api.system.timer.create(RAK_TIMER_0, (RAK_TIMER_HANDLER) handler, RAK_TIMER_PERIODIC) != true) 
  {
    Serial.printf("Creating timer failed.\r\n");
    return;
  }
  // Set timer periode to 1 min and start timer
  if (api.system.timer.start(RAK_TIMER_0, 1000 * SLEEPTIME , NULL) != true) 
  {
    Serial.printf("Starting timer failed.\r\n");
    return;
  }

  api.lorawan.join();

  //wait for Join success
  while (api.lorawan.njs.get() == 0)
  {
    Serial.print("Waiting for Lorawan join...");
    api.lorawan.join();
    delay(10000);
  }
  initSensor();
  delay(100);
}

void handler(void *)
{
  digitalWrite(WB_IO2, HIGH); // Switch on SSD1680
  digitalWrite(LED_GREEN, HIGH);
  delay(20);
  digitalWrite(LED_GREEN, LOW);

  readSensor();
  printValues();

  uint16_t core = 11722;  // this is the used core module

  String str = "RAK" + String(core) + " WisBlock IoT Node";
  strcpy(arr, str.c_str()); 
  drawtext(45, 15, arr, EPD_BLACK, 1);

  int16_t temp = temperature * 10;
  uint16_t humi = humidity * 10;
  uint16_t press = pressure * 10; 
  if (DEBUG) Serial.printf("Payload: %d %d %d %d\n", core, temp, humi, press);

  data_array[0] = highByte(core);
  data_array[1] = lowByte(core);
  data_array[2] = highByte(temp);
  data_array[3] = lowByte(temp);
  data_array[4] = highByte(humi);
  data_array[5] = lowByte(humi);
  data_array[6] = highByte(press);
  data_array[7] = lowByte(press);

  //wait for Join success
  while (api.lorawan.njs.get() == 0)
  {
    Serial.print("Waiting for Lorawan join...");
    api.lorawan.join();
    delay(10000);
  }
  if (api.lorawan.send(sizeof(data_array), data_array, 2, false, 1)) 
  {
    Serial.println("Send Success");  // Port = 2, confirmation false, retry = 1
    drawtext(45, 100, "Message sent successfully.", EPD_BLACK, 1);
    display.display(true);
  }
  else 
  {
    Serial.println("Send fail");
    drawtext(45, 100, "Message sending failed.", EPD_BLACK, 1);
    display.display(true);
  }
  digitalWrite(WB_IO2, LOW); // Switch off SSD1680
}

// This example is complete timer driven. 
// The loop() does nothing than sleep.
void loop()
{
	api.system.sleep.all();
  api.system.scheduler.task.destroy();
}