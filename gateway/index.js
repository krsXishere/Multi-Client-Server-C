const fs = require('fs');
const https = require('https');
const WebSocket = require('ws');
const net = require('net');

const tcpServerHost = '127.0.0.1';
const tcpServerPort = 8080;

// Muat sertifikat dan kunci privat
const serverOptions = {
    cert: fs.readFileSync('cert.pem'), // Path ke file sertifikat
    key: fs.readFileSync('key.pem')   // Path ke file private key
};

// Buat HTTPS server
const httpsServer = https.createServer(serverOptions);

// Buat WebSocket server yang menggunakan HTTPS
const wss = new WebSocket.Server({ server: httpsServer });

wss.on('connection', (ws) => {
    console.log('Secure WebSocket client connected');

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

// HTTPS server listen pada port tertentu
httpsServer.listen(3000, () => {
    console.log('Secure WebSocket server running on wss://localhost:3000');
});
