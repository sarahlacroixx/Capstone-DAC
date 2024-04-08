
//red is 16

void setup() {
  // put your setup code here, to run once:
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(9, OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(16, LOW);
  digitalWrite(17, HIGH);
  digitalWrite(9, HIGH);
}
