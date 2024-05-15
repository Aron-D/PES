#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include "stdint.h"
#include <Servo.h>

#define ServoPin 2                // gpio2
#define RST_PIN 0                 // gpio0
#define SS_PIN 15                 // gpio15
const char* ssid = "blinkywifi";  // Hotspot name
const char* password = "";        // Password
const int serverPort = 8080;      // poort om de server te bereiken

Servo servo;                       // Create Servo instance
WiFiServer server(serverPort);     // Create WiFiServer instance
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// Defined allowed and denied UIDs
byte allowedUID[4] = { 0xD9, 0x35, 0xD8, 0x14 };  // Card UID: D9 35 D8 14
byte deniedUID[4] = { 0xB1, 0x55, 0x72, 0x1D };   // Card UID: B1 55 72 1D

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

  SPI.begin();                        // Init SPI bus
  mfrc522.PCD_Init();                 // Init MFRC522
  //mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  //Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
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
            buffer[pos] = '\0';  // Add terminator
            Serial.println(buffer);  // Debugging for received message

            // Converts string to an integer
            int value = atoi(buffer);

            // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
            if (!mfrc522.PICC_IsNewCardPresent()) {
              break;
            }

            // Select one of the cards
            if (!mfrc522.PICC_ReadCardSerial()) {
              break;
            }

            // Check if the UID matches allowedUID
            if (mfrc522.uid.size == 4 && memcmp(allowedUID, mfrc522.uid.uidByte, 4) == 0) {
              client.println("Toegang : 1");
              Serial.println("Toegang : 1");
            }
            else {
              client.println("Toegang : 0");
              Serial.println("Toegang : 0");
            }
            if (value == 1) {
              servo.write(180);
              Serial.println("open open");
            }
            if (value == 0) {
              servo.write(0);
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
  }
}
