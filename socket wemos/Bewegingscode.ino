#include <ESP8266WiFi.h>
#include <WiFiClient.h>

int pirPin = 0;   // Pin voor de HC-S501 sensor
int pirValue;      // Uitgelezen sensor waarde
int pirStatus;    // de status van de bewegingssensor

const char* ssid = "blinkywifi";  //naam van de hotspot
const char* password = "";        //wachtwoord
const int serverPort = 8080;      //poort om de server te bereiken

WiFiServer server(serverPort);

void setup() {
  Serial.begin(115200);
  pinMode(pirPin, INPUT);     // Stel de pirPin in als invoer
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);   // start de verbinding
  
  while (WiFi.status() != WL_CONNECTED) { // debug om aan te geven dat er nog geen verbinding is
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
  WiFiClient client = server.available();   // object om de server te controleren
  if (client) {
    Serial.println("New client connected");
    while (client.connected()) {
      if (client.available() > 0) {
        char request = client.read(); // Leest data van de client
        Serial.println(request);
        pirValue = digitalRead(pirPin); // Lees de waarde van de PIR uit

          if(pirValue == HIGH){ // voorwaarde om actief te sturen
            client.println("draadlozeBewegingsSensor : 1");
            Serial.println("draadlozeBewegingsSensor : 1");
          } else {
            client.println("draadlozeBewegingsSensor : 0");
            Serial.println("draadlozeBewegingsSensor : 0");
          }
          delay(50);
      }
    }
    Serial.println("Client disconnected"); 
  }
}
