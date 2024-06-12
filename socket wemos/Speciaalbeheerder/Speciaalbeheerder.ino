#include <Wire.h>
#include <U8g2lib.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "blinkywifi";
const char* password = "";
const int serverPort = 8080;

// U8G2 constructor for SSD1306 128x64 OLED via I2C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/D2, /* data=*/D3);

WiFiServer server(serverPort);
char buffer[300];           // Buffer om ontvangen data op te slaan
bool dataReceived = false;  // Vlag om aan te geven dat data is ontvangen

// Strings om de waarden op te slaan
String values[6] = { "", "", "", "", "", "" };  // Initiële waarden zijn leeg

void setup() {
  Serial.begin(115200);
  u8g2.begin();

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
  // Check for client connection and read data
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    int pos = 0;

    while (client.connected()) {
      if (client.available() > 0) {
        char c = client.read();
        if (c == '\r' || c == '\n') {
          if (pos > 0) {
            buffer[pos] = '\0';      // Null-terminate the buffer
            Serial.println(buffer);  // Received message uitlezen

            // Process the buffer and extract key-value pairs
            char* token = strtok(buffer, ",");
            int index = 0;
            while (token != NULL && index < 6) {
              char key[100];
              char val[100];
              sscanf(token, "%49[^:]: %49s", key, val);  // Use correct format specifier
              values[index] = val;
              token = strtok(NULL, ",");
              index++;
            }

            dataReceived = true;  // Set flag to true when data is received
            client.println("Ack");
            break;
          }
        } else {
          buffer[pos++] = c;
          // Check voor bufferoverflow
          if (pos >= sizeof(buffer) - 1) {
            pos = sizeof(buffer) - 1;
          }
        }
      }
    }
    client.stop();
  }

  // Update het display altijd, ongeacht of er nieuwe data ontvangen is
  updateDisplay();
}

void updateDisplay() {
  u8g2.clearBuffer();                  // Clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Choose a suitable font

  // Lees de analoge waarde
  int analogValue = analogRead(A0);

  // Serial.println(analogValue);

  // Bepaal de positie van de hoverbalk op basis van de analoge waarde
  int hoverY = 0;
  int offset = 0;

  if (analogValue >= 900) {
    hoverY = 4 * 16;  // Positie 6
    offset = 1;
  } else if (analogValue >= 800 && analogValue < 900) {
    hoverY = 3 * 16;  // Positie 5
    offset = 1;
  } else if (analogValue >= 700 && analogValue < 800) {
    hoverY = 2 * 16;  // Positie 4
  } else if (analogValue >= 600 && analogValue < 700) {
    hoverY = 1 * 16;  // Positie 3
  } else if (analogValue >= 500 && analogValue < 600) {
    hoverY = 0;  // Positie 2
  }

  // Hoverbalk tekenen
  u8g2.setDrawColor(1);                  // Zet de kleur naar wit
  u8g2.drawBox(0, hoverY, 128, 16);      // Tekent een witte balk
  u8g2.setDrawColor(0);                  // Zet de kleur naar zwart
  u8g2.drawBox(2, hoverY + 2, 124, 12);  // Tekent een kleinere zwarte balk binnenin voor een grijze rand

  // Tekst binnen elke positie met ontvangen waarden
  u8g2.setDrawColor(1);  // Zet de kleur naar wit

  if (offset == 0) {
    u8g2.setCursor(5, 12);  // Zet de cursor positie voor de eerste tekst
    u8g2.print("Temp: ");
    u8g2.print(values[0]);  // Toon de waarde van Temp

    u8g2.setCursor(5, 28);  // Zet de cursor positie voor de tweede tekst
    u8g2.print("Hum: ");
    u8g2.print(values[1]);  // Toon de waarde van Luchtvocht

    u8g2.setCursor(5, 44);  // Zet de cursor positie voor de derde tekst
    u8g2.print("CO2: ");
    u8g2.print(values[2]);  // Toon de waarde van CO2

    u8g2.setCursor(5, 60);  // Zet de cursor positie voor de vierde tekst
    u8g2.print("RFID: ");
    u8g2.print(values[3]);  // Toon de waarde van Laatst poging deur
  } else {
    u8g2.setCursor(5, 12);  // Zet de cursor positie voor de tweede tekst
    u8g2.print("CO2: ");
    u8g2.print(values[2]);  // Toon de waarde van Luchtvocht

    u8g2.setCursor(5, 28);  // Zet de cursor positie voor de derde tekst
    u8g2.print("RFID: ");
    u8g2.print(values[3]);  // Toon de waarde van CO2

    u8g2.setCursor(5, 44);  // Zet de cursor positie voor de vierde tekst
    u8g2.print("Bedtijd: ");
    u8g2.print(values[4]);  // Toon de waarde van Laatst poging deur

    u8g2.setCursor(5, 60);  // Zet de cursor positie voor de vijfde tekst
    u8g2.print("Bed is: ");
    // if(values[5] == "1"){
    // u8g2.print("droog");  // Toon de waarde van Tijd in bed
    // } else {
    u8g2.print(values[5]);
    // }
    // u8g2.setCursor(5, 76);  // Zet de cursor positie voor de zesde tekst
    // u8g2.print("Nat: ");
    // u8g2.print(values[5]);  // Toon de waarde van Nat
  }

  u8g2.sendBuffer();  // Transfer internal memory to the display
}
