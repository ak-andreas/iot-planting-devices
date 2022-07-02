String boardDataJSON() {
  StaticJsonDocument<1024> doc;
  doc["soilMoisture"] = readSoilMoisture();
  doc["plant_id"] = 1;
  doc["macAddress"] = WiFi.macAddress();
  String output;
  Serial.print(output);
  serializeJson(doc, output);
  Serial.println(output);
  return output;
}

int readSoilMoisture() {
  int output_value = analogRead(sensor_pin);
  output_value = map(output_value, airValue, waterValue, 0, 100);
  Serial.print("Moisture : ");
  Serial.print(output_value);
  Serial.println("%");
  delay(1000);
  if (output_value < 0) return 0;
  if (output_value > 100) return 100;
  return output_value;
}

int waterPlant(int duration, double goalMoisture, int time) {
  Serial.println("Watering: ");
  digitalWrite(relay, HIGH);
  double sec = 0;
  int soilMoisture = readSoilMoisture();
  int start = soilMoisture;
  do {
    soilMoisture = readSoilMoisture();
    Serial.println(soilMoisture);
    duration++;
    delayHalfSec(1);
    sec += 0.5;
    if(sec == 3.0 && soilMoisture <= soilMoisture +5)  return 0;
  } while (soilMoisture < goalMoisture);
  digitalWrite(relay, LOW);
  Serial.print("Watering time: ");
  Serial.println(sec);
  return duration;
}