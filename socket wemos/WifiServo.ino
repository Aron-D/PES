#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include "stdint.h"
#include <Servo.h>

#define ServoPin 0                // gpio0
const char* ssid = "blinkywifi";  // Hotspot name
const char* password = "";        // Password
const int serverPort = 8080;      // poort om de server te bereiken

Servo servo;                    // Create Servo instance
WiFiServer server(serverPort);  // Create WiFiServer instance


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);  // Start connection

  while (WiFi.status() != WL_CONNECTED) {  //Debugging for wifi
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print(WiFi.localIP());
  Serial.println("/");

  // Start the server
  server.begin();
  Serial.println("Server started");
  servo.attach(ServoPin);
}

void loop() {
  WiFiClient client = server.available();  // Instance of WifiClient to check if the server is available
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
            //buffer[pos] = '\0';  // Add terminator
            Serial.println(buffer);  // Debugging for received message

            // Converts string to an integer
            int value = atoi(buffer);

            if (value == 1) {
              servo.write(180);
              client.println("Deurstatus : 1");
              Serial.println("open open");
            } else {
              servo.write(0);
              client.println("Deurstatus : 0");
              Serial.println("Dicht Dicht");
            }
          }
        } else {
          lineStarted = true;
          buffer[pos++] = c;
          // Check if bufferoverflow
          if (pos >= 99) {
            break;
          }
        }
      }
    }
    Serial.println("Client disconnected");
    delay(100);
  }
}
