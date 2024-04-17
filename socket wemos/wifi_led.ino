#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ledWaarde;


const char* ssid = "blinkywifi";
const char* password = "";
const int serverPort = 8080;
 const char* targetValue = "draadlozeLed : 1";
//  const char* request;
const int ledPin = 16;  // D4 on the Wemos is the builtin LED - GPIO16
bool ledState = false;  // Houdt de status van de LED bij (aan/uit)

WiFiServer server(serverPort);

void setup() {
  Serial.begin(115200);



  pinMode(ledPin, OUTPUT);

  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
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
  WiFiClient client = server.available();
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
            client.stop();
            Serial.println(buffer); // Debug: print de ontvangen string

            // Converteer de ontvangen string naar een integer
            int value = atoi(buffer);

            // Controleer de waarde en update de LED-status
            if (value == 1) {
              ledState = true;
            } else {
              ledState = false;
            }

            // Update LED based on new state
            digitalWrite(ledPin, ledState ? HIGH : LOW);

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
    client.stop();
    Serial.println("Client disconnected");
  }
}