#include <Agrumino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "config.h"

Agrumino agrumino;
WiFiClient espClient;
PubSubClient client(espClient);

// WiFi
const char *password = WIFI_PASSWORD;
const char *ssid = WIFI_SSID;

const char *mqtt_broker = MQTT_BROKER;
const char *mqtt_username = MQTT_USERNAME;
const char *mqtt_password = MQTT_PASSWORD;
const int mqtt_port = MQTT_PORT;

String topicIrrigate = "";
String topicSleep = "";

void setup() {
  Serial.begin(115200);
  agrumino.setup();
  connectToWifi();
  setUpMqtt();
  String message = boardDataJSON();
  Serial.println(message);
  client.publish("sensorsnapshot", message.c_str());
}
void loop() {
  client.loop();
}


String boardDataJSON() {
  StaticJsonDocument<1024> doc;
  agrumino.turnBoardOn();
  doc["temperature"] = agrumino.readTempC();
  doc["illuminance"] = agrumino.readLux();
  doc["soilMoisture"] = agrumino.readSoil();
  doc["batteryLevel"] = agrumino.readBatteryLevel();
  doc["batteryCharging"] = agrumino.isBatteryCharging();
  doc["attachedToUSB"] = agrumino.isAttachedToUSB();
  doc["plant_id"] = 1;
  doc["macAddress"] = WiFi.macAddress();
  Serial.println(WiFi.macAddress());
  String output;
  Serial.print(output);
  serializeJson(doc, output);
  Serial.println(output);
  agrumino.turnLedOn();
  delay(5000);
  agrumino.turnLedOff();
  agrumino.turnBoardOff();

  return output;
}

// duration in seconds
int waterPlant(int duration, double goalMoisture, int time) {
  Serial.print("Watering for time: ");
  Serial.println(time);
  agrumino.turnLedOn();
  agrumino.turnWateringOn();
  while (agrumino.readSoil() < goalMoisture && !agrumino.isButtonPressed()) {
    Serial.println(agrumino.readSoil());
    Serial.println(duration);
    duration++;
    delayHalfSec(1);
  }
  agrumino.turnWateringOff();
  agrumino.turnLedOff();

  return duration;
}
