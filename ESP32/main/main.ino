
#include <ArduinoJson.h>
#include <connectMqttBroker.h>
#include <TempDevice.h>

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
#define switch3Pin 12
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

  Serial.begin(115200);
  myMQTT.begin();
  dhtDevice.begin();

  // Handle messages from broker
  myMQTT.setOnMessageCallback([](const char* topic, const char* message) {
    Serial.printf("MQTT [%s] -> %s\n", topic, message);

    if (strcmp(topic, "main/setLed1") == 0) {
      int value = atoi(message);
      digitalWrite(led1Pin, value);
      Serial.println("  setLed1  to   " + String(value));
    }
    if (strcmp(topic, "main/setLed2") == 0) {
      int value = atoi(message);
      digitalWrite(led2Pin, value);
      Serial.println("  setLed2  to   " + String(value));
    }
    if (strcmp(topic, "main/setLed3") == 0) {
      int value = atoi(message);
      digitalWrite(led3Pin, value);
      Serial.println("  setLed3  to   " + String(value));
    }
    if (strcmp(topic, "main/setLed4") == 0) {
      int value = atoi(message);
      digitalWrite(led4Pin, value);
      Serial.println("  setLed4  to   " + String(value));
    }
  });


  myMQTT.publishMessage("main/restart", true);
}

static unsigned long VrLast = 0;
static unsigned long lightLast = 0;
static unsigned long tempLast = 0;

void loop() {

  if (millis() - VrLast > 500) {
    VrLast = millis();
    int vrValue = analogRead(vrPIN);  // 0-4095
    int percent = map(vrValue, 0, 4095, 0, 100); // optional: 0-100%
    
    // Serial.print("VR raw: ");
    // Serial.print(vrValue);
    // Serial.print("  |  VR %: ");
    // Serial.println(percent);

    myMQTT.publishMessage("main/vr", percent);
  }

  if (millis() - lightLast > 1000) {
    lightLast = millis();
    int ldrRaw = analogRead(ldrPin);
    int ldrPercent = map(ldrRaw, 0, 4095, 100, 0);
    
    // Serial.print("light raw: ");
    // Serial.print(ldrRaw);
    // Serial.print("  |  light %: ");
    // Serial.println(ldrPercent);

    myMQTT.publishMessage("main/light", ldrPercent);
  }

  if (millis() - tempLast > 1000) {
    tempLast = millis();

    JsonDocument data;
    data["temp"] = dhtDevice.readTemperature();
    data["humid"] = dhtDevice.readHumidity();

    char buffer[256];
    serializeJson(data, buffer);

    myMQTT.publishMessage("main/temp", buffer);
  }


  int sw1 = !digitalRead(switch1Pin);
  int sw2 = !digitalRead(switch2Pin);
  int sw3 = !digitalRead(switch3Pin);
  int sw4 = !digitalRead(switch4Pin);

  digitalWrite(led1Pin, sw1);
  digitalWrite(led2Pin, sw2);
  digitalWrite(led3Pin, sw3);
  digitalWrite(led4Pin, sw4);

  Serial.print(digitalRead(sw1));
  Serial.print(":");
  Serial.print(digitalRead(sw2));
  Serial.print(":");
  Serial.print(digitalRead(sw3));
  Serial.print(":");
  Serial.println(digitalRead(sw4));

  myMQTT.loop();
}





// void setup() {
//   pinMode(led1Pin, OUTPUT);
//   pinMode(led2Pin, OUTPUT);
//   pinMode(led3Pin, OUTPUT);
//   pinMode(led4Pin, OUTPUT);
  
//   pinMode(switch1Pin, INPUT_PULLUP);
//   pinMode(switch2Pin, INPUT_PULLUP);
//   pinMode(switch3Pin, INPUT_PULLUP);
//   pinMode(switch4Pin, INPUT_PULLUP);

//   Serial.begin(115200);
// }

// void loop() {
//   Serial.print(digitalRead(switch1Pin));
//   Serial.print(":");
//   Serial.print(digitalRead(switch2Pin));
//   Serial.print(":");
//   Serial.print(digitalRead(switch3Pin));
//   Serial.print(":");
//   Serial.println(digitalRead(switch4Pin));

//   int sw1 = !digitalRead(switch1Pin);
//   int sw2 = !digitalRead(switch2Pin);
//   int sw3 = !digitalRead(switch3Pin);
//   int sw4 = !digitalRead(switch4Pin);

//   digitalWrite(led1Pin, sw1);
//   digitalWrite(led2Pin, sw2);
//   digitalWrite(led3Pin, sw3);
//   digitalWrite(led4Pin, sw4);

//   int vrValue = analogRead(vrPIN);  // 0-4095
//   int percent = map(vrValue, 0, 4095, 0, 100); // optional: 0-100%
  
//   Serial.print("VR raw: ");
//   Serial.print(vrValue);
//   Serial.print("  |  VR %: ");
//   Serial.println(percent);

//   int ldrRaw = analogRead(ldrPin);
//   int ldrPercent = map(ldrRaw, 0, 4095, 100, 0);
  
//   Serial.print("light raw: ");
//   Serial.print(ldrRaw);
//   Serial.print("  |  light %: ");
//   Serial.println(ldrPercent);

//   delay(500);

// }


