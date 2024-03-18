/*
This code is for the "slave" esp32. This code controls the
- heaters
- fans
- stepper motors
- valve
- pump
*/

//load libraries
#include <Wire.h>
#include <AccelStepper.h> //library for stepper motors
#include <esp_now.h>
#include <WiFi.h>



#DEFINE motorInterfacetype 1



//include functions from espNOW
typedef struct parameters {
    int pinAssign;
    bool heat;
    bool fans;
    bool pump;
    bool valve;
    int motors;

} parameters;

parameters highVolt;

//FUNCTION FROM LIBRARY
//everything should happen in this function
void OnDataRecv(const esp_now_recv_info_t * mac, const uint8_t *recData, int len) {
  memcpy(&highVolt, recData, sizeof(highVolt));
  checkPins();
  checkFans();
  checkHeat();
  checkPump();
  checkValve();
  checkMotors();
}

void setup(){
  Serial.begin(115200);
  
  // Set wifi up
  WiFi.mode(WIFI_STA);

   // init espnow
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error with init");
    return;
  }
  // callback function
  esp_now_register_recv_cb(OnDataRecv);

  //relay for heaters
  pinMode(relay1, OUTPUT);

  //fans
  pinMode(fan1, OUTPUT);
  pintMode(fan2, OUTPUT);

  //relay for valve
  pinMode(valveRelay, OUTPUT);

  //set up pump
  pinMode(pump, OUTPUT);

  
}

void loop(){
  //chill and wait for data to be received
}

void checkPins(){
    switch (highVolt.motors){
        case 1:
            cham1PinAssign();
            break;
        case 2:
            cham2PinAssign();
            break;
        case 3:
            break;
    }
}

void checkFans(){
  if (highVolt.fans == true){
    startFans();
  }
  else{
    stopFans();
  }
}

void checkHeat(){
  if (highVolt.heat == true){
    heatON();
  }
  else{
    heatOFF();
  }
}

void checkPump(){
  if (highVolt.pump == true){
    startPump();
  }
  else{
    stopPump();
  }
}

void checkValve(){
  if (highVolt.valve == true){
    openValve();
  }
  else{
    closeValve();
  }
}

void checkMotors(){
    //0 is do nothing
    //1 is close stepper motors
    //2 is open stepper motors
    switch (highVolt.motors){
        case 1:
            stepperMotorsClose(motor1, motor2);
            break;
        case 2:
            stepperMotorsClose(motor1, motor2);
            break;
        case 3:
            break;
    }
}

//function to start fans
void startFans(){
  digitalWrite(fanPin, HIGH);
}

void stopFans(){
  digitalWrite(fanPin, LOW);
}

void openValve(int valvePin){
  digitalWrite(valvePin, HIGH);
}

void closeValve(int valvePin){
  digitalWrite(valvePin, LOW);
}

void startPump(int pumpPin){
  digitalWrite(pumpPin, HIGH);
}

void stopPump(int pumpPin){
  digitalWrite(pumpPin, LOW);
}

void heatON(int heatPin){
    digitalWrite(heatPin, HIGH);
}

void hearOFF(int heatPin){
    digitalWrite(heatPin, LOW);
}

void stepperMotorsClose(AccelStepper motor1, AccelStepper motor2){
    //change pin number depending on limit switch pin
    while (digitalRead(ls1) == LOW && digitalRead(ls2) == LOW) {
        motor1.run();
        motor2.run();
    }
    
}

void stepperMotorsOpen(AccelStepper motor1, AccelStepper motor2){
    //add code here to open stepper motor
    motor1.moveTo(80);
    motor2.moveTo(80;
    motor1.setSpeed(-25);
    motor2.setSpeed(-25);

    if (!motor1.run() && !motor2.run()) {
        motor1.moveTo(-motor1.currentPosition());
        motor1.moveTo(-motor1.currentPosition());
    }
    motor1.run();
    motor2.run();
    
}
//Pin assignments for chamber 1
void cham1PinAssign(){
  //SLAVE PINS for chamber 1
  const int curSen12v = 6;
  const int fanPin = 7;
  const int pumpPin = 0;
  const int DIR = 1;
  const int STEP1 = 10;
  const int STEP2 = 11;
  const int ls1 = 21;
  const int ls2 = 20;
  const int valvePin = 22;
  const int heatPin = 23;
    AccelStepper motor1(motorInterfaceType, STEP1, DIR1);
    AccelStepper motor2(motorInterfaceType, STEP2, DIR2);
  
}
//pin assignments for chamber 2
void cham2PinAssign(){
  //SLAVE PINS for chamber 2
  const int curSen12v = 6;
  const int fanPin = 13;
  const int pumpPin = 0;
  const int DIR = 1;
  const int STEP1 = 2;
  const int STEP2 = 3;
  const int ls1 = 19;
  const int ls2 = 18;
  const int valvePin = 12;
  const int heatPin = TXD; // NEED TO FIX THIS
    AccelStepper motor1(motorInterfaceType, STEP1, DIR1);
    AccelStepper motor2(motorInterfaceType, STEP2, DIR2);
  
}
