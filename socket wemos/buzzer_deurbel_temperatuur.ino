#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "stdint.h"

const char* ssid = "blinkywifi";
const char* password = "";
const int serverPort = 8080;

const int buzzerPin = 4;  // D2 op de Wemos GPIO4 voor output op buzzer
const int knopPin = 5;    // D1 op de Wemos GPIO5 voor input van buzzer

// Defineren van de frequencies voor de notes
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

// Defineer de Mario theme song
const int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0, 0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};

// Defineren van de duurtijd voor elke noot (in milliseconden)
const int noteDurations[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12
};

int knopState = 0;     // Houdt de status van de drukknop bij (aan/uit)

WiFiServer server(serverPort);

void setup() {
  pinMode(buzzerPin, OUTPUT);      // Set de buzzer pin als een output
  pinMode(knopPin, INPUT_PULLUP);  // Set de drukknop pin als een input met een pull_up weerstand
  
  Serial.begin(115200);
  // Connect naar WiFi
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
    knopState = digitalRead(knopPin);

    if (knopState == 0) {
      // speel de Mario theme song
      for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
        int noteDuration = 1000 / noteDurations[i];
        tone(buzzerPin, melody[i], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(buzzerPin);
      }
    }
  
  WiFiClient client = server.available();  // Instantie van WifiClient om te checken of de server is beschikbaar is
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
           buffer[pos] = '\0';  // indien van toepassing een null terminator toevoegen. hier niet nodig
            Serial.println(buffer);  // received message uitlezen

            // Convert string naar integer
            int value = atoi(buffer);

            if (value == 1) {
             
              tone(buzzerPin, 1000);  // buzzer aan
              // delay(100);
              tone(buzzerPin, 1500);
              // delay(100);
              client.println("Buzzer Aan");
            } else {
              noTone(buzzerPin);
              client.println("Buzzer uit!");
            }
          }
        } else {
          lineStarted = true;
          buffer[pos++] = c;
          // Check voor bufferoverflow
          if (pos >= 99) {
            break;
          }
        }
      }
    }
    Serial.println("Client disconnected");
  }
}
