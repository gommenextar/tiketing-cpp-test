# FlyGo — Web Tiketing C++

Aplikasi web sederhana (tema tiket pesawat, mirip Traveloka) yang berjalan dari satu program C++.

## Syarat tugas

- **Struct** `Ticket`: data penerbangan
- **Array** `Ticket tickets[16]`
- **Pointer** `Ticket*` untuk traversal, comparison, dan swap
- **Bubble Sort** (harga / durasi) + log langkah
- **Linear Search** (maskapai / kota asal / tujuan) + log langkah

## Jalankan

```bat
compile.bat
tiketing.exe
```

Buka browser: http://127.0.0.1:8080

Compile manual:

```bat
g++ -std=c++11 -O2 src\main.cpp -o tiketing.exe -lws2_32
```

## Cara pakai

1. Ketik kata kunci misalnya `DPS` atau `Garuda`, lalu **Cari tiket**.
2. Pilih urutan **Harga termurah** atau **Durasi terpendek**.
3. Panel kanan menampilkan log perbandingan search dan sorting.
