/*
char* checkTime(unsigned long StartTime){
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;

  int hour = 0;
  int min = 0;
  int sec = ElapsedTime/1000;
  min = sec/60;
  sec = sec%60;
  if (min >= 60) {
    hour = min/60;
    min = min%60;
  }
  char time[] = hour + ":" + min + ":" + sec;

  return time;
}
*/