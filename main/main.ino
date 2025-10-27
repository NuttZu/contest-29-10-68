/*
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
*/







#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define OLED_RESET -1       // Not used with I2C
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define LDR_PIN 2
#define VR_PIN 15

#define SwPin1 25
#define SwPin2 26
#define SwPin3 27
#define SwPin4 14

void setup() {
  Serial.begin(115200);

  pinMode(SwPin1, INPUT_PULLUP);
  pinMode(SwPin2, INPUT_PULLUP);
  pinMode(SwPin3, INPUT_PULLUP);
  pinMode(SwPin4, INPUT_PULLUP);

  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Initializing...");
  display.display();

  // Initialize DHT
  dht.begin();
  delay(2000);
}

void loop() {
  //  Read Switch
  int sw1 = digitalRead(SwPin1);
  int sw2 = digitalRead(SwPin2);
  int sw3 = digitalRead(SwPin3);
  int sw4 = digitalRead(SwPin4);

  Serial.println(String(sw1) + String(sw2) + String(sw3) + String(sw4));

  // --- Read sensors ---
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int ldr = analogRead(LDR_PIN);
  int vr = analogRead(VR_PIN);

  // --- Normalize readings (0–4095 → 0–10 blocks) ---
  int ldrLevel = map(ldr, 0, 4095, 0, 10);
  int vrLevel = map(vr, 0, 4095, 0, 10);

  // --- Draw on screen ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Temperature and humidity
  display.setCursor(0, 0);
  display.print("Temp : ");
  if (isnan(temp)) display.print("--");
  else display.print(temp, 1);
  display.print("C");

  display.setCursor(0, 10);
  display.print("Humid: ");
  if (isnan(hum)) display.print("--");
  else display.print(hum, 0);
  display.println("%");

  // Light bar
  display.setCursor(0, 28);
  display.print("Light: [");
  for (int i = 0; i < 10; i++) {
    if (i < ldrLevel) display.print((char)219); // █
    else display.print(' ');
  }
  display.println("]");

  // VR bar
  display.setCursor(0, 40);
  display.print("VR   : [");
  for (int i = 0; i < 10; i++) {
    if (i < vrLevel) display.print((char)219);
    else display.print(' ');
  }
  display.println("]");

  display.display();

  delay(200);
}
