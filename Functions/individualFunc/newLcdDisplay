
//function for a rotating lcd display

unsigned long StartTime = millis();
int prevValue = 0;


void lcdDisplayWithPot(const char display, int chambTemp, int elecTemp, int inCo2, int outCo2, int flowCo2){
  //change pot to the pin number associated with the potentiometer
  int analogValue = analogRead(1);

  int value = map(analogValue, 0, 3127, 0, 3);

  if (prevValue != value){
    lcd.clear();
  }
  Serial.println(value);
  switch(value){
    case 0:
      displayChar(display, 0);
      displayChambTemp(chambTemp, 1);
      displayInCo2(inCo2, 2);
      displayOutCo2(outCo2, 3);
      prevValue = value;
      break;
    case 1:

      displayChambTemp(chambTemp, 0);
      displayInCo2(inCo2, 1);
      displayOutCo2(outCo2, 2);
      displayClock(StartTime, 3);
      prevValue = value;
      break;
    case 2:
   
      displayInCo2(inCo2, 0);
      displayOutCo2(outCo2, 1);
      displayClock(StartTime, 2);
      displayElecTemp(elecTemp, 3);
      prevValue = value;
      break;
    case 3:
      displayOutCo2(outCo2, 0);
      displayClock(StartTime, 1);
      displayElecTemp(elecTemp, 2);
      displayFlowCo2(flowCo2, 3);
      prevValue = value;
      break;
  }
  
}


void displayChambTemp(int chambTemp, int spot){
      lcd.setCursor(0, spot);
      lcd.print("Temperature: ");
      lcd.setCursor(14, spot);
      lcd.print(chambTemp);
}

void displayChar(char display, int spot){
      lcd.setCursor(0, spot);
      lcd.print(display);
}

void displayInCo2(int inCo2, int spot){
      lcd.setCursor(0, spot);
      lcd.print("Inlet CO2: ");  
      lcd.setCursor(12, spot);
      lcd.print(inCo2);
}

void displayOutCo2(int outCo2, int spot){
      lcd.setCursor(0, spot);
      lcd.print("Outlet CO2: ");  
      lcd.setCursor(13, spot);
      lcd.print(outCo2);
}

void displayElecTemp(int elecTemp, int spot){
      lcd.setCursor(0, spot);
      lcd.print("Elec. Temp: ");
      lcd.setCursor(13, spot);
      lcd.print(elecTemp);
}

void displayFlowCo2(int flowCo2, int spot){
      lcd.setCursor(0, spot);
      lcd.print("Exiting CO2: ");  
      lcd.setCursor(14, spot);
      lcd.print(flowCo2);
}
