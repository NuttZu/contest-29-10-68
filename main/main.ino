#include <connectMqttBroker.h>
#include <TempDevice.h>

#define VR_PIN 15
#define ldrPin 2
#define dhtPin 4
#define segPin 16

#define led1Pin 36
#define led2Pin 39
#define led3Pin 34
#define led4Pin 35

#define switch1Pin 25
#define switch2Pin 26
#define switch3Pin 27
#define switch4Pin 14


                        //  wifi ssid         wifi pass     broker ip   broker port
connectMqttBroker myMQTT("NuttZuRaspiWifi", "NuttZuWifi1", "10.42.0.1", 1883);

void setup() {

  // pinMode(vrPin, INPUT);
  // pinMode(ldrPin, INPUT);
  // pinMode(dhtPin, INPUT);
  // pinMode(segPin, OUTPUT);

  // pinMode(led1Pin, OUTPUT);
  // pinMode(led2Pin, OUTPUT);
  // pinMode(led3Pin, OUTPUT);
  // pinMode(led4Pin, OUTPUT);
  
  // pinMode(switch1Pin, INPUT);
  // pinMode(switch2Pin, INPUT);
  // pinMode(switch3Pin, INPUT);
  // pinMode(switch4Pin, INPUT);

  Serial.begin(115200);
  myMQTT.begin();
}

void loop() {

  static unsigned long VrLast = 0;
  // int vrMap = map();
  if (millis() - VrLast > 500) {
    int vrValue = analogRead(VR_PIN);  // 0-4095
    int percent = map(vrValue, 0, 4095, 0, 100); // optional: 0-100%
    
    Serial.print("VR raw: ");
    Serial.print(vrValue);
    Serial.print("  |  VR %: ");
    Serial.println(percent);
  }

  myMQTT.loop();
}