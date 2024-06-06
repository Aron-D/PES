# Project Embedded Systems
![plattegrond van het appartement.](https://github.com/Aron-D/PES/blob/main/Plattegrond%20van%20appartement.png)
**Casus Appartement automatisering**,
Het doel van dit project is om een multifunctioneel gebouw bestaande uit 6 verdiepen, waarbij elke verdieping een eigen functie heeft te automatiseren. Met als centraalthema het verbeteren van comfort, veiligheid, energie-efficientie, toegankelijkheid en duurzaamheid.
Dit project is gericht op het **Automatiseren van het appartement**.

## Export (.zip) Latest Project :card_index_dividers:
- 

## Higlights
- **I2C_code+RPI_Server** - ...
- **L432KC Solutions** - De configuratie en initialisatie van STM32CubeIDE.
- **Final code in C** - Implementatie van socket verbinding code in C programmataal, Later in C++ OOP.
- **Socket Wemos** - De Socket code van de LOLIN D1 Mini board A.K.A. Wemos. De Wemos functioneert als de server socket.

## I2C_Code +RPI_Server

## Socket Wemos
Ieder Wemos board bestuurt een sensoren en combineert de sensor code samen met de Socket code om dit verder door te sturen naar de Client Socket van de raspberry pi (WirelessPi).

### Demo Code
- De [Button Testcode + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/ButtonCode.ino), test of button signaal wordt doorverwezen via socket verbinding met de WirelessPi.
- De [Ledlamp Testcode + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/wifi_led.ino), test of de lamp aan en uit kan gaan via de opgenomen raw data van de WirelessPi en met condities in de wemos de lamp besturen.


### Sprint Code
- De [Speciaalbeheerder + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/Speciaalbeheerder/Speciaalbeheerder.ino), een I2C  oled display + U8G2_SSD1306_128X64 library + potentiometer.
- De [Bewegingssensor Testcode](https://github.com/Aron-D/PES/blob/main/socket%20wemos/BewegingsSensorCode.ino), test of er beweging wordt herkent op de HC-S501 sensor.
- De [Bewegingssensor code + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/Bewegingscode.ino)
- De [Ledstrip testCode](https://github.com/Aron-D/PES/blob/main/socket%20wemos/WerkendeLedstripCode.ino)
- De [Ledstrip Code + Socket Connectie](https://github.com/Aron-D/PES/blob/main/socket%20wemos/Ledstripcode.ino)
