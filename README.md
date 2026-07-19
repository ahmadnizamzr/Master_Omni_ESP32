# MASTER I2C - ESP32
Kontrol 4 motor per slave.

Format perintah lewat Serial Monitor:
```bash
  <id> <arah1><pwm1> <arah2><pwm2> <arah3><pwm3> <arah4><pwm4>
```
id    : alamat I2C slave tujuan (1-127)

arahX : 'a' = CW, 'b' = CCW  (X = motor 1..4)
  
pwmX  : nilai 0 - 255

Contoh di serial Monitor

```bash
  1 a0 a150 b250 b180
```
  -> slave id 1:
     motor1 = CW,  pwm 0
     motor2 = CW,  pwm 150
     motor3 = CCW, pwm 250
     motor4 = CCW, pwm 180

```bash
  npm run test
```

Data yang dikirim ke slave = 8 byte (2 byte per motor):
  [dir1, pwm1, dir2, pwm2, dir3, pwm3, dir4, pwm4]
  dir = 1 (CW) atau 0 (CCW)
## Get Project

Install my-project with Terminal VS Code

Set foder tujuan
```bash
  cd "C:\Users\Administrator\Documents\PlatformIO\Projects"
```
Clone : untuk mengambil project yang belum ada di komputer

    
```bash
  git clone https://github.com/ahmadnizamzr/Master_Omni_ESP32.git
```
Pull  : dipakai kalau project sudah ada di komputer, tapi ada update baru di GitHub (Contributing) yang mau disinkronkan.

```bash
  git pull
```
Buka folder itu di VS Code
```bash
  code Master_Omni_ESP32
```
## Upload Project

Inisialisasi Git ti project melalui terminal VS Code, pastikan berada di folder project PlatformIO, lalu jalankan

```bash
  git init
  git add .
  git commit -m "Initial commit"  
```
Hubungkan ke GitHub dan push
    
```bash
  git branch -M main
  git remote add origin https://github.com/ahmadnizamzr/Master_Omni_ESP32.git
  git push -u origin main
```

## .gitignore

| Baris            | Fungsi    |
| ----------------- | ------------------------------------------------------------------ |
| .pio | Mengabaikan seluruh folder hasil build/compile|
| .vscode/.browse.c_cpp.db* | File cache IntelliSense (auto-generated, tidak perlu di-upload) |
| .vscode/c_cpp_properties.json | Config C/C++ IntelliSense yang di-generate otomatis oleh extension PlatformIO |
| .vscode/launch.json | Config debugging (auto-generated) |
| .vscode/ipch | Cache tambahan untuk IntelliSense |

