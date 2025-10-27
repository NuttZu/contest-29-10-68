// mqttClient.js
const mqtt = require('mqtt');
const EventEmitter = require('events');

class MqttClient extends EventEmitter {
  constructor(brokerUrl, options = {}) {
    super();
    this.client = mqtt.connect(brokerUrl, options);

    this.client.on('connect', () => {
      console.log('✅ MQTT connected');
      this.emit('connected');
    });

    this.client.on('error', (err) => {
      console.error('❌ MQTT error:', err.message);
      this.emit('error', err);
    });

    this.client.on('message', (topic, message) => {
      try {
        const data = message.toString();
        // Emit an event to main file with topic and parsed message
        this.emit('data', { topic, data });
      } catch (err) {
        console.error('❌ Parse error:', err);
      }
    });
  }

  subscribe(topic) {
    this.client.subscribe(topic, (err) => {
      if (err) console.error('❌ Subscribe error:', err);
      else console.log(`📡 Subscribed to: ${topic}`);
    });
  }

  publish(topic, message) {
    this.client.publish(topic, message, (err) => {
      if (err) console.error('❌ Publish error:', err);
    });
  }
}
 
module.exports = MqttClient;
