#include <ArduinoJson.h>
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
#define switch3Pin 34
#define switch4Pin 13

TempDevice dhtDevice(dhtPin);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool pressed[4] = {false, false, false, false};

unsigned long lastUpdate = 0;

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
  oled.printText("Booting...", 0, 0, 2);

  Serial.begin(115200);
  dhtDevice.begin();
}

void loop() {
  unsigned long now = millis();

  // ---- update display every 1 second ----
  if (now - lastUpdate >= 100) {
    lastUpdate = now;

    int vrValue = analogRead(vrPIN);
    int vrPercent = map(vrValue, 0, 4095, 0, 100);

    int ldrValue = analogRead(ldrPin);
    int ldrPercent = map(ldrValue, 0, 4095, 100, 0);

    int temp = dhtDevice.readTemperature();
    int humid = dhtDevice.readHumidity();

    oled.clear();  // clear once per update

    oled.printText("Temperature : " + String(temp) + " °C / " + String((temp * 1.8) + 32) + " °F", 0, 0, 1);
    oled.printText("Humidity    : " + String(humid) + " %", 0, 10, 1);
    oled.printText("Light Level : " + String(ldrPercent) + " %", 0, 20, 1);
    oled.printText("VR Sensor   : " + String(vrPercent) + " %", 0, 30, 1);

    oled.display();
  }

  // ---- button handling ----
  int pins[4] = {switch1Pin, switch2Pin, switch3Pin, switch4Pin};
  for (int i = 0; i < 4; i++) {
    bool state = digitalRead(pins[i]);
    if (!state && !pressed[i]) {
      pressed[i] = true;
      handlePress(i);
    }
    if (state) pressed[i] = false;
  }
}

void handlePress(int index) {
  switch (index) {
    case 0: Serial.println("Button 1 pressed"); break;
    case 1: Serial.println("Button 2 pressed"); break;
    case 2: Serial.println("Button 3 pressed"); break;
    case 3: Serial.println("Button 4 pressed"); break;
  }
}

void printText(const String text, int x, int y, int size = 1) {
    oled.setTextSize(size);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(x, y);
    oled.print(text);
}