#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Define hardware type and pins for LED matrix
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 15

// WiFi credentials
const char* ssid = "blinkywifi";  // Hotspot name
const char* password = "";        // Password
const int serverPort = 8080;      // Port to reach the server
char buffer[100];
// Create instances
WiFiServer server(serverPort);                                        // Create WiFiServer instance
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);  // Create MD_Parola instance

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);  // Start WiFi connection

  while (WiFi.status() != WL_CONNECTED) {  // Debugging for WiFi connection
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

  // Initialize the LED matrix display
  myDisplay.begin();
  myDisplay.setIntensity(15);  // Set brightness (0-15)
  myDisplay.displayClear();
  myDisplay.displayScroll(buffer, PA_CENTER, PA_SCROLL_LEFT, 50);
}

void loop() {
   if (myDisplay.displayAnimate()) {
      myDisplay.displayReset();
    }
  WiFiClient client = server.available();  // Instance of WifiClient to check if the server is available
  if (client) {
    Serial.println("New client connected");
    bool lineStarted = false;
    int pos = 0;
   
    while (client.connected()) {
      if (client.available() > 0) {
        char c = client.read();
        if (c == '\r' || c == '\n') {
          if (lineStarted) {
            buffer[pos] = '\0';      // Add terminator
            Serial.println(buffer);  // Debugging for received message
            client.println("Ack");

            // Animate the display
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