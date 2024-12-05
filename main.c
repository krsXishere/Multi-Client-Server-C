//
// Created by Krisna Purnama on 05/12/24.
//
#include "header/server.h"

// Fungsi utama untuk memanggil semua modul yang diperlukan
int main() {
    Server server;

    // Inisialisasi server
    initialize_server(&server);

    // Jalankan server
    run_server(&server);

    return 0;
}