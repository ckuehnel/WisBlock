/*
 * File: RAK11200_Pushover.ino
 * 
 * Program for sending pushover messages
 * 
 * 2023-07-16 Claus Kühnel info@ckuehnel.ch
 */
#include <WiFi.h>
#include "arduino_secrets.h"

WiFiClient client;

void setup() 
{
  Serial.begin(115200);
  delay(2000); // wait for serial monitor
  Serial.printf("\nRAK11200 sends Pushover messages to mobile phone...\n");
  initWiFi();
  pinMode(LED_BLUE, OUTPUT); digitalWrite(LED_BLUE, LOW);
} 
  
void loop() 
{ 
  digitalWrite(LED_BLUE, HIGH);
  sendMessage();
  digitalWrite(LED_BLUE, LOW);
  delay(10000);  
}
