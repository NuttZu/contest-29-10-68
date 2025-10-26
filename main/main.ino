#include <connectMqttBroker.h>

                        //  wifi ssid         wifi pass     broker ip   broker port
connectMqttBroker myMQTT("NuttZuRaspiWifi", "NuttZuWifi1", "10.42.0.1", 1883);

void setup() {
  Serial.begin(115200);
  myMQTT.begin();
}

void loop() {
  myMQTT.loop();

  static unsigned long last = 0;
  if (millis() - last > 5000) {
    last = millis();
    myMQTT.publishMessage("main", "Hello from ESP64!");
  }
}