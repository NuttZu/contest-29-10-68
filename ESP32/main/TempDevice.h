#ifndef TEMPDEVICE_H
#define TEMPDEVICE_H

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class TempDevice {
public:
    TempDevice(uint8_t pin, uint8_t type = DHT11);
    void begin();
    float readTemperature();
    float readHumidity();
    uint32_t getDelay() const;

private:
    DHT_Unified _dht;
};

#endif
