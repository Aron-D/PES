  const int buzzerPin = 4;  // D2 op de Wemos GPIO4 voor output op buzzer
  const int knopPin = 5;  // D1 op de Wemos GPIO5 voor input van buzzer
  int knopState = 0;  // Houdt de status van de drukknop bij (aan/uit)


void setup() {
  pinMode(buzzerPin, OUTPUT); // Set de buzzer pin als een output
  pinMode(knopPin, INPUT_PULLUP); // Set de drukknop pin als een input met een pull_up weerstand
  Serial.begin(115200);
}

void loop() {
  knopState = digitalRead(knopPin);
    Serial.println(knopState);
  if(knopState == 0){
    tone(buzzerPin, 1000); // zet buzzer aan
  }
  else{
    noTone(buzzerPin); // buzzer gaat uit
  }
}