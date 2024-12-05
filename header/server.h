//
// Created by Krisna Purnama on 05/12/24.
//

#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#define PORT 8080
#define BACKLOG 10
#define MAX_CLIENTS 30


// Struktur untuk server
typedef struct {
    int server_fd;
    int client_sockets[MAX_CLIENTS];
    struct sockaddr_in address;
} Server;

// Mendeklarasikan fungsi
void initialize_server(Server *server);
void run_server(Server *server);
void send_json_message(int client_socket, const char *message);

#endif