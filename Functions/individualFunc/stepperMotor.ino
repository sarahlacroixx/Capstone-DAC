
void runMotorsSet(int ls1, int ls2)
  while (ls1 != 1 && ls2 != 1){
    //can change this to runSpeed if want to control speed of motor
    motor1.run();
    motor2.run();
    ls1 = checkLS(ls1);
    ls2 = checkLS(ls2);
  }

}

int checkLS1(int ls){
  if (digtalRead(ls1) == 1){
    return 1;
  }else {
    return 0;
  }
}
