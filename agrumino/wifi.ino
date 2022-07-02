// 
void connectToWifi() {
    WiFi.begin(ssid, password);
    byte attempts = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
        if(++attempts >= 20){
          Serial.println("Couldn't connect to WiFi. Going to deep sleep for 900 Sec"); 
          deepSleepSec(900);
        }
    }
    Serial.println("Connected to the WiFi network");
}
