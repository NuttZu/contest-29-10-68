#include "WString.h"
#ifndef MY_OLED_H
#define MY_OLED_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

class MyOLED {
private:
    Adafruit_SSD1306 display;

public:
    MyOLED() : display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET) {}

    bool begin() {
        if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
            Serial.println(F("SSD1306 allocation failed"));
            return false;
        }
        display.clearDisplay();
        display.display();
        return true;
    }

    void clear() {
        display.clearDisplay();
        display.display();
    }

    void printText(const String text, int x, int y, int size = 1) {
        display.setTextSize(size);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(x, y);
        display.print(text);
        display.display();
    }

    void printNumber(int num, int x, int y, int size = 1) {
        display.setTextSize(size);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(x, y);
        display.print(num);
        display.display();
    }

    void printFloat(float num, int decimals, int x, int y, int size = 1) {
        display.setTextSize(size);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(x, y);
        display.print(num, decimals);
        display.display();
    }

    void drawProgressBar(int x, int y, int w, int h, int percentage) {
        display.drawRect(x, y, w, h, SSD1306_WHITE);           // outline
        int fill = map(percentage, 0, 100, 0, w);
        display.fillRect(x + 1, y + 1, fill-2, h-2, SSD1306_WHITE); // fill
        display.display();
    }

    void drawUI(float tempC, float tempF, float hum, int lightPercent, int vrPercent) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);

        display.setCursor(0, 0);
        display.printf("Temperature: %.1f C / %.1f F", tempC, tempF);

        display.setCursor(0, 10);
        display.printf("Humidity: %.1f %%", hum);

        display.setCursor(0, 20);
        display.print("Light Level:");
        drawProgressBar(0, 30, 128, 6, lightPercent);

        display.setCursor(0, 40);
        display.print("VR Sensor:");
        drawProgressBar(0, 50, 128, 6, vrPercent);
    }
};

#endif
