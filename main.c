//
// Created by Krisna Purnama on 02/12/24.
//
#include "header/server.h"
#include "body/server.c"

// Fungsi utama untuk memanggil semua modul yang diperlukan
int main() {
    Server server;

    // Inisialisasi server
    initialize_server(&server);

    // Jalankan server
    run_server(&server);

    return 0;
}
