void logSDCardCO2(float time, float inCo2Reading, float outCo2Reading) {
  data = String(time) + "," + String(co2Sensor) + "," + 
                String(co2Sensor) + "\r\n";
  //Serial.print("Save data: ");
  //Serial.println(data);
  appendFile(SD, "/co2Data.txt", data.c_str());
}

void logSDCardTemp(float time, float temp) {
  data2 = String(time) + "," + String(temp) + "\r\n";
  //Serial.print("Save data: ");
  //Serial.println(data);
  appendFile(SD, "/tempData.txt", data2.c_str());
}

void logSDCardOutCO2(float time, float outCo2) {
  data3 = String(time) + "," + String(outCo2) + "\r\n";
  //Serial.print("Save data: ");
  //Serial.println(data);
  appendFile(SD, "/outputtedData.txt", data3.c_str());
}


//Random nerd tutorials functions - need to reference
void WriteFile(const char * path, const char * message){
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(path, FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    myFile.println(message);
  } 
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening file ");
    Serial.println(path);
  }
  myFile.close(); // close the file:
}

//random nerd tutorial function (not mine)
// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
