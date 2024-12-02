#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <string.h> // Untuk fungsi string seperti strlen

#define PORT 8080      // Port yang akan digunakan server
#define BACKLOG 10     // Maksimal jumlah klien yang dapat masuk dalam antrian

// Fungsi untuk menangani komunikasi dengan klien
void handle_client(int client_socket) {
    char buffer[1024];  // Buffer untuk menyimpan data dari klien
    ssize_t valread;    // Jumlah byte yang dibaca dari klien

    // Loop untuk terus membaca data dari klien hingga koneksi terputus
    while ((valread = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[valread] = '\0'; // Tambahkan null terminator ke akhir string

        // Cetak pesan yang diterima dari klien ke server
        printf("Message from client: %s\n", buffer);
        fflush(stdout); // Pastikan output dicetak segera

        // Kirim kembali data yang diterima ke klien (echo)
        send(client_socket, buffer, valread, 0);
        printf("Message from server: %s\n", buffer);
    }
}

int main() {
    struct sockaddr_in address;  // Struktur untuk menyimpan informasi alamat server
    socklen_t addrlen = sizeof(address);

    // Tangani sinyal anak proses yang selesai (menghindari zombie process)
    signal(SIGCHLD, SIG_IGN);

    // Buat socket server
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed"); // Cetak error jika socket gagal dibuat
        exit(EXIT_FAILURE);
    }

    // Atur opsi socket agar dapat segera digunakan kembali setelah ditutup
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Konfigurasi alamat dan port untuk server
    address.sin_family = AF_INET;                // Gunakan IPv4
    address.sin_addr.s_addr = INADDR_ANY;        // Dengarkan pada semua alamat
    address.sin_port = htons(PORT);             // Atur port dalam format network byte order

    // Bind socket ke alamat dan port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed"); // Cetak error jika bind gagal
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Mulai mendengarkan koneksi masuk
    if (listen(server_fd, BACKLOG) < 0) {
        perror("Listen failed"); // Cetak error jika listen gagal
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // Terima koneksi dari klien
        int client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_socket < 0) {
            perror("Accept failed"); // Cetak error jika accept gagal
            continue; // Lanjutkan loop untuk menerima koneksi berikutnya
        }

        // Fork untuk menangani setiap klien secara terpisah
        if (fork() == 0) {
            close(server_fd); // Tutup server socket di proses anak
            handle_client(client_socket); // Panggil fungsi untuk menangani klien
            close(client_socket); // Tutup socket klien setelah selesai
            exit(0); // Keluar dari proses anak
        }

        close(client_socket); // Tutup socket klien di proses utama
    }

    close(server_fd); // Tutup server socket saat server dihentikan
    return 0;
}
