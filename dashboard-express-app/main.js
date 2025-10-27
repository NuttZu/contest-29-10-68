const express = require('express');
const path = require('path');
const http = require("http");
const { Server } = require("socket.io");

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const PORT = process.env.PORT || 3000;

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
  socket.emit("message", "Hello from the server!");
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