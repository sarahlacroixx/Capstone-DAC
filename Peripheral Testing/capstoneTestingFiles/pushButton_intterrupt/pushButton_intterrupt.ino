

void IRAM_ATTR detectStart();

void setup() {
  // put your setup code here, to run once:
  pinMode(0, OUTPUT);
  pinMode(12, INPUT_PULLDOWN);

  Serial.begin(115200);

 // attachInterrupt(digitalPinToInterrupt(12), detectStart, RISING);


}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = digitalRead(12);
  
    digitalWrite(0, reading);
    Serial.println(reading);
  
/*
  if (digitalRead(12) == HIGH) {
    Serial.println("Ready push1");
    delay(200);
  
  }
  */

}


