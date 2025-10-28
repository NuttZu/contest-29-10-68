#include "connectMqttBroker.h"

connectMqttBroker::connectMqttBroker(
    char* ssid,
    char* password,
    const char* mqtt_server,
    int mqtt_port
) : _ssid(ssid),
    _password(password),
    _mqtt_server(mqtt_server),
    _mqtt_port(mqtt_port),
    _client(_espClient)
{}

// -------------------------
// Public Methods
// -------------------------
void connectMqttBroker::begin() {
    connectWiFi();
    connectToMQTT();
    _client.setServer(_mqtt_server, _mqtt_port);
    _client.setCallback(connectMqttBroker::mqttCallback);
}

void connectMqttBroker::loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected, reconnecting...");
        connectWiFi();
    }
    if (!_client.connected()) {
        Serial.println("MQTT disconnected, reconnecting...");
        connectToMQTT();
    }
    _client.loop();
}

void connectMqttBroker::publishMessage(const char* topic, const char* payload) {
    if (_client.connected()) {
        _client.publish(topic, payload);
    }
}

void connectMqttBroker::subscribeTopic(const char* topic) {
    if (_client.connected()) {
        _client.subscribe(topic);
    }
}

// -------------------------
// Private Methods
// -------------------------
void connectMqttBroker::connectWiFi() {
    WiFi.begin(_ssid.c_str(), _password.c_str());
    Serial.print("Connecting to WiFi");
    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
        count++;
        if (count > 120){
            WiFi.disconnect();
            break;
        }
    }
    Serial.println();
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("WiFi connection failed.");
    }
}

void connectMqttBroker::connectToMQTT() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected, reconnecting...");
        connectWiFi();
    }

    int count = 0;
    while (!_client.connected()) {
        count++;
        if (count > 3) {
            break;
        }
        Serial.print("Attempting " + String(count) + " MQTT connection...");
        if (_client.connect("esp32")) {
            Serial.println("connected");

            // Subscribe default topics if needed
            _client.subscribe("main");

        } else {
            Serial.print("failed, rc=");
            Serial.print(_client.state());
            Serial.println(" try again in 0.5 second");
            delay(500);
        }
    }
}

void connectMqttBroker::mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
