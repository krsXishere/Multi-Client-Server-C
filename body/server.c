//
// Created by Krisna Purnama on 05/12/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "../header/server.h"
#include <sys/errno.h>

// Implementasi fungsi untuk mengirim pesan JSON
void send_json_message(int client_socket, const char *message) {
    char response[2048];
    char sanitized_message[1024];
    snprintf(sanitized_message, sizeof(sanitized_message), "%s", message);

    // Hapus karakter newline atau carriage return
    for (int i = 0; i < strlen(sanitized_message); i++) {
        if (sanitized_message[i] == '\n' || sanitized_message[i] == '\r') {
            sanitized_message[i] = 0;
            break;
        }
    }

    snprintf(response, sizeof(response), "{\"status\":\"success\",\"message\":\"%s\"}", sanitized_message);
    send(client_socket, response, strlen(response), 0);
}

// Inisialisasi server
void initialize_server(Server *server) {
    server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_fd == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(PORT);

    if (bind(server->server_fd, (struct sockaddr *)&server->address, sizeof(server->address)) < 0) {
        perror("Bind failed");
        close(server->server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server->server_fd, BACKLOG) < 0) {
        perror("Listen failed");
        close(server->server_fd);
        exit(EXIT_FAILURE);
    }
}

// Jalankan server
void run_server(Server *server) {
    fd_set readfds;
    int max_sd, sd, activity;
    int addrlen = sizeof(server->address);
    char buffer[1024];

    for (int i = 0; i < MAX_CLIENTS; i++) {
        server->client_sockets[i] = 0;
    }

    printf("Listening on port %d\n", PORT);

    while (1) {
        // Bersihkan set soket
        FD_ZERO(&readfds);
        FD_SET(server->server_fd, &readfds);
        max_sd = server->server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = server->client_sockets[i];
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        // Tunggu aktivitas
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            printf("Select error");
        }

        // Jika ada koneksi baru
        if (FD_ISSET(server->server_fd, &readfds)) {
            int new_socket = accept(server->server_fd, (struct sockaddr *)&server->address, (socklen_t*)&addrlen);
            if (new_socket < 0) {
                perror("Accept failed");
                exit(EXIT_FAILURE);
            }

            printf("New connection\n");

            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (server->client_sockets[i] == 0) {
                    server->client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        // Periksa aktivitas pada soket klien
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = server->client_sockets[i];
            if (FD_ISSET(sd, &readfds)) {
                int valread;
                if ((valread = read(sd, buffer, 1024)) == 0) {
                    // Koneksi ditutup
                    getpeername(sd, (struct sockaddr*)&server->address, (socklen_t*)&addrlen);
                    printf("Host disconnected\n");
                    close(sd);
                    server->client_sockets[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    // Kirim pesan ke semua klien
                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        if (server->client_sockets[j] != 0 && j != i) {
                            send_json_message(server->client_sockets[j], buffer);
                        }
                    }
                }
            }
        }
    }
}