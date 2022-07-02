#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"


// GLOBAL VARIABLES 

// Pins
int sensor_pin = A0;
int relay = D1;
int d1_status = 0;

// Wifi
const char *password = WIFI_PASSWORD;
const char *ssid = WIFI_SSID;
WiFiClient espClient;

// MQTT
const char *mqtt_broker = MQTT_BROKER;
const int mqtt_port = MQTT_PORT;
const char *mqtt_username = MQTT_USERNAME;
const char *mqtt_password = MQTT_PASSWORD;
String topicIrrigate = "";
String topicSleep = "";
PubSubClient client(espClient);

const int airValue = 689;
const int waterValue = 290;

void setup() {
  Serial.begin(9600);
  connectToWifi();
  setUpMqtt();
  String message = boardDataJSON();
  Serial.println(message);
  client.publish("sensorsnapshot", message.c_str());
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
}

void loop() {
  client.loop();
}


