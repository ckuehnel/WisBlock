/*
 * OpenWeather Data by RAKwireless WisBlock Modules
 * 
 * based on https://randomnerdtutorials.com/esp32-http-get-open-weather-map-thingspeak-arduino/
 * used hardware 
 *    RAK11200 ESP32 based core module
 *    RAK14014 TFT display w/ touchscreen
 * 2023-05-11 Claus Kuehnel info@ckuehnel.ch
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include "bmp.h"            // Sketch tab header for bmp image
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"

char* ssid     = SECRET_SSID;
char* password = SECRET_PASS;
char* appid    = SECRET_APPID;

#define DEBUG 0  // Select debug mode by 1

// Pins for RAK14014 
#define CS            SS
#define BL            WB_IO3
#define RST           WB_IO5
#define DC            WB_IO4

Adafruit_ST7789 tft = Adafruit_ST7789(CS, DC, RST);
GFXcanvas1 canvas(160, 120);   //  1-bit, 160x120 pixels -> needs 160 x 120 x  1 =  19200 Bit RAM

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 2 minutes (120000)
unsigned long timerDelay = 120000;  // 1000 calls/day free
// Set timer to 10 seconds (10000)
//unsigned long timerDelay = 10000;

StaticJsonDocument<1024> doc;
String jsonBuffer;

bool firstTime = true;

void setup() 
{
  pinMode(WB_IO2, OUTPUT);
  digitalWrite(WB_IO2, HIGH);
  
  pinMode(BL, OUTPUT);
  digitalWrite(BL, HIGH); // Enable the backlight, you can also adjust the backlight brightness through PWM.

  time_t timeout = millis();
  Serial.begin(115200);
  while (!Serial)
  {
    if ((millis() - timeout) < 5000) delay(100);
    else break;
  };

  Serial.println(F("WisBlock Weather Station"));
  tft.init(240, 320); // Init ST7789 240x320.
  tft.setRotation(3);
  tft.setFont(&FreeSans9pt7b); // Use custom font
  drawBackground();

  canvas.setFont(&FreeSansBold12pt7b); // Use custom font
  canvas.setTextWrap(false); // Clip text within canvas
  
  Serial.println(F("Initialized"));

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
}

void loop() 
{
  // Send an HTTP GET request
  if (((millis() - lastTime) > timerDelay) || firstTime) 
  {
    firstTime = false;

    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&units=metric" + "&APPID=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);

      DeserializationError error = deserializeJson(doc, jsonBuffer);

      if (error) 
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }

      JsonObject main = doc["main"];
      float temp = main["temp"]; // 14.72
      int press = main["pressure"]; // 1012
      int humi = main["humidity"]; // 66

      JsonObject wind = doc["wind"];
      float windspeed = wind["speed"]; // 2.71

      Serial.printf("Temperature: %.1f °C\n", temp);
      Serial.printf("Pressure:  %d hPa\n", press); 
      Serial.printf("Humidity: %d %%rH\n", humi); 
      Serial.printf("Wind Speed: %.1f m/s", windspeed);

      String result = "  " + String(temp,1) + " °C\n" + "  " + String(humi) + " %rH\n" + String(press) + " hPa\n" + "  " + String(windspeed,1) + " m/s";
      drawCanvas(result);
    }
    else Serial.println("WiFi Disconnected");
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) 
{
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) 
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else 
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}