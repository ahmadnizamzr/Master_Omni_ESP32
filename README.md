# MASTER CELL - ESP32

Kontrol Cell 4 motor.

Format perintah lewat Serial Monitor:

```bash
  <id> <arah1><pwm1> <arah2><pwm2> <arah3><pwm3> <arah4><pwm4>
```

| Var   | Fungsi                               |
| ----- | ------------------------------------ |
| id    | alamat I2C slave tujuan (1-127)      |
| arahX | 'a' = CW, 'b' = CCW (X = motor 1..4) |
| pwmX  | nilai 0 - 255                        |

Contoh di serial Monitor

```bash
  1 a0 a150 b250 b180
```

| Var      | Hasil          |
| -------- | -------------- |
| Slave ID | 1              |
| Motor 1  | CW, PWM = 0    |
| Motor 2  | CW, PWM = 150  |
| Motor 3  | CCW, PWM = 250 |
| Motor 4  | CCW, PWM = 180 |

Data yang dikirim ke slave = 8 byte (2 byte per motor):
[dir1, pwm1, dir2, pwm2, dir3, pwm3, dir4, pwm4]
dir = 1 (CW) atau 0 (CCW)

| Gerakan | Command               |
| ------- | --------------------- |
| Maju    | 1 b250 a250 a250 b250 |
| Kanan   | 1 b250 b250 a250 a250 |
| Mundur  | 1 a250 b250 b250 a250 |
| Kiri    | 1 a250 a250 b250 b250 |

## Get Project

Install my-project with Terminal VS Code

Set folder tujuan

```bash
  cd "C:\Users\Administrator\Documents\PlatformIO\Projects"
```

Clone : untuk mengambil project yang belum ada di komputer

```bash
  git clone https://github.com/ahmadnizamzr/Master_Omni_ESP32.git
```

Pull : dipakai kalau project sudah ada di komputer, tapi ada update baru di GitHub (Contributing) yang mau disinkronkan.

```bash
  git pull
```

Buka folder itu di VS Code

```bash
  code Master_Omni_ESP32
```

## Upload Project

Inisialisasi Git di project melalui terminal VS Code, pastikan berada di folder project PlatformIO, lalu jalankan

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

| Baris                         | Fungsi                                                                        |
| ----------------------------- | ----------------------------------------------------------------------------- |
| .pio                          | Mengabaikan seluruh folder hasil build/compile                                |
| .vscode/.browse.c_cpp.db\*    | File cache IntelliSense (auto-generated, tidak perlu di-upload)               |
| .vscode/c_cpp_properties.json | Config C/C++ IntelliSense yang di-generate otomatis oleh extension PlatformIO |
| .vscode/launch.json           | Config debugging (auto-generated)                                             |
| .vscode/ipch                  | Cache tambahan untuk IntelliSense                                             |

## Environment Arduino IDE

To run this project, you will need to add the following Environment

`esp32 by Espressif System Ver 2.0.11`

Additional Boards Manager

`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
