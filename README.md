Kode ini membuat **berbagai efek tampilan teks di LCD 16x2** menggunakan modul I2C.
Setiap **5 detik**, sistem **berpindah otomatis ke mode tampilan berikutnya**, mulai dari teks statis hingga animasi seperti progress bar.

Ada **8 mode**:

| Mode | Nama Fungsi         | Deskripsi Efek                                   |
| ---- | ------------------- | ------------------------------------------------ |
| 0    | `modeStatic()`      | Teks statis biasa                                |
| 1    | `modeRunning()`     | Teks berjalan kiri-kanan                         |
| 2    | `modeBlink()`       | Teks berkedip                                    |
| 3    | `modeMarquee()`     | Huruf muncul dari kanan (marquee)                |
| 4    | `modeTyping()`      | Huruf muncul satu per satu seperti mengetik      |
| 5    | `modeWave()`        | Huruf bergelombang (baris atas-bawah bergantian) |
| 6    | `modeAlternating()` | Teks berganti tiap detik                         |
| 7    | `modeProgressBar()` | Bar kemajuan (progress bar) horizontal           |

---

## ⚙️ **2. Penjelasan Bagian Program**

### 🔹 Library dan Inisialisasi

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
```

* Mengimpor library I2C dan LCD.
* `0x27` = alamat I2C LCD (umum digunakan).
* `16x2` = ukuran LCD (16 kolom, 2 baris).

---

### 🔹 Pengaturan Pin I2C

```cpp
Wire.begin(0, 2); // SDA=GPIO0 (D3), SCL=GPIO2 (D4)
```

Kamu memilih pin **D3 (GPIO0)** dan **D4 (GPIO2)** untuk komunikasi I2C.
📌 *Jika menggunakan pin standar NodeMCU (D1=SCL, D2=SDA)*, kamu bisa pakai:

```cpp
Wire.begin(D2, D1);
```

---

### 🔹 Mode dan Interval

```cpp
int currentMode = 0;
unsigned long lastChange = 0;
const unsigned long interval = 5000;
```

* `currentMode` menyimpan mode tampilan yang sedang aktif.
* `interval` menentukan waktu perpindahan antar-mode (5 detik).

---

### 🔹 Karakter Khusus Progress Bar

```cpp
byte bar[8] = {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111};
lcd.createChar(0, bar);
```

Membuat karakter khusus berbentuk **kotak penuh (█)** untuk efek **progress bar**.

---

### 🔹 Loop Utama

```cpp
void loop() {
  unsigned long now = millis();
  if (now - lastChange > interval) {
    currentMode = (currentMode + 1) % 8;
    lcd.clear();
    lastChange = now;
  }

  switch (currentMode) {
    case 0: modeStatic(); break;
    case 1: modeRunning(); break;
    case 2: modeBlink(); break;
    case 3: modeMarquee(); break;
    case 4: modeTyping(); break;
    case 5: modeWave(); break;
    case 6: modeAlternating(); break;
    case 7: modeProgressBar(); break;
  }
}
```

Setiap **5 detik**, mode akan berganti secara otomatis menggunakan `millis()` (non-blocking timer).

---

## ✨ **3. Penjelasan Tiap Mode (Singkat)**

### 🟩 Mode 0 – Static

```cpp
Static Message
ESP8266 + LCD
```

Menampilkan teks statis tanpa efek.

---

### 🟩 Mode 1 – Running Text

Teks "Running Text" bergerak dari kanan ke kiri, seperti spanduk berjalan.

---

### 🟩 Mode 2 – Blink

Teks “Blinking Text” muncul dan hilang setiap 500 ms (berkedip).

---

### 🟩 Mode 3 – Marquee

Huruf muncul satu per satu dari kanan ke kiri, seperti efek “masuk dari kanan”.

---

### 🟩 Mode 4 – Typing

Teks muncul huruf demi huruf seperti sedang diketik (typing effect).

---

### 🟩 Mode 5 – Wave

Huruf dari teks tampil bergelombang antara baris pertama dan kedua (zig-zag).

---

### 🟩 Mode 6 – Alternating

Menampilkan teks “Message One” dan “Message Two” bergantian setiap 1 detik.

---

### 🟩 Mode 7 – Progress Bar

Menampilkan batang kemajuan horizontal (dari kosong ke penuh) di baris pertama.

---

## 🔌 **4. Wiring LCD I2C ke ESP8266**

| LCD I2C Pin | ESP8266 Pin                        | Keterangan                              |
| ----------- | ---------------------------------- | --------------------------------------- |
| **VCC**     | **3V3**                            | Daya (beberapa modul juga mendukung 5V) |
| **GND**     | **GND**                            | Ground                                  |
| **SDA**     | **D3 (GPIO0)** atau **D2 (GPIO4)** | Jalur data                              |
| **SCL**     | **D4 (GPIO2)** atau **D1 (GPIO5)** | Jalur clock                             |

📍 **Catatan penting:**

* Kalau LCD tidak menampilkan apapun, cek alamat I2C (gunakan program *I2C Scanner*).
* Jika alamatnya berbeda (misalnya `0x3F`), ubah di baris inisialisasi:

  ```cpp
  LiquidCrystal_I2C lcd(0x3F, 16, 2);
  ```

---

## 🧩 **5. Cara Kerja Keseluruhan**

1. LCD diinisialisasi dan menyalakan backlight.
2. ESP8266 menampilkan pesan pembuka “LCD 16x2 DEMO”.
3. Setiap 5 detik, mode animasi berubah (mode 0 → mode 1 → mode 2 … → mode 7 → kembali ke mode 0).
4. Efek visual dijalankan sesuai fungsi masing-masing mode.

---

## 🖼️ **6. Contoh Tampilan pada LCD**

### Saat mode 0:

```
Static Message
ESP8266 + LCD
```

### Saat mode 1:

```
Running Text →
```

### Saat mode 7:

```
████████████████
Progress:
