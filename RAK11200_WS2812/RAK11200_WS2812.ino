#include <Adafruit_NeoPixel.h>

#define PIN            WB_IO6
#define NUMPIXELS      1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 2000;

void setup() 
{
  Serial.begin(115200);
  delay(2000);
  pixels.begin();
  pixels.clear();
  pixels.setBrightness(255);
  pixels.show(); // Initialize all pixels to 'off
}

void loop() 
{
  Serial.println("Red");
  pixels.setPixelColor(0, pixels.Color(128, 0,0)); // Moderately bright red color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(delayval); // Delay for a period of time (in milliseconds).
  Serial.println("Green");
  pixels.setPixelColor(0, pixels.Color(0, 128, 0)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(delayval); // Delay for a period of time (in milliseconds).
  Serial.println("Blue");
  pixels.setPixelColor(0, pixels.Color(0,0,128)); // Moderately bright blue color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(delayval); // Delay for a period of time (in milliseconds).
}
