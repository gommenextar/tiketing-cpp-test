@echo off
g++ -std=c++11 -O2 src\main.cpp -o tiketing.exe -lws2_32
if %errorlevel% neq 0 (
  echo Gagal compile.
  exit /b 1
)
echo Berhasil. Jalankan: tiketing.exe
echo Buka browser: http://127.0.0.1:8080
