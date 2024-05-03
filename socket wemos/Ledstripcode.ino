#include <ESP8266WiFi.h>
#include <WiFiClient.h>
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

int ledWaarde;

const char* ssid = "blinkywifi";    //hotspot naam
const char* password = "";          //wachtwoord van de server
const int serverPort = 8080;        //poort om de server te bereiken
const char* targetValue = "draadlozeLed : 1";
// const char* request;
const int ledPin = 0;  // D3 on the Wemos GPIO0
bool ledState = false;  // Houdt de status van de LED bij (aan/uit)
const unsigned long timerInterval = 30000;    //timer om de led aan te houden
bool timerRunning = false;                    //check om te zien of timer aanstaat
unsigned long previousMillisTimer = 0;        //check om te zien of timer aanstaat

WiFiServer server(serverPort);

void setup() {
  Serial.begin(115200);

  strip.begin();           // Initialize NeoPixel object
  strip.fill(strip.Color(255, 150, 50)); // intialize the color
  strip.show(); //geef de kleur weer

  pinMode(ledPin, OUTPUT);

  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // start de verbinding
  while (WiFi.status() != WL_CONNECTED) {  // debug om aan te geven dat er nog geen verbinding is
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  unsigned long currentMillis = millis();

  WiFiClient client = server.available(); // object om de server te controleren
  if (client) {
    Serial.println("New client connected");
    bool lineStarted = false;
    char buffer[100];
    int pos = 0;
    while (client.connected()) {
      if (client.available() > 0) {
        char c = client.read();
        if (c == '\r' || c == '\n') {
          if (lineStarted) {
            buffer[pos] = '\0'; // Null-terminator toevoegen
            client.println("Ack");
            client.stop();
            Serial.println(buffer); // Debug: print de ontvangen string

            // Converteer de ontvangen string naar een integer
            int value = atoi(buffer);

            // Controleer de waarde en update de LED-status
            if (value == 1 && !timerRunning && !ledState) {
              ledState = true;
              timerRunning = true;
               // Start the timer
              for (int i = 0; i < 75; i++) {
                strip.setBrightness(i);
                strip.fill(strip.Color(255, 150, 50));
                Serial.println(i);
                strip.show();
                delay(50);
              }
              Serial.println("klaar met brightnes up");
            }
            if (value == 1) {
              previousMillisTimer = currentMillis;
            }
            // Controleer de waarde en update de LED-status
            if (value == 0 && !timerRunning && ledState) {
              ledState = false;
              for (int i = 75; i >= 0; i--) {
                strip.setBrightness(i);
                strip.fill(strip.Color(255, 150, 50));
                Serial.println(i);
                strip.show();
                delay(50);
              }
              Serial.println("klaar met brightnes down");
            }
            // Reset de buffer en positie voor de volgende regel
            memset(buffer, 0, sizeof(buffer));
            pos = 0;
            lineStarted = false;
          }
        } else {
          if (!lineStarted) {
            lineStarted = true;
          }
          buffer[pos++] = c;
          // Controleer of de buffer niet overloopt
          if (pos >= sizeof(buffer) - 1) {
            pos = sizeof(buffer) - 2; // Zet positie terug om te voorkomen dat de buffer overloopt
          }
        }
      }
    }
    // Close the connection with the client after the client disconnects
    Serial.println("Client disconnected");
  }

  // Check if the timer is running and if it's time to turn it off
  if (timerRunning && currentMillis - previousMillisTimer >= timerInterval) {
    timerRunning = false; // Turn off the timer
  }
}
