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


//Random nerd tutorials functions
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}


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
