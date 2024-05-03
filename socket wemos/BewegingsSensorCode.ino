int ledPin = 2;   // Ingebouwde LED 
int pirPin = 0;   // Pin voor de HC-S501 sensor
int pirValue = 0;      // Uitgelezen sensor waarde
int pirStatus;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);    // Stel de ledPin in als uitvoer
  pinMode(pirPin, INPUT);     // Stel de pirPin in als invoer

}

// Herhaal oneindig
void loop() {
  pirValue = digitalRead(pirPin); // Lees de waarde van de PIR uit
  Serial.println(pirValue);
  if (pirStatus != pirValue) {
    pirStatus = pirValue;
    if(pirValue == HIGH){
      Serial.println("beweging");
    }
    if(pirValue == LOW){
      Serial.println("Geen beweging");
    }
  }

  digitalWrite(ledPin, pirValue); // Schrijf de gelezen waarde naar het ingebouwde LED  
  //Serial.println(pirValue);
}