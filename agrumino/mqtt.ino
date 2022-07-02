

void setUpMqtt() {
    // connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp8266-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public mqtt broker\n",
                      client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Mosquitto broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    // publish and subscribe

     topicIrrigate = "device/"+WiFi.macAddress() + "/irrigate" ;
     topicSleep = "device/"+WiFi.macAddress() + "/sleep" ;
   
    client.publish("hello", "hello Mosquitto");
    client.subscribe(topicIrrigate.c_str());
    client.subscribe(topicSleep.c_str());
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String payloadStr = "";
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        payloadStr = payloadStr + (char)payload[i];
    }

    Serial.println(payloadStr);

    Serial.println(strcmp(topic, "irrigate/1"));
    if (strcmp(topic, topicIrrigate.c_str()) == 0) {
        Serial.println(topic);
        Serial.println(payloadStr);
        Serial.println("Irrigation instruction came in.");
        agrumino.turnBoardOn();
        StaticJsonDocument<1024> doc;
        deserializeJson(doc, payload);
        int duration = doc["duration"];
        double goalMoisture = doc["goalMoisture"];
        double startMoisture = agrumino.readSoil();
         duration = waterPlant(duration, goalMoisture, 0);
           goalMoisture = agrumino.readSoil();
        publishIrrigationAction(duration, startMoisture,goalMoisture);
        agrumino.turnBoardOff();
    } else if (strcmp(topic, topicSleep.c_str()) == 0) {
        //Serial.println("Im going to sleep 1800");
        //deepSleepSec(900);
    }
    Serial.println("Im going to sleep 1800");
    deepSleepSec(1800);
    
}

void publishIrrigationAction(int duration, double startMoisture, double goalMoisture){
  Serial.println("Irrigation has been performed:");
  Serial.print("Duration in seconds: ");
  Serial.println(duration);
  Serial.print("Start Moisture: ");
  Serial.println(startMoisture);
  Serial.print("Goal Moisture: ");
  Serial.println(goalMoisture);

  StaticJsonDocument<1024> doc;
  doc["time"] = duration;
  doc["startMoisture"] = startMoisture;
  doc["goalMoisture"] = goalMoisture;
  String output;
  serializeJson(doc, output);
  client.publish("irrigationAction", output.c_str());
  Serial.println(output.c_str());
  delaySec(1);
  
  
}
