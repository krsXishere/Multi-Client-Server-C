//
// Created by Krisna Purnama on 05/12/24.
//
#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h> // Untuk sockaddr_in
#include <sys/socket.h> // Untuk socket APIs

#define PORT 8080          // Port yang digunakan server
#define BACKLOG 10         // Maksimal jumlah klien dalam antrian
#define BUFFER_SIZE 10240   // Ukuran buffer untuk komunikasi

// Deklarasi fungsi
size_t ai_response_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
void init_server();
void handle_client(int client_socket);
void sanitize_json_string(const char *input, char *output, size_t max_size);
void format_message(const char *input, char *output, int use_ai);
int call_ai_api(const char *input, char *response);

#endif
