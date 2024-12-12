// //
// // Created by Krisna Purnama on 05/12/24.
// //
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>
// #include <sys/select.h>
// #include "../header/server.h"
// #include <sys/errno.h>
//
// // Implementasi fungsi untuk mengirim pesan JSON
// void send_json_message(int client_socket, const char *message) {
//     char response[2048];
//     char sanitized_message[1024];
//     snprintf(sanitized_message, sizeof(sanitized_message), "%s", message);
//
//     // Hapus karakter newline atau carriage return
//     for (int i = 0; i < strlen(sanitized_message); i++) {
//         if (sanitized_message[i] == '\n' || sanitized_message[i] == '\r') {
//             sanitized_message[i] = 0;
//             break;
//         }
//     }
//
//     snprintf(response, sizeof(response), "{\"status\":\"success\",\"message\":\"%s\"}\n", sanitized_message);
//     send(client_socket, response, strlen(response), 0);
// }
//
// // Inisialisasi server
// void initialize_server(Server *server) {
//     server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (server->server_fd == 0) {
//         perror("Socket creation failed");
//         exit(EXIT_FAILURE);
//     }
//
//     int opt = 1;
//     setsockopt(server->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
//
//     server->address.sin_family = AF_INET;
//     server->address.sin_addr.s_addr = INADDR_ANY;
//     server->address.sin_port = htons(PORT);
//
//     if (bind(server->server_fd, (struct sockaddr *)&server->address, sizeof(server->address)) < 0) {
//         perror("Bind failed");
//         close(server->server_fd);
//         exit(EXIT_FAILURE);
//     }
//
//     if (listen(server->server_fd, BACKLOG) < 0) {
//         perror("Listen failed");
//         close(server->server_fd);
//         exit(EXIT_FAILURE);
//     }
// }
//
// // Jalankan server
// void run_server(Server *server) {
//     fd_set readfds;
//     int max_sd, sd, activity;
//     int addrlen = sizeof(server->address);
//     char buffer[1024];
//
//     for (int i = 0; i < MAX_CLIENTS; i++) {
//         server->client_sockets[i] = 0;
//     }
//
//     printf("Listening on port %d\n", PORT);
//
//     while (1) {
//         // Bersihkan set soket
//         FD_ZERO(&readfds);
//         FD_SET(server->server_fd, &readfds);
//         max_sd = server->server_fd;
//
//         for (int i = 0; i < MAX_CLIENTS; i++) {
//             sd = server->client_sockets[i];
//             if (sd > 0) {
//                 FD_SET(sd, &readfds);
//             }
//             if (sd > max_sd) {
//                 max_sd = sd;
//             }
//         }
//
//         // Tunggu aktivitas
//         activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
//         if ((activity < 0) && (errno != EINTR)) {
//             printf("Select error");
//         }
//
//         // Jika ada koneksi baru
//         if (FD_ISSET(server->server_fd, &readfds)) {
//             int new_socket = accept(server->server_fd, (struct sockaddr *)&server->address, (socklen_t*)&addrlen);
//             if (new_socket < 0) {
//                 perror("Accept failed");
//                 exit(EXIT_FAILURE);
//             }
//
//             printf("New connection\n");
//
//             for (int i = 0; i < MAX_CLIENTS; i++) {
//                 if (server->client_sockets[i] == 0) {
//                     server->client_sockets[i] = new_socket;
//                     break;
//                 }
//             }
//         }
//
//         // Periksa aktivitas pada soket klien
//         for (int i = 0; i < MAX_CLIENTS; i++) {
//             sd = server->client_sockets[i];
//             if (FD_ISSET(sd, &readfds)) {
//                 int valread;
//                 if ((valread = read(sd, buffer, 1024)) == 0) {
//                     // Koneksi ditutup
//                     getpeername(sd, (struct sockaddr*)&server->address, (socklen_t*)&addrlen);
//                     printf("Host disconnected\n");
//                     close(sd);
//                     server->client_sockets[i] = 0;
//                 } else {
//                     buffer[valread] = '\0';
//                     // Kirim pesan ke semua klien
//                     for (int j = 0; j < MAX_CLIENTS; j++) {
//                         if (server->client_sockets[j] != 0 && j != i) {
//                             send_json_message(server->client_sockets[j], buffer);
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

#include "../header/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

// Fungsi untuk memformat pesan sebagai JSON
void format_message(const char *input, char *output) {
    char response[2048];
    char sanitized_message[1024];
    snprintf(sanitized_message, sizeof(sanitized_message), "%s", input);

    // Hapus karakter newline atau carriage return
    for (int i = 0; i < strlen(sanitized_message); i++) {
        if (sanitized_message[i] == '\n' || sanitized_message[i] == '\r') {
            sanitized_message[i] = 0;
            break;
        }
    }

    snprintf(output, BUFFER_SIZE, "{\"status\":\"success\",\"message\":\"%s\"}\n", sanitized_message);
}

// Fungsi untuk menangani komunikasi dengan klien
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];       // Buffer untuk menerima data dari klien
    char formatted_message[BUFFER_SIZE]; // Buffer untuk pesan terformat
    ssize_t valread;

    while ((valread = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[valread] = '\0'; // Tambahkan null terminator ke string

        // Tampilkan pesan yang diterima
        printf("Message from client: %s\n", buffer);

        // Format pesan ke dalam format JSON
        format_message(buffer, formatted_message);

        // Kirim pesan kembali ke klien
        send(client_socket, formatted_message, strlen(formatted_message), 0);
        printf("Message from server: %s\n", formatted_message);
    }
}

// Fungsi untuk menginisialisasi server dan menangani koneksi klien
void init_server() {
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    // Tangani sinyal anak proses yang selesai (menghindari zombie process)
    signal(SIGCHLD, SIG_IGN);

    // Buat socket server
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Atur opsi socket agar dapat segera digunakan kembali setelah ditutup
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Konfigurasi alamat dan port server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket ke alamat dan port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Mulai mendengarkan koneksi masuk
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // Terima koneksi dari klien
        int client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Fork untuk menangani klien secara terpisah
        if (fork() == 0) {
            close(server_fd); // Tutup server socket di proses anak
            handle_client(client_socket); // Tangani komunikasi dengan klien
            close(client_socket); // Tutup socket klien setelah selesai
            exit(0);
        }

        close(client_socket); // Tutup socket klien di proses utama
    }

    close(server_fd); // Tutup server socket saat server dihentikan
}
