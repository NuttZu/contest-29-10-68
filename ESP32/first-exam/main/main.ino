#include <ArduinoJson.h>
#include <TempDevice.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define vrPIN 36
#define ldrPin 39
#define dhtPin 4

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
unsigned long lastBlink = 0;
bool blinkState = false;

void oledprintText(const String text, int x, int y, int size = 1);

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
  dhtDevice.begin();

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.print("Booting...");
  oled.display();
  delay(1000);
  oled.clearDisplay();
}

void loop() {
  unsigned long now = millis();

  // --- Read sensors ---
  int vrValue = analogRead(vrPIN);
  int vrPercent = map(vrValue, 0, 4095, 99, 2);

  int ldrValue = analogRead(ldrPin);
  int ldrPercent = map(ldrValue, 0, 4095, 100, 0);

  int temp = dhtDevice.readTemperature();
  int humid = dhtDevice.readHumidity();

  // --- Control LED1 based on LDR ---
  if (ldrPercent < 30) {
    digitalWrite(led1Pin, 1);
  } else if (ldrPercent > 70) {
    digitalWrite(led1Pin, 0);
  }

  // --- Button states ---
  bool sw1 = !digitalRead(switch1Pin);
  bool sw2 = !digitalRead(switch2Pin);

  // --- LED2 & LED3 logic ---
  if (sw1 && sw2) {
    // Both pressed → blink
    if (now - lastBlink > 150) {
      lastBlink = now;
      blinkState = !blinkState;
      digitalWrite(led2Pin, blinkState ? HIGH : LOW);
      digitalWrite(led3Pin, blinkState ? HIGH : LOW);
    }
  } else {
    // Separate buttons
    digitalWrite(led2Pin, sw1 ? HIGH : LOW);
    digitalWrite(led3Pin, sw2 ? HIGH : LOW);
  }

  // --- Update OLED every 100ms ---
  if (now - lastUpdate >= 100) {
    lastUpdate = now;

    oled.clearDisplay();
    oledprintText("Temperature: " + String(temp, 2) + " C", 0, 0, 2);
    oledprintText("Humidity   : " + String(humid) + " %", 0, 20, 2);
    oledprintText("Light Lvl : " + String(ldrPercent) + " %", 0, 40, 2);
    oledprintText("VR Sensor : " + String(vrPercent) + " %", 0, 60, 2);
    oled.display();
  }
}

void oledprintText(const String text, int x, int y, int size) {
  oled.setTextSize(size);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(x, y);
  oled.print(text);
}
