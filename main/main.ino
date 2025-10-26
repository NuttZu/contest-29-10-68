#include <connectMqttBroker.h>

connectMqttBroker myMQTT( // ssid, password, broker address, broker port
  "NuttZuRaspiWifi", 
  "NuttZuWifi1", 
  "nuttpi", 
  1883
);

void setup() {
  Serial.begin(115200);
  myMQTT.begin();
}

void loop() {
  myMQTT.loop();
}
