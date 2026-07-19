#include <Arduino.h>
#include <Wire.h>

#define I2C_SDA 21
#define I2C_SCL 22
#define I2C_FREQ 100000
#define JUMLAH_MOTOR 4

String inputBuffer = "";

// Pecah string jadi token berdasarkan spasi
int splitToken(String cmd, String tokens[], int maxToken)
{
    int count = 0;
    int start = 0;
    cmd.trim();
    while (start < (int)cmd.length() && count < maxToken)
    {
        int spaceIdx = cmd.indexOf(' ', start);
        if (spaceIdx == -1)
        {
            String t = cmd.substring(start);
            t.trim();
            if (t.length() > 0) tokens[count++] = t;
            break;
        }
        else
        {
            String t = cmd.substring(start, spaceIdx);
            t.trim();
            if (t.length() > 0) tokens[count++] = t;
            start = spaceIdx + 1;
        }
    }
    return count;
}

// Parsing satu token arah+pwm, misal "a150" -> dir=1, pwm=150
bool parseMotorToken(String tok, uint8_t &dir, uint8_t &pwm)
{
    tok.toLowerCase();
    if (tok.length() < 2) return false;
    char a = tok.charAt(0);
    if (a != 'a' && a != 'b') return false;

    String pwmStr = tok.substring(1);
    for (unsigned int i = 0; i < pwmStr.length(); i++)
    {
        if (!isDigit(pwmStr.charAt(i))) return false;
    }
    int pwmVal = pwmStr.toInt();
    if (pwmVal < 0) pwmVal = 0;
    if (pwmVal > 255) pwmVal = 255;

    dir = (a == 'a') ? 1 : 0; // 1 = CW, 0 = CCW
    pwm = (uint8_t)pwmVal;
    return true;
}

// Kirim data 4 motor ke slave via I2C
void kirimPerintah(uint8_t id, uint8_t dir[], uint8_t pwm[])
{
    Wire.beginTransmission(id);
    for (int i = 0; i < JUMLAH_MOTOR; i++)
    {
        Wire.write(dir[i]);
        Wire.write(pwm[i]);
    }
    uint8_t err = Wire.endTransmission();

    Serial.print("-> ID ");
    Serial.print(id);
    Serial.print(" | ");
    for (int i = 0; i < JUMLAH_MOTOR; i++)
    {
        Serial.print("M");
        Serial.print(i + 1);
        Serial.print(":");
        Serial.print(dir[i] ? "CW" : "CCW");
        Serial.print("/");
        Serial.print(pwm[i]);
        if (i < JUMLAH_MOTOR - 1) Serial.print("  ");
    }
    Serial.print(" | status: ");
    Serial.println(err == 0 ? "OK" : "GAGAL (kode " + String(err) + ")");
}

// Scan semua alamat I2C (0-126) dan tampilkan yang merespon
void scanI2C()
{
    Serial.println("Memindai alamat I2C...");
    int jumlahDitemukan = 0;

    for (uint8_t addr = 1; addr < 127; addr++)
    {
        Wire.beginTransmission(addr);
        uint8_t err = Wire.endTransmission();

        if (err == 0)
        {
            Serial.print("Perangkat ditemukan di alamat 0x");
            if (addr < 16) Serial.print("0");
            Serial.print(addr, HEX);
            Serial.print(" (desimal ");
            Serial.print(addr);
            Serial.println(")");
            jumlahDitemukan++;
        }
        delay(5); // beri jeda kecil antar percobaan
    }

    if (jumlahDitemukan == 0)
    {
        Serial.println("Tidak ada perangkat I2C yang ditemukan.");
    }
    else
    {
        Serial.print("Selesai. Total perangkat ditemukan: ");
        Serial.println(jumlahDitemukan);
    }
}

bool prosesPerintah(String cmd)
{
    String cmdCek = cmd;
    cmdCek.trim();
    cmdCek.toLowerCase();
    if (cmdCek == "scan")
    {
        scanI2C();
        return true;
    }

    String tokens[JUMLAH_MOTOR + 1];
    int jumlahToken = splitToken(cmd, tokens, JUMLAH_MOTOR + 1);

    if (jumlahToken != JUMLAH_MOTOR + 1) return false;

    // token[0] = id, harus semua digit
    String idStr = tokens[0];
    for (unsigned int i = 0; i < idStr.length(); i++)
    {
        if (!isDigit(idStr.charAt(i))) return false;
    }
    int idVal = idStr.toInt();
    if (idVal < 1 || idVal > 127) return false;

    uint8_t dir[JUMLAH_MOTOR];
    uint8_t pwm[JUMLAH_MOTOR];

    for (int i = 0; i < JUMLAH_MOTOR; i++)
    {
        if (!parseMotorToken(tokens[i + 1], dir[i], pwm[i])) return false;
    }

    kirimPerintah((uint8_t)idVal, dir, pwm);
    return true;
}

void setup()
{
    delay(50);
    Serial.begin(115200);
    delay(50);

    Wire.begin(I2C_SDA, I2C_SCL, I2C_FREQ); // ESP32 sebagai MASTER

    Serial.println();
    Serial.println("Format perintah motor : <id> <a/b><pwm1> <a/b><pwm2> <a/b><pwm3> <a/b><pwm4>");
    Serial.println("Contoh                : 1 a0 a150 b250 b180");
    Serial.println("Scan Address I2C      : (scan)");
}

void loop()
{
    while (Serial.available())
    {
        char c = Serial.read();

        if (c == '\n' || c == '\r')
        {
            if (inputBuffer.length() > 0)
            {
                if (!prosesPerintah(inputBuffer))
                {
                    Serial.println("Perintah tidak dikenali! Gunakan: <id> <a/b><pwm1> <a/b><pwm2> <a/b><pwm3> <a/b><pwm4>  atau ketik: scan");
                }
                inputBuffer = "";
            }
        }
        else
        {
            inputBuffer += c;
        }
    }
}
