float currentSensor(int currSen){

    float value = analogRead(currSen);
    float mapValue = map(value, 0, 3300, -125000, 125000);
    mapValue = mapValue/10000.00;
    if (mapValue < 0) {
      mapValue = 0;
    }

    return mapValue;
}