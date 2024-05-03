#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Adafruit_NeoPixel.h>

// Pin to use to send signals to WS2812B
#define LED_PIN 0

// Number of WS2812B LEDs attached to the Arduino
#define LED_COUNT 10

// Setting up the NeoPixel library
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


const int ledPin = 0;   // D3 on the Wemos - GPIO0

void setup() {
  Serial.begin(9600);

  strip.begin();                          // Initialize NeoPixel object
  strip.fill(strip.Color(255, 150, 50));  // intialize the color
  strip.show();                           //geef de kleur weer

  pinMode(ledPin, OUTPUT);
}

void loop() {


  for (int i = 0; i < 75; i++) {
    strip.setBrightness(i);
    strip.fill(strip.Color(255, 150, 50));
    Serial.println(i);
    strip.show();
    delay(50);
  }

  for (int i = 75; i >= 0; i--) {
    strip.setBrightness(i);
    strip.fill(strip.Color(255, 150, 50));
    Serial.println(i);
    strip.show();
    delay(50);
  }
}
