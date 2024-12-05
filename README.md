# MULTI CLIENT SERVER
## Oleh:
- Adrian Pratama Sasmita - 231524033
- Krisna Purnama - 231524048

## Pendahuluan
Pada era digital saat ini, kebutuhan akan komunikasi antar perangkat melalui jaringan komputer semakin meningkat. Salah satu metode yang sering digunakan untuk membangun aplikasi jaringan adalah dengan menggunakan konsep client-server, di mana aplikasi server bertindak sebagai penyedia layanan dan aplikasi client berfungsi untuk mengakses layanan yang diberikan oleh server. Dalam konteks ini, sockets digunakan untuk memfasilitasi komunikasi antar perangkat di jaringan.

Dalam proyek ini, akan dibangun sebuah multi-client server menggunakan bahasa pemrograman C, dengan penerapan konsep-konsep dasar dalam pengembangan aplikasi jaringan, seperti socket programming, forking, dan multi-threading. Server akan mampu menangani beberapa client secara bersamaan, memberikan simulasi nyata dari aplikasi jaringan yang dapat melayani banyak pengguna dalam waktu yang bersamaan.

## Tujuan
Tujuan dari proyek ini adalah untuk mengembangkan sebuah server yang dapat menangani banyak client secara simultan menggunakan konsep fork dan socket programming. Dalam hal ini, setiap client akan berkomunikasi dengan server melalui koneksi socket yang berbeda, sementara server akan mengelola beberapa koneksi client dalam waktu yang bersamaan. Dengan menggunakan fork, setiap kali server menerima koneksi dari client baru, server akan membuat proses baru untuk menangani permintaan tersebut tanpa mempengaruhi client lainnya. Konsep ini memungkinkan aplikasi untuk menskalakan layanannya, menangani banyak client tanpa mengurangi kinerja.

## Konsep dan Teknologi
Socket Programming: Socket adalah endpoint komunikasi yang digunakan oleh client dan server untuk saling bertukar data dalam jaringan. Dengan menggunakan socket, kita dapat mengirim dan menerima data antar perangkat secara efisien.

Forking: Dalam server yang multi-client, menggunakan proses fork memungkinkan server untuk menangani banyak client secara bersamaan. Setiap kali server menerima koneksi baru, proses server akan menyalin dirinya sendiri untuk menangani komunikasi dengan client tersebut tanpa memblokir server untuk menerima koneksi lainnya.

Multi-client Handling: Server akan dibuat untuk menangani banyak client secara paralel. Dengan pendekatan multi-fork ini, setiap client yang terhubung akan dilayani oleh proses yang terpisah, memungkinkan server untuk menangani banyak permintaan tanpa terhambat oleh koneksi lain.

Bahasa Pemrograman C: Bahasa C dipilih karena memiliki kemampuan yang sangat baik dalam pengelolaan memori dan pengelolaan proses, serta kemampuannya untuk berinteraksi langsung dengan sistem operasi pada level yang rendah, yang sangat dibutuhkan untuk implementasi socket programming dan forking.

## Tech Stack:
- Clion: sebagai IDE
- Executable C - C11: sebagai compiler
- Cmake: sistem build otomatis yang digunakan untuk mengonfigurasi

## Dokumentasi Laporan Akhir
[Dokumentasi laporan akhir berupa DOCX](https://polbanacid-my.sharepoint.com/:w:/g/personal/krisna_purnama_tif423_polban_ac_id/EXRQoBDp63xKh5stfz2aMcMBHmN0EncPL9scXJdXwmr9Aw?e=kV03U2)