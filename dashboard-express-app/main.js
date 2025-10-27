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
const {
    LedStateManager, 
    TempManager, 
    VrStateManager,
    LightStateManager

} = require("./modules/dataManagement");

const PORT = process.env.PORT || 80;

// Basic middleware
app.use(express.json());
const ledManager = new LedStateManager("./STORED/ledState.json");
const tempManager = new TempManager("./STORED/tempData.json");
const vrStateManager = new VrStateManager("./STORED/vrData.json");
const lightStateManager = new LightStateManager("./STORED/lightData.json");

// Serve the "views" folder as public static assets
app.use(express.static(path.join(__dirname, 'views')));

app.get('/', (req, res) => {
    // This will still work, or the static middleware can serve dashboard.html directly
    res.sendFile(path.join(__dirname, './views/dashboard.html'));
});

app.get('/Dashboard', (req, res) => {
    // This will still work, or the static middleware can serve dashboard.html directly
    res.sendFile(path.join(__dirname, './views/dashboard.html'));
});

app.get('/Control', (req, res) => {
    // This will still work, or the static middleware can serve dashboard.html directly
    res.sendFile(path.join(__dirname, './views/control.html'));
});

// Socket.io connection handler
io.on("connection", (socket) => {
    console.log("✅ A user connected");

    socket.emit("led1State", ledManager.getLedState("led1State"));
    socket.emit("led2State", ledManager.getLedState("led2State"));
    socket.emit("led3State", ledManager.getLedState("led3State"));
    socket.emit("led4State", ledManager.getLedState("led4State"));
    
    socket.emit("mqttTemp", tempManager.getField("temp"));
    socket.emit("mqttHumid", tempManager.getField("humid"));

    socket.emit("mqttVr", vrStateManager.getVr());
    socket.emit("mqttLight", lightStateManager.getLight());
    
    
    socket.on('toggleLed', (data) => {
        console.log("📩 Data from client:", data);
        let ledState = ledManager.getLedState(data.led);
        console.log('ledState : ' + ledState.toString());
        if (ledState) {
            ledState = false;
        } else if (!ledState) { ledState = true; }
        mqttClient.publish(`main/${data.led}`, ledState.toString());
        console.log("mqtt : " + (`main/${data.led}` + ledState.toString()));
    });

    // get data from mqtt
    mqttClient.on('data', ({ topic, data }) => {
        if (topic == 'main') {
            console.log(`[${topic}] : ${data}`);
        }
        if (topic == 'main/temp') {
            console.log(`[${topic}] : ${data}`);
            const parseData = JSON.parse(data);
            tempManager.setField('temp', parseData.temp);
            tempManager.setField('humid', parseData.humid);
            socket.emit("mqttTemp", tempManager.getField("temp"));
            socket.emit("mqttHumid", tempManager.getField("humid"));
        }
        if (topic == 'main/light') {
            console.log(`[${topic}] : ${data}`);
            lightStateManager.setLight(data);
            socket.emit("mqttLight", lightStateManager.getLight());
        }
        if (topic == 'main/vr') {
            console.log(`[${topic}] : ${data}`);
            vrStateManager.setVr(data);
            socket.emit("mqttVr", vrStateManager.getVr());
        }

        if (topic == 'main/led1State') {
            console.log(`[${topic}] : ${data}`);
            if (data == "true") {
                data = true;
            }
            else if (data == "false") {
                data = false;
            } else {data = false}
            ledManager.setLedState("led1State", data);
            socket.emit("led1State", ledManager.getLedState("led1State"));
        }
        if (topic == 'main/led2State') {
            console.log(`[${topic}] : ${data}`);
            if (data == "true") {
                data = true;
            }
            else if (data == "false") {
                data = false;
            } else {data = false}
            ledManager.setLedState("led2State", data);
            socket.emit("led2State", ledManager.getLedState("led2State"));
        }
        if (topic == 'main/led3State') {
            console.log(`[${topic}] : ${data}`);
            if (data == "true") {
                data = true;
            }
            else if (data == "false") {
                data = false;
            } else {data = false}
            ledManager.setLedState("led3State", data);
            socket.emit("led3State", ledManager.getLedState("led3State"));
        }
        if (topic == 'main/led4State') {
            console.log(`[${topic}] : ${data}`);
            if (data == "true") {
                data = true;
            }
            else if (data == "false") {
                data = false;
            } else {data = false}
            ledManager.setLedState("led4State", data);
            socket.emit("led4State", ledManager.getLedState("led4State"));
        }
    });
});

// start mqtt client 
mqttClient.on('connected', () => {
    console.log('Subscribing to topics...');
    mqttClient.subscribe('main');
    mqttClient.subscribe('main/temp');
    mqttClient.subscribe('main/light');
    mqttClient.subscribe('main/vr');
    mqttClient.subscribe('main/led1State');
    mqttClient.subscribe('main/led2State');
    mqttClient.subscribe('main/led3State');
    mqttClient.subscribe('main/led4State');
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