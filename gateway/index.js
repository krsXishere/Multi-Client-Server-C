const net = require('net');
const WebSocket = require('ws');

const tcpServerHost = '127.0.0.1';
const tcpServerPort = 8080;

const wss = new WebSocket.Server({ port: 3000 });

wss.on('connection', (ws) => {
    console.log('WebSocket client connected');

    // Hubungkan ke server TCP
    const tcpClient = new net.Socket();
    tcpClient.connect(tcpServerPort, tcpServerHost, () => {
        console.log('Connected to TCP server');
    });

    // WebSocket menerima data dari browser
    ws.on('message', (message) => {
        console.log('Message from WebSocket client:', message);
        tcpClient.write(message); // Kirim ke server TCP
    });

    // TCP menerima data dari server C
    tcpClient.on('data', (data) => {
        console.log('Message from TCP server:', data.toString());
        ws.send(data.toString()); // Kirim ke WebSocket client
    });

    // Tangani koneksi TCP atau WebSocket yang terputus
    ws.on('close', () => tcpClient.destroy());
    tcpClient.on('close', () => console.log('TCP connection closed'));
});

console.log('WebSocket server running on ws://localhost:3000');
