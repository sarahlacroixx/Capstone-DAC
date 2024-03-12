
void runMotorsSet(){
  bool ls = false;
  while (ls == false){
    //can change this to runSpeed if want to control speed of motor
    motor1.run();
    motor2.run();
    ls = checkLS();
  }
}
