#include "TempDevice.h"
#include <Arduino.h>

TempDevice::TempDevice(uint8_t pin, uint8_t type) : _dht(pin, type) {
}

void TempDevice::begin() {
    Serial.begin(9600);
    _dht.begin();
    Serial.println(F("DHTxx Unified Sensor Example"));

    sensor_t sensor;

    // Temperature sensor info
    _dht.temperature().getSensor(&sensor);
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print(F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print(F("Driver Ver:  ")); Serial.println(sensor.version);
    Serial.print(F("Unique ID:   ")); Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
    Serial.print(F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
    Serial.print(F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));

    // Humidity sensor info
    _dht.humidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print(F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print(F("Driver Ver:  ")); Serial.println(sensor.version);
    Serial.print(F("Unique ID:   ")); Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
    Serial.print(F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
    Serial.print(F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
    Serial.println(F("------------------------------------"));

    _dht.temperature().getSensor(&sensor);
}

float TempDevice::readTemperature() {
    sensors_event_t event;
    _dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        Serial.println(F("Error reading temperature!"));
        return NAN;
    }
    return event.temperature;
}

float TempDevice::readHumidity() {
    sensors_event_t event;
    _dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        Serial.println(F("Error reading humidity!"));
        return NAN;
    }
    return event.relative_humidity;
}