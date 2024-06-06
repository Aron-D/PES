# Project Embedded Systems
![plattegrond van het appartement.](https://github.com/Aron-D/PES/blob/main/Plattegrond%20van%20appartement.png)
**Casus Appartement automatisering**,
Het doel van dit project is om een multifunctioneel gebouw bestaande uit 6 verdiepen, waarbij elke verdieping een eigen functie heeft te automatiseren. Met als centraalthema het verbeteren van comfort, veiligheid, energie-efficientie, toegankelijkheid en duurzaamheid.
Dit project is gericht op het **Automatiseren van het appartement**.

## Export (.zip) Latest Project :card_index_dividers:
- [PES Domotica](https://github.com/Aron-D/PES/archive/refs/heads/main.zip)

## Highlights
- **I2C_code+RPI_Server** - ...
- **L432KC Solutions** - De configuratie en initialisatie van STM32CubeIDE.
- **Final code in C** - Implementatie van socket verbinding code in C programmataal, Later in C++ OOP.
- **Socket Wemos** - De Socket code van de LOLIN D1 Mini board A.K.A. Wemos. De Wemos functioneert als de server socket.
> [!IMPORTANT]
> De **Socket Wemos** library kan niet OOP geimplementeerd worden en zal dus in de C programma-taal blijven.

## I2C_Code +RPI_Server
De I2C Communicatie vindt plaats tussen de connectie van het Bedrade Raspberry Pi (Server Socket) en het Draadloze Raspberry Pi (Client Socket). Hier wordt het BedradePi gezien als de Server, waar ook het lokale hotspot wordt opgesteld voor de socketverbinding.

- De [BedradePi (Server) code](https://github.com/Aron-D/PES/blob/main/socket%20raspberry%20pi/picode_29_5_2024/serverV2.c)
- De [WirelessPi (Client) code](https://github.com/Aron-D/PES/blob/main/socket%20raspberry%20pi/picode_29_5_2024/client.c)

## Socket Wemos
Ieder Wemos board bestuurt een sensor en combineert de sensor code samen met de Socket code om dit verder door te sturen naar de Client Socket van de raspberry pi (WirelessPi).

### Demo Code
- De [Button Testcode + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/ButtonCode.ino), test of button signaal wordt doorverwezen via socket verbinding met de WirelessPi.
- De [Ledlamp Testcode + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/wifi_led.ino), test of de lamp aan en uit kan gaan via de opgenomen raw data van de WirelessPi en met condities in de wemos de lamp besturen.


### Sprint Code
- De [Speciaalbeheerder + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/Speciaalbeheerder/Speciaalbeheerder.ino), een I2C  oled display + U8G2_SSD1306_128X64 library + potentiometer.
- De [Bewegingssensor Testcode](https://github.com/Aron-D/PES/blob/main/socket%20wemos/BewegingsSensorCode.ino), test of er beweging wordt herkent op de HC-S501 sensor.
- De [Bewegingssensor code + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/Bewegingscode.ino), Leest data van bewegingssensor op en stuurt dit door via socketverbinding.
- De [Ledstrip testCode](https://github.com/Aron-D/PES/blob/main/socket%20wemos/WerkendeLedstripCode.ino), test of ledstrip (model: WS2812B) op de juiste wijze functioneert met de Adafruit_NeoPixel library.
- De [Ledstrip Code + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/Ledstripcode.ino), functioneert op basis van bewegingssensor data en zet ledstrip aan wanneer er beweging gedetecteert is.
- De [Lichtkrant + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/LichtkrantWiFi.ino), test de LED matrix of sensorwaardes gedisplayed kunnen worden.
- De [RFID-Scanner testcode](https://github.com/Aron-D/PES/blob/main/socket%20wemos/RFIDscanner.ino), Test of RFID-Scanner een kaart of blauwe chip kan inlezen via de MFRC522 + SPI library.
- De [RFID-Scanner code + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/WifiRFIDscanner.ino), Scan van kaart of blauwe chip kan worden doorgestuurd via socketverbinding met de WirelessPi.
- De [Deur Servo code + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/WifiServo.ino), Deur wordt gerepresenteerd als een servomotor. Als de RFID-Scan heeft plaatsgevonden wordt er via de socketverbinding een signaal verstuurd met een respons, dat uiteindelijk het servomotor aan laat gaan.
