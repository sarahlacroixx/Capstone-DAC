// function was modified from the nerd tutorials on the implementation of google sheets

void dataLogging(){
  bool ready = GSheet.ready();
  FirebaseJson response;

  FirebaseJson valueRange;

  unsigned long graphCurrentTime = millis();
  unsigned long graphElapsedTime = graphCurrentTime - StartTime;

  int temp1 = curr.centCurrentTemp;
  int temp2 = curr.sideCurrentTemp;

  int inCo2 = curr.inCo2;
  int outCo2 = curr.outCo2;
  int flowCo2 = curr.flowCo2; 
  int currentSen1 = currentSensor(curSen);


  valueRange.add("majorDimension", "COLUMNS");
  valueRange.set("values/[0]/[0]", graphElapsedTime);
  valueRange.set("values/[2]/[0]", temp1);
  valueRange.set("values/[3]/[0]", temp2);
  valueRange.set("values/[6]/[0]", inCo2);
  valueRange.set("values/[7]/[0]", outCo2);
  valueRange.set("values/[9]/[0]", flowCo2);
  valueRange.set("values/[10]/[0]", 0); //for air flow
  valueRange.set("values/[12]/[0]", currentSen1);

  delay(800);

  bool success = GSheet.values.append(&response /* returned response */, spreadsheetId /* spreadsheet Id to append */, "Sheet1!A1" /* range to append */, &valueRange /* data range to append */);
        if (success){
            response.toString(Serial, true);
            valueRange.clear();
        }
        else{
            Serial.println(GSheet.errorReason());
        }
        Serial.println();
        Serial.println(ESP.getFreeHeap());

}
