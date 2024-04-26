#include <ESP8266WiFi.h>
#include <WiFiClient.h>

int pirPin = 0;   // Pin voor de HC-S501 sensor
int pirValue;      // Uitgelezen sensor waarde
int pirStatus;

const char* ssid = "blinkywifi";
const char* password = "";
const int serverPort = 8080;

WiFiServer server(serverPort);

void setup() {
  Serial.begin(115200);
  pinMode(pirPin, INPUT);     // Stel de pirPin in als invoer

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
  Serial.print(WiFi.localIP());
  Serial.println("/");

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    while (client.connected()) {
      if (client.available() > 0) {
        // Read data from the client
        char request = client.read();
        Serial.println(request);

        // Check if the button is pressed or not
        pirValue = digitalRead(pirPin); // Lees de waarde van de PIR uit

        
          if(pirValue == HIGH){
            client.println("draadlozeBewegingsSensor : 1");
            Serial.println("draadlozeBewegingsSensor : 1");
          } else {
          //if(pirValue == LOW){
            client.println("draadlozeBewegingsSensor : 0");
            Serial.println("draadlozeBewegingsSensor : 0");
          }
          delay(50);
      }
    }
    // Close the connection with the client after the client disconnects
    //client.stop();
    Serial.println("Client disconnected");
  }
}
