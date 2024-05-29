#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include "stdint.h"

#define RST_PIN 0                 // gpio0
#define SS_PIN 15                 // gpio15
const char* ssid = "blinkywifi";  // Hotspot name
const char* password = "";        // Password
const int serverPort = 8080;      // poort om de server te bereiken

WiFiServer server(serverPort);     // Create WiFiServer instance
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

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

  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522
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
            buffer[pos] = '\0';      // Add terminator
            //Serial.println(buffer);  // Debugging for received message

            // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
            if (!mfrc522.PICC_IsNewCardPresent()) {
              client.println("Ack");
              break;
            }

            // Select one of the cards
            if (!mfrc522.PICC_ReadCardSerial()) {
              client.println("Ack");
              break;
            }
            char buffer2[1000];
            char buffer3[1000];
            for (int i = 0; i < 4; i++) {
              sprintf(buffer2 + (i * 2), "%02X", mfrc522.uid.uidByte[i]);
              sprintf(buffer3, "Toegang : %s", buffer2);
            }
            client.println(buffer3);
            Serial.println(buffer3);
          }
        } else {
          lineStarted = true;
          buffer[pos++] = c;
          if (pos >= 99) {
            break;
          }
        }
      }
    }
    Serial.println("Client disconnected");
  }
}
