require('dotenv').config();

const MqttClient = require('./modules/mqttApp');
const express = require('express');
const path = require('path');
const http = require("http");
const { Server } = require("socket.io");

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const mqttClient = new MqttClient('mqtt://nuttpi:1883');

const PORT = process.env.PORT || 80;

// Basic middleware
app.use(express.json());

// Serve the "views" folder as public static assets
app.use(express.static(path.join(__dirname, 'views')));

app.get('/', (req, res) => {
    // This will still work, or the static middleware can serve dashboard.html directly
    res.sendFile(path.join(__dirname, './views/dashboard.html'));
});

// Socket.io connection handler
io.on("connection", (socket) => {
    console.log("✅ A user connected");

    // get data from mqtt
    mqttClient.on('data', ({ topic, data }) => {
        if (topic == 'main') {
            console.log(`[${topic}] : ${data}`);
        }
        if (topic == 'main/temp') {
            console.log(`[${topic}] : ${data}`);
            socket.emit("mqttTemp", data);
        }
        if (topic == 'main/humid') {
            console.log(`[${topic}] : ${data}`);
            socket.emit("mqttHumid", data);
        }
        if (topic == 'main/light') {
            console.log(`[${topic}] : ${data}`);
            socket.emit("mqttLight", data);
        }
        if (topic == 'main/vr') {
            console.log(`[${topic}] : ${data}`);
            socket.emit("mqttVr", data);
        }
    });

});

// start mqtt client 
mqttClient.on('connected', () => {
  mqttClient.subscribe('main');
  mqttClient.subscribe('main/temp');
  mqttClient.subscribe('main/humid');
  mqttClient.subscribe('main/light');
});

// 404 handler
app.use((req, res) => {
    res.status(404).json({ error: 'Not Found' });
});

// Error handler
app.use((err, req, res, next) => {
    console.error(err);
    res.status(500).json({ error: 'Internal Server Error' });
});

// Start server when run directly
server.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});