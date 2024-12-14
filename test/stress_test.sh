#!/bin/bash

# Jumlah klien yang akan diuji (Misalnya 100 untuk lebih banyak koneksi)
NUM_CLIENTS=100

# File log untuk mencatat hasil
LOG_FILE="load_test_log.txt"
ERROR_LOG_FILE="load_test_error_log.txt"

# Tulis header log
echo "=== Mulai Pengujian Koneksi ===" > $LOG_FILE
echo "=== Mulai Pengujian Koneksi ===" > $ERROR_LOG_FILE

# Perintah untuk setiap klien
function send_request {
    # Catat waktu permintaan dimulai
    start_time=$(date +"%Y-%m-%d %H:%M:%S")

    # File respons akan disimpan di folder kerja
    response_file="telnet_response_$RANDOM.txt"

    # Kirim data melalui telnet
    {
        echo "Hello from client $RANDOM";  # Data yang dikirim ke server
        sleep 1;  # Opsional, menunggu respon jika server butuh waktu
    } | telnet 127.0.0.1 8080 > $response_file 2>&1

    # Simpan hasil dari telnet
    response=$(cat $response_file)

    # Catat waktu permintaan selesai
    end_time=$(date +"%Y-%m-%d %H:%M:%S")

    # Identifikasi apakah berhasil atau gagal
    if echo "$response" | grep -q "Connection refused"; then
        echo "$start_time - Gagal koneksi (Connection refused)" >> $ERROR_LOG_FILE
    elif echo "$response" | grep -q "Escape character is '^]'"; then
        echo "$start_time - Permintaan berhasil - Selesai pada $end_time" >> $LOG_FILE
    else
        echo "$start_time - Response tidak dikenali - Response: $response" >> $ERROR_LOG_FILE
    fi

    # Hapus file respons sementara
    rm -f $response_file
}

# Jalankan klien secara paralel
for i in $(seq 1 $NUM_CLIENTS); do
    send_request &  # Jalankan permintaan secara background
done

# Tunggu semua klien selesai
wait

# Tulis pesan selesai pengujian
echo "Pengujian selesai!" >> $LOG_FILE
