#ifndef CONNECT_MQTT_BROKER_H
#define CONNECT_MQTT_BROKER_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

class connectMqttBroker {
  public:
    // Constructor
    connectMqttBroker(
      String ssid,
      String password, 
      const char* mqtt_server,
      int mqtt_port,
      const char* mqtt_Client_ID = nullptr,
      const char* mqtt_Client_username = nullptr,
      const char* mqtt_Client_password = nullptr
    );

    void begin();                 // Setup WiFi and MQTT
    void loop();                  // Must call in main loop
    void publishMessage(const char* topic, const char* payload);
    void subscribeTopic(const char* topic);

  private:
    String _ssid;
    String _password;
    const char* _mqtt_server;
    int _mqtt_port;
    const char* _mqtt_Client_ID;
    const char* _mqtt_Client_username;
    const char* _mqtt_Client_password;

    WiFiClient _espClient;
    PubSubClient _client;

    void connectToMQTT();
    void connectWiFi();
    static void mqttCallback(char* topic, byte* payload, unsigned int length);
};

#endif
