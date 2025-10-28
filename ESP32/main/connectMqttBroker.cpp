#include "connectMqttBroker.h"

static connectMqttBroker* currentInstance = nullptr;

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
    currentInstance = this;
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

void connectMqttBroker::subscribeTopic(const char* topic) {
    if (_client.connected()) {
        _client.subscribe(topic);
    }
}

void connectMqttBroker::setOnMessageCallback(std::function<void(const char* topic, const char* message)> cb) {
    onMessageCallback = cb;
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
            Serial.println();
            Serial.println("MQTT connected");

            // Subscribe default topics if needed
            _client.subscribe("main");
            _client.subscribe("main/setLed1");
            _client.subscribe("main/setLed2");
            _client.subscribe("main/setLed3");
            _client.subscribe("main/setLed4");

        } else {
            Serial.print("failed, rc=");
            Serial.print(_client.state());
            Serial.println(" try again in 0.5 second");
            delay(500);
        }
    }
}

void connectMqttBroker::mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("MQTT msg topic: ");
  Serial.print(topic);
  Serial.print(" | message: ");
  Serial.println(message);

  // 🔹 NEW: forward message to main program callback
  if (currentInstance && currentInstance->onMessageCallback) {
    currentInstance->onMessageCallback(topic, message.c_str());
  }
}
