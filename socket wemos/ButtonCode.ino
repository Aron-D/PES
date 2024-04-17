#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "blinkywifi";
const char* password = "";
const int serverPort = 8080;

const int ButtonPin = 0;  // D3 on the Wemos is the builtin LED - GPIO16
int ButtonState = 0;  // Keeps track of the button status (pressed/released)

WiFiServer server(serverPort);

void setup() {
  Serial.begin(115200);

  pinMode(ButtonPin, INPUT);

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
        Serial.print(request);

        // Check if the button is pressed or not
        if(digitalRead(ButtonPin) == HIGH) {
          ButtonState = 0; // Button is pressed
        } else {
          ButtonState = 1; // Button is not pressed
        }
        
          if (ButtonState == 0) {
            client.println("draadlozeKnop : 0");
          } else {
            client.println("draadlozeKnop : 1");
          }

      }
    }
    // Close the connection with the client after the client disconnects
    client.stop();
    Serial.println("Client disconnected");
  }
}