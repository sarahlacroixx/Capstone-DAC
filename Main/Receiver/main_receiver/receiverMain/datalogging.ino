//NOT MY FUNCTION - NEED TO CHANGE
int count;

void dataLogging(){
  bool ready = GSheet.ready();
  FirebaseJson response;

  Serial.println("\nAppend spreadsheet values...");
  Serial.println("----------------------------");

  unsigned long graphCurrentTime = millis();
  unsigned long graphElapsedTime = graphCurrentTime - StartTime;

  FirebaseJson valueRange;
  count++;

  //unsigned long graphCurrentTime = millis();
  //unsigned long graphElapsedTime = graphCurrentTime - StartTime;
  float test = currentSensor(curSen12v);
  Serial.println(test);
  int testv2 = test*1000.00;
  Serial.println(testv2);

  valueRange.add("majorDimension", "COLUMNS");
  //valueRange.set("values/[0]/[0]", 0);
  
  valueRange.set("values/[8]/[0]", graphElapsedTime);
  valueRange.set("values/[9]/[0]", testv2);

  bool success = GSheet.values.append(&response /* returned response */, spreadsheetId /* spreadsheet Id to append */, "Sheet5!A1" /* range to append */, &valueRange /* data range to append */);
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