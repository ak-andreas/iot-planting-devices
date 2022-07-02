void delayHalfSec(int sec) { delay(sec * 500); }
void delaySec(int sec) { delay(sec * 1000); }
void delayHour(int hour) { delay(hour * 1000 * 60); }
void deepSleepSec(int sec) {
    ESP.deepSleep(sec * 1000000);  // microseconds
}
