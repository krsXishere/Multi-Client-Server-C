//
// Created by Krisna Purnama on 05/12/24.
//
#include "../header/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <curl/curl.h>

// Fungsi untuk memanggil NVIDIA AI API
size_t ai_response_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    strncat((char *)userdata, (char *)ptr, size * nmemb);
    return size * nmemb;
}

//Memanggil api ai nvidia
int call_ai_api(const char *input, char *response) {
    CURL *curl;
    CURLcode res;

    const char *api_url = "https://integrate.api.nvidia.com/v1/chat/completions";
    const char *api_key = "";
    char json_payload[BUFFER_SIZE];

    snprintf(json_payload, sizeof(json_payload),
             "{\"model\":\"nvidia/llama-3.1-nemotron-70b-instruct\","
             "\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}],"
             "\"temperature\":0.5,\"top_p\":1,\"max_tokens\":1024,\"stream\":false}", input);

    memset(response, 0, BUFFER_SIZE); // Bersihkan response

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char auth_header[BUFFER_SIZE];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
        headers = curl_slist_append(headers, auth_header);

        curl_easy_setopt(curl, CURLOPT_URL, api_url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ai_response_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "API request failed: %s\n", curl_easy_strerror(res));
            return -1;
        }
        return 0;
    }

    return -1; // Jika curl gagal diinisialisasi
}

// Melakukan formatting json
void sanitize_json_string(const char *input, char *output, size_t max_size) {
    size_t j = 0; // Indeks untuk output
    for (size_t i = 0; input[i] != '\0' && j < max_size - 1; i++) {
        switch (input[i]) {
            case '\"': // Escape kutip ganda
                if (j + 2 >= max_size - 1) break;
            output[j++] = '\\';
            output[j++] = '\"';
            break;
            case '\\': // Escape backslash
                if (j + 2 >= max_size - 1) break;
            output[j++] = '\\';
            output[j++] = '\\';
            break;
            case '\n': // Escape newline
                if (j + 2 >= max_size - 1) break;
            output[j++] = '\\';
            output[j++] = 'n';
            break;
            case '\r': // Escape carriage return
                if (j + 2 >= max_size - 1) break;
            output[j++] = '\\';
            output[j++] = 'r';
            break;
            case '\t': // Escape tab
                if (j + 2 >= max_size - 1) break;
            output[j++] = '\\';
            output[j++] = 't';
            break;
            default:
                output[j++] = input[i];
        }
    }
    output[j] = '\0'; // Tambahkan null terminator
}

// Fungsi untuk memformat pesan sebagai JSON
void format_message(const char *input, char *output, int use_ai) {
    char ai_response[BUFFER_SIZE];
    char sanitized_input[BUFFER_SIZE];

    // Sanitize input untuk memastikan string valid dalam JSON
    sanitize_json_string(input, sanitized_input, sizeof(sanitized_input));

    if (use_ai) {
        // Jika AI digunakan, panggil API AI
        if (call_ai_api(sanitized_input, ai_response) == 0) {
            // Jika berhasil, gunakan JSON respons AI langsung tanpa wrap string
            snprintf(output, BUFFER_SIZE, "{\"status\":\"success\",\"message\":%s}\n", ai_response);
            return;
        } else {
            // Tangani jika ada kegagalan pada API AI
            snprintf(output, BUFFER_SIZE, "{\"status\":\"error\",\"message\":\"AI processing failed\"}\n");
            return;
        }
    }

    // Jika tidak menggunakan AI, output hanya berisi pesan input
    snprintf(output, BUFFER_SIZE, "{\"status\":\"success\",\"message\":\"%s\"}\n", sanitized_input);
}

// Fungsi untuk menangani komunikasi dengan klien
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    char formatted_message[BUFFER_SIZE];
    ssize_t valread;

    while ((valread = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[valread] = '\0';

        printf("Message from client: %s\n", buffer);

        // Pilih mode pemrosesan: 0 untuk pantulkan pesan, 1 untuk gunakan AI
        int use_ai = strstr(buffer, "use_ai") != NULL ? 1 : 0;

        // Format pesan sesuai mode
        format_message(buffer, formatted_message, use_ai);

        // Kirim pesan kembali ke klien
        send(client_socket, formatted_message, strlen(formatted_message), 0);
        printf("Message from server: %s\n", formatted_message);
    }
}

// Fungsi untuk menginisialisasi server
void init_server() {
    struct sockaddr_in address; // Struktur untuk menyimpan informasi alamat server
    socklen_t addrlen = sizeof(address); // Panjang alamat, digunakan untuk accept()

    // Tangani sinyal anak proses yang selesai (menghindari "zombie process")
    signal(SIGCHLD, SIG_IGN);

    // Membuat socket untuk komunikasi
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed"); // Jika pembuatan socket gagal, cetak pesan error
        exit(EXIT_FAILURE);      // Keluar dengan kode error
    }

    // Konfigurasi socket agar dapat digunakan kembali segera setelah ditutup
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Mengatur properti alamat server
    address.sin_family = AF_INET;           // Menggunakan IPv4
    address.sin_addr.s_addr = INADDR_ANY;   // Dengarkan pada semua alamat yang tersedia
    address.sin_port = htons(PORT);         // Port yang digunakan (dalam format jaringan)

    // Bind socket ke alamat dan port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed"); // Jika bind gagal, cetak pesan error
        close(server_fd);      // Tutup socket sebelum keluar
        exit(EXIT_FAILURE);    // Keluar dengan kode error
    }

    // Memulai mode "listen" untuk menerima koneksi masuk
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Listen failed"); // Jika gagal mendengarkan, cetak pesan error
        close(server_fd);        // Tutup socket sebelum keluar
        exit(EXIT_FAILURE);      // Keluar dengan kode error
    }

    // Server berhasil masuk ke mode mendengarkan
    printf("Server is listening on port %d...\n", PORT);

    // Loop utama untuk menerima koneksi masuk
    while (1) {
        // Menerima koneksi dari klien
        int client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_socket < 0) {
            perror("Accept failed"); // Jika accept gagal, cetak pesan error
            continue;                // Lanjutkan ke iterasi berikutnya
        }

        // Membuat proses baru untuk menangani klien menggunakan fork
        if (fork() == 0) { // Proses anak
            close(server_fd);              // Tutup socket server di proses anak
            handle_client(client_socket);  // Panggil fungsi untuk menangani klien
            close(client_socket);          // Tutup socket klien setelah selesai
            exit(0);                       // Proses anak selesai
        }

        // Menutup socket klien di proses utama
        close(client_socket);
    }

    // Menutup server socket saat server dihentikan (tidak akan pernah dicapai dalam loop ini)
    close(server_fd);
}
