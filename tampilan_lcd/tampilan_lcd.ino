#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Atur alamat I2C LCD (umumnya 0x27 atau 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variabel kontrol mode
int currentMode = 0;
unsigned long lastChange = 0;
const unsigned long interval = 5000; // ganti mode setiap 5 detik

// Karakter khusus untuk progress bar
byte bar[8] = {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111};

void setup() {
  Wire.begin(0, 2); // SDA=GPIO0 (D3), SCL=GPIO2 (D4)
 // Wire.begin(); // jika penggunakan pin D1 untuk SCL dan D2 untuk SDA
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, bar);

  lcd.setCursor(0,0);
  lcd.print("LCD 16x2 DEMO");
  delay(2000);
  lcd.clear();
}

void loop() {
  unsigned long now = millis();

  // ganti mode setiap 5 detik
  if (now - lastChange > interval) {
    currentMode = (currentMode + 1) % 8; // ada 8 mode
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

//
// ==================== MODE DEFINITIONS ====================
//

// Mode 0: Static text
void modeStatic() {
  lcd.setCursor(0,0);
  lcd.print("Static Message");
  lcd.setCursor(0,1);
  lcd.print("ESP8266 + LCD");
}

// Mode 1: Running text (scroll kiri-kanan)
void modeRunning() {
  static int pos = 0;
  lcd.setCursor(0,0);
  lcd.print("                "); // bersihkan baris
  lcd.setCursor(pos,0);
  lcd.print("Running Text");
  delay(200);
  pos--;
  if (pos < -12) pos = 16;
}

// Mode 2: Blink text
void modeBlink() {
  static bool state = false;
  lcd.setCursor(0,1);
  if (state) {
    lcd.print("Blinking Text ");
  } else {
    lcd.print("              ");
  }
  state = !state;
  delay(500);
}

// Mode 3: Marquee effect (masuk dari kanan)
void modeMarquee() {
  String msg = "Marquee Effect!";
  for (int i = 0; i <= msg.length(); i++) {
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(16 - i, 0);
    lcd.print(msg.substring(0, i));
    delay(200);
  }
}

// Mode 4: Typing effect
void modeTyping() {
  String msg = "Typing Effect...";
  lcd.setCursor(0,1);
  lcd.print("                ");
  for (int i = 0; i < msg.length(); i++) {
    lcd.setCursor(i,1);
    lcd.print(msg[i]);
    delay(150);
  }
}

// Mode 5: Wave effect
void modeWave() {
  String msg = "Wave Text Demo";
  for (int offset = 0; offset < 16; offset++) {
    lcd.clear();
    for (int i = 0; i < msg.length(); i++) {
      int row = (i + offset) % 2;
      lcd.setCursor(i, row);
      lcd.print(msg[i]);
    }
    delay(300);
  }
}

// Mode 6: Alternating text
void modeAlternating() {
  static bool toggle = false;
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  if (toggle) {
    lcd.print("Message One");
  } else {
    lcd.print("Message Two");
  }
  toggle = !toggle;
  delay(1000);
}

// Mode 7: Progress Bar
void modeProgressBar() {
  lcd.setCursor(0,1);
  lcd.print("Progress:");
  for (int i = 0; i <= 16; i++) {
    lcd.setCursor(i, 0);
    lcd.write((byte)0);
    delay(150);
  }
}
