
#include <ArduinoJson.h>
#include <connectMqttBroker.h>
#include <TempDevice.h>
#include "MyOLED.h"

MyOLED oled;

#define vrPIN 36
#define ldrPin 39
#define dhtPin 4
#define segPin 16

#define led1Pin 32
#define led2Pin 33
#define led3Pin 17
#define led4Pin 26

#define switch1Pin 27
#define switch2Pin 14
#define switch3Pin 34
#define switch4Pin 13


                        //  wifi ssid         wifi pass     broker ip   broker port
connectMqttBroker myMQTT("NuttZuRaspiWifi", "NuttZuWifi1", "10.42.0.1", 1883);
TempDevice dhtDevice(dhtPin);

void setup() {

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(led4Pin, OUTPUT);
  
  pinMode(switch1Pin, INPUT_PULLUP);
  pinMode(switch2Pin, INPUT_PULLUP);
  pinMode(switch3Pin, INPUT_PULLUP);
  pinMode(switch4Pin, INPUT_PULLUP);

  oled.begin();
  oled.printText("boot....", 0, 0, 2);


  Serial.begin(115200);
  myMQTT.begin();
  dhtDevice.begin();

  // Handle messages from broker
  myMQTT.setOnMessageCallback([](const char* topic, const char* message) {
    Serial.printf("MQTT > %s\n", topic, message);
    int state = (strcmp(message, "true") == 0) ? HIGH : LOW;
    
    if (strcmp(topic, "main/setLed1") == 0) {
      digitalWrite(led1Pin, state);
      Serial.println("  setLed1  to   " + String(state));
    }
    if (strcmp(topic, "main/setLed2") == 0) {
      digitalWrite(led2Pin, state);
      Serial.println("  setLed2  to   " + String(state));
    }
    if (strcmp(topic, "main/setLed3") == 0) {
      digitalWrite(led3Pin, state);
      Serial.println("  setLed3  to   " + String(state));
    }
    if (strcmp(topic, "main/setLed4") == 0) {
      digitalWrite(led4Pin, state);
      Serial.println("  setLed4  to   " + String(state));
    }
  });


  myMQTT.publishMessage("main/restart", true);
}

static unsigned long VrLast = 0;
static unsigned long lightLast = 0;
static unsigned long tempLast = 0;

bool pressed[4] = {false, false, false, false};

void loop() {

  if (millis() - VrLast > 500) {
    VrLast = millis();
    int vrValue = analogRead(vrPIN);  // 0-4095
    int percent = map(vrValue, 0, 4095, 0, 100); // optional: 0-100%

    oled.printText(("VR Sensor :            "), 0, 20, .5);
    oled.printText(("VR Sensor : " + String(percent)), 0, 20, .5);
    myMQTT.publishMessage("main/vr", percent);
  }

  if (millis() - lightLast > 1000) {
    lightLast = millis();
    int ldrRaw = analogRead(ldrPin);
    int ldrPercent = map(ldrRaw, 0, 4095, 100, 0);

    String LightText = ("LDR Sensor : " + String(ldrPercent));

    oled.printText("LDR Sensor :            ", 0, 30, .5);
    oled.printText(LightText, 0, 30, .5);
    myMQTT.publishMessage("main/light", ldrPercent);
  }

  if (millis() - tempLast > 1000) {
    tempLast = millis();

    int temp = dhtDevice.readTemperature();
    int humid = dhtDevice.readHumidity();

    JsonDocument data;
    data["temp"] = temp;
    data["humid"] = humid;

    char buffer[256];
    serializeJson(data, buffer);

    String TempText = ("Tempereture : " + String(temp) + " °C / " + String((temp * 1.8) + 32));
    String HumidText = ("Humidity : " + String(humid) + " %");

    oled.printText("Tempereture :           ", 0, 0, .5);
    oled.printText("Humidity :              ", 0, 10, .5);

    oled.printText(TempText, 0, 0, .5);
    oled.printText(HumidText, 0, 10, .5);
    myMQTT.publishMessage("main/temp", buffer);
  }

  int pins[4] = {switch1Pin, switch2Pin, switch3Pin, switch4Pin};

  for (int i = 0; i < 4; i++) {
    bool state = digitalRead(pins[i]);

    if (!state && !pressed[i]) {   // button pressed
      pressed[i] = true;
      handlePress(i);
    }

    if (state) pressed[i] = false; // button released
  }

  myMQTT.loop();
}

void handlePress(int index) {
  switch(index) {
    case 0:  // button 1
      myMQTT.publishMessage("main/ledToggle", "led1State");
      break;
    case 1:  // button 2
      myMQTT.publishMessage("main/ledToggle", "led2State");
      break;
    case 2:  // button 3
      myMQTT.publishMessage("main/ledToggle", "led3State");
      break;
    case 3:  // button 4
      myMQTT.publishMessage("main/ledToggle", "led4State");
      break;
  }
}

