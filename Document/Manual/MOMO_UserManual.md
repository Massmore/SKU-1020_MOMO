| MOMO ESP32-S3 |
|---|
| Robot Development Board |
| WiFi • Bluetooth • AI • Arduino |
| by Massmore |

| คู่มือการใช้งาน (User Manual) |
|---|
| MCU: ESP32-S3-WROOM-1U(N16R8)  \|  Flash: 16MB  \|  PSRAM: 8MB |
| Motor Driver: TB67H451AFNG x4  \|  PWM: PCA9685PW 16CH  \|  Display: ST7789 240×240 |

# 1. Overview

MOMO ESP32-S3 คือบอร์ดพัฒนาหุ่นยนต์ (Robot Development Board) ที่ออกแบบและผลิตโดย Massmore สำหรับนักพัฒนา นักเรียน และผู้ที่สนใจงานด้านหุ่นยนต์ IoT และ AI โดยเฉพาะ บอร์ดนี้รวมเอาพลังของ ESP32-S3 เข้ากับไดรเวอร์มอเตอร์, ตัวควบคุม Servo, จอแสดงผล TFT และระบบป้องกันความเสียหายต่าง ๆ ไว้ในบอร์ดเดียว พร้อมใช้งานทันทีกับ Arduino IDE

## 1.1 Features (คุณสมบัติเด่น)

🧠  MCU: ESP32-S3-WROOM-1U(N16R8) — Dual-Core Xtensa LX7 @ 240 MHz พร้อม AI Acceleration

📡  WiFi 802.11 b/g/n (2.4 GHz) + Bluetooth 5.0 / BLE 5.0 ในตัว

💾  Flash 16 MB + PSRAM 8 MB — รองรับโปรแกรมขนาดใหญ่และ AI Model ได้

⚡  Power: TPS62162DSGT — DC-DC Step-Down ประสิทธิภาพสูงสำหรับจ่ายไฟ 3.3V

🔌  USB to Serial: CH343P — อัพโหลดโปรแกรมเร็ว รองรับ Driver บน Windows/Mac/Linux

🔒  Reverse Voltage Protection — ป้องกันความเสียหายเมื่อต่อแบตเตอรี่สลับขั้ว

🎮  Motor Driver: TB67H451AFNG x4 ช่อง — ควบคุมมอเตอร์ DC ได้ 4 ตัวพร้อมกัน

🔄  PWM Servo Driver: PCA9685PW ผ่าน I2C — ควบคุม Servo ได้สูงสุด 16 ตัว

🖥️  จอ TFT SPI 240×240 ST7789 ใช้งานกับ Library TFT_eSPI

📊  อนาล็อก I/O 11 ช่อง (IO1–IO11) / I2C / SPI / UART ครบถ้วน

🔔  Active Buzzer บนบอร์ด (GPIO 45)

🔘  User Button (GPIO 0) สำหรับ Input ควบคุมโปรแกรม

🌡️  รองรับ Arduino IDE, PlatformIO และ ESP-IDF

## 1.2 Pinout Diagram (แผนผังขาสัญญาณ)

| ชื่อบนบอร์ด | GPIO Number | หน้าที่ / หมายเหตุ |
|---|---|---|
| IO1 | GPIO 1 | Analog/Digital Input-Output |
| IO2 | GPIO 2 | Analog/Digital Input-Output |
| IO3 | GPIO 4 | Analog/Digital Input-Output |
| IO4 | GPIO 5 | Analog/Digital Input-Output |
| IO5 | GPIO 6 | Analog/Digital Input-Output |
| IO6 | GPIO 7 | Analog/Digital Input-Output |
| IO7 | GPIO 8 | Analog/Digital Input-Output |
| IO8 | GPIO 9 | Analog/Digital Input-Output |
| IO9 | GPIO 10 | Analog/Digital Input-Output |
| IO10 | GPIO 16 | Analog/Digital Input-Output |
| IO11 | GPIO 17 | Analog/Digital + TFT_CS (ใช้ร่วมกับ CS หน้าจอ) |
| BUZZER | GPIO 45 | Active Buzzer Output |
| SW_PIN | GPIO 0 | User Switch Input (Boot Button) |

| ระบบบัส | ขา | GPIO | หมายเหตุ |
|---|---|---|---|
| I2C | SDA | GPIO 14 | สำหรับ PCA9685 + Sensor |
| I2C | SCL | GPIO 15 | สำหรับ PCA9685 + Sensor |
| UART | RXD | GPIO 19 | Serial Receive |
| UART | TXD | GPIO 20 | Serial Transmit |
| SPI / TFT | MISO | GPIO 13 | TFT_MISO |
| SPI / TFT | MOSI | GPIO 11 | TFT_MOSI |
| SPI / TFT | SCLK | GPIO 12 | TFT_SCLK |
| TFT | TFT_CS | GPIO 17 | Chip Select หน้าจอ (= IO11) |
| TFT | TFT_DC | GPIO 3 | Data/Command หน้าจอ |
| TFT | TFT_RST | GPIO 18 | Reset หน้าจอ |

| มอเตอร์ช่อง | EN Pin | PH Pin | หมายเหตุ |
|---|---|---|---|
| Motor 1 | GPIO 21 (M1_EN) | GPIO 38 (M1_PH) | TB67H451AFNG CH1 |
| Motor 2 | GPIO 39 (M2_EN) | GPIO 40 (M2_PH) | TB67H451AFNG CH2 |
| Motor 3 | GPIO 41 (M3_EN) | GPIO 42 (M3_PH) | TB67H451AFNG CH3 |
| Motor 4 | GPIO 47 (M4_EN) | GPIO 48 (M4_PH) | TB67H451AFNG CH4 |

# 2. Introduction (บทนำและข้อมูลจำเพาะ)

## 2.1 บล็อกไดอะแกรมระบบ

MOMO ESP32-S3 ประกอบด้วยชิปหลักและวงจรสนับสนุนดังต่อไปนี้:

บอร์ดพัฒนาหุ่นยนต์ WiFi · Bluetooth · AI — ส่วนประกอบของบอร์ด

| ข้อ | ส่วนประกอบ | หน้าที่การทำงาน |
|---|---|---|
| 1 | VIN 6–16V Terminal Block | ขั้วต่อเทอร์มินอลบล็อกสำหรับรับไฟเลี้ยงเข้าบอร์ด รองรับแรงดัน 6–16 โวลต์ |
| 2 | XT30 (VIN 6–16V) | ขั้วต่อ XT30 สำหรับรับไฟเลี้ยงจากแบตเตอรี่ รองรับแรงดัน 6–16 โวลต์ และกระแสสูง |
| 3 | ESP32-S3-WROOM-1U | โมดูลไมโครคอนโทรลเลอร์หลัก (ชิปประมวลผล) รองรับ Wi-Fi / Bluetooth / AI พร้อมเสาอากาศภายนอก |
| 4 | USB Type-C | พอร์ต USB Type-C สำหรับจ่ายไฟ อัปโหลดโปรแกรม และสื่อสารกับคอมพิวเตอร์ |
| 5 | LED USB Status TX / RX | ไฟแสดงสถานะการรับ-ส่งข้อมูลผ่าน USB (TX / RX) |
| 6 | UART Port (MX1.25-4P) | พอร์ตสื่อสารอนุกรม UART ขนาด 4 พิน สำหรับเชื่อมต่ออุปกรณ์ภายนอก |
| 7 | SW IO0 / RST | สวิตช์ IO0 (BOOT) และ RST สำหรับเข้าสู่โหมดอัปโหลดโปรแกรมและรีเซ็ตบอร์ด |
| 8 | I²C Qwiic (JST-SH 4-Pin) | พอร์ตสื่อสาร I²C มาตรฐาน Qwiic สำหรับเชื่อมต่อเซนเซอร์และอุปกรณ์ภายนอก |
| 9 | M1 / M2 DC Motor | ขั้วต่อสำหรับขับมอเตอร์กระแสตรง ช่อง M1 และ M2 |
| 10 | M3 / M4 DC Motor | ขั้วต่อสำหรับขับมอเตอร์กระแสตรง ช่อง M3 และ M4 |
| 11 | LED Error | ไฟแสดงสถานะข้อผิดพลาดของระบบ |
| 12 | Power Switch (SW ON-OFF) | สวิตช์เปิด-ปิดการจ่ายไฟเข้าบอร์ด |
| 13 | LED PW | ไฟแสดงสถานะการจ่ายไฟ (Power) |
| 14 | PWM CH1–8 Servo Driver | ช่องสัญญาณขับเซอร์โว / PWM ช่อง 1–8 |
| 15 | PWM CH9–16 Servo Driver | ช่องสัญญาณขับเซอร์โว / PWM ช่อง 9–16 |
| 16 | Buzzer | ลำโพง Active Buzzer |
| 17 | 14-Pin Display (SPI / SD Card) | พอร์ตจอแสดงผล 14 พิน อินเทอร์เฟซ SPI และรองรับ SD Card |
| 18 | 8-Pin Display (SPI) | พอร์ตจอแสดงผล 8 พิน อินเทอร์เฟซ SPI |
| 19 | 7-Pin Display (SPI) | พอร์ตจอแสดงผล 7 พิน อินเทอร์เฟซ SPI |
| 20 | Digital / Analog I/O 11 CH | พอร์ตอินพุต/เอาต์พุต ดิจิทัลและแอนะล็อก รวม 11 ช่องสัญญาณ สำหรับงาน I/O ทั่วไป |

## 2.2 Hardware Specification

| หัวข้อ | รายละเอียด |
|---|---|
| MCU | ESP32-S3-WROOM-1U(N16R8) |
| CPU | Xtensa® Dual-Core 32-bit LX7 @ 240 MHz |
| SRAM | 512 KB (ใน Chip) + 8 MB PSRAM (ภายนอก) |
| Flash | 16 MB (N16) |
| WiFi | IEEE 802.11 b/g/n, 2.4 GHz, สูงสุด 150 Mbps |
| Bluetooth | Bluetooth 5.0 / BLE 5.0, สูงสุด 2 Mbps |
| USB | USB 2.0 OTG Full-Speed ผ่าน CH343P |
| จ่ายไฟ | 5V via USB-C หรือ VIN 7–24V |
| แรงดันระบบ | 3.3V (จาก TPS62162DSGT) |
| กระแสมอเตอร์สูงสุด | 2.5A ต่อช่อง (TB67H451AFNG) |
| จำนวน Servo | สูงสุด 16 ช่อง (PCA9685PW I2C) |
| จอแสดงผล | SPI TFT 240×240 pixels, ST7789 Driver |
| อุณหภูมิใช้งาน | 0 ถึง 60°C |
| ป้องกัน | Reverse Voltage Polarity Protection |

## 2.3 IC สำคัญบนบอร์ด

### ESP32-S3-WROOM-1U(N16R8)

ชิปหลักจาก Espressif ที่มาพร้อม Flash 16 MB และ PSRAM 8 MB ในโมดูลเดียว รองรับ AI Inference ด้วย Vector Instructions และ USB OTG ในตัว เหมาะสำหรับโปรเจกต์ Vision AI, Edge AI และ Robotics

### CH343P (USB to Serial)

ชิป USB-to-Serial ความเร็วสูงจาก WCH ที่รองรับ Driver ในระบบปฏิบัติการ Windows, macOS และ Linux โดยไม่ต้องติดตั้ง Driver เพิ่มเติมในระบบใหม่ อัพโหลดโปรแกรมได้รวดเร็ว

### PCA9685PW (16-Channel PWM)

ตัวควบคุม PWM 16 ช่องจาก NXP สื่อสารผ่าน I2C (SDA=GPIO 14, SCL=GPIO 15) ความถี่ PWM ปรับได้ 24–1526 Hz ใช้ Library Adafruit_PWMServoDriver สำหรับควบคุม Servo

### TB67H451AFNG (Motor Driver)

H-Bridge Motor Driver จาก Toshiba รองรับแรงดัน 4.5–44V กระแสสูงสุด 3.5A ต่อช่อง (2.5A ต่อเนื่อง) มี Over-Temperature และ Over-Current Protection ในตัว

# 3. User Guide — ขั้นตอนการใช้งานแบบจับมือทำ

## 3.1 ติดตั้ง Arduino IDE — Step by Step (ฉบับจับมือทำ)

| 🗺️  Road Map — ภาพรวมขั้นตอนทั้งหมด |
|---|
| STEP 1: ดาวน์โหลด Arduino IDE 2  →  STEP 2: ติดตั้ง Driver CH343P  →  STEP 3: เพิ่ม ESP32 Board Package |
| →  STEP 4: ตั้งค่าบอร์ด ESP32S3 Dev Module  →  STEP 5: เลือก Port & Upload  →  STEP 6: ตรวจสอบ Serial Monitor |

| STEP 1    ดาวน์โหลดและติดตั้ง Arduino IDE 2 |
|---|

URL: https://www.arduino.cc/en/software

เปิดเบราว์เซอร์ ไปที่  https://www.arduino.cc/en/software

เลือกดาวน์โหลดตามระบบปฏิบัติการ:

Windows  : คลิก "Windows Win 10 and newer, 64 bits" → ได้ไฟล์ .exe

macOS    : คลิก "macOS Intel, 10.15 Catalina or newer" → ได้ไฟล์ .dmg

Linux    : คลิก "AppImage 64 bits (X86-64)" → ได้ไฟล์ .AppImage

ติดตั้งตามระบบปฏิบัติการ:

Windows  : ดับเบิลคลิก .exe → Next → Install → Finish

macOS    : เปิด .dmg → ลาก Arduino IDE ไปโฟลเดอร์ Applications

Linux    : chmod +x arduino-ide_*.AppImage แล้วดับเบิลคลิกรัน

เปิดโปรแกรม Arduino IDE 2 — จะเห็นหน้าจอ Welcome Screen

| 💡 ทริป: แนะนำ Arduino IDE 2.3.x ขึ้นไป รองรับ ESP32-S3 ได้ดีที่สุด มี Auto-complete และ Debugger ในตัว |
|---|

| STEP 2    ติดตั้ง Driver USB-to-Serial (CH343P) |
|---|

บอร์ด MOMO ESP32-S3 ใช้ชิป CH343P สำหรับการเชื่อมต่อ USB ไปยัง Serial หากคอมพิวเตอร์ไม่พบ Port ให้ติดตั้ง Driver ก่อน

### Windows:

ไปที่  https://www.wch-ic.com/downloads/CH343SER_EXE.html

ดาวน์โหลดและรันไฟล์  CH343SER.EXE

คลิก INSTALL ในหน้าต่างติดตั้ง

รีสตาร์ทคอมพิวเตอร์หากระบบแจ้ง

ต่อสาย USB-C → เปิด Device Manager → ตรวจสอบว่าปรากฏ  "USB-Enhanced-SERIAL CH343 (COMx)"

### macOS:

ไปที่  https://www.wch-ic.com/downloads/CH341SER_MAC_ZIP.html

ดาวน์โหลดและติดตั้งไฟล์ .pkg

ไปที่ System Preferences → Privacy & Security → อนุญาต Driver จาก WCH

รีสตาร์ท Mac

### Linux (Ubuntu/Debian):

```
# ตรวจสอบ Port ก่อน (ส่วนใหญ่มี Driver ในตัว):
ls /dev/ttyUSB*

# หากไม่พบ ให้เพิ่มตัวเองเข้ากลุ่ม dialout:
sudo usermod -aG dialout $USER
# Logout แล้ว Login ใหม่ให้สิทธิ์มีผล
```

| ✅ วิธีตรวจสอบ: ถอดสาย USB → ดูรายการ Port ใน Device Manager → เสียบสายคืน → Port ใหม่ที่ปรากฏขึ้นคือ Port ของบอร์ด MOMO |
|---|

| STEP 3    เพิ่ม ESP32 Board Package เข้า Arduino IDE |
|---|

Arduino IDE ไม่รู้จัก ESP32 โดยค่าเริ่มต้น ต้องเพิ่ม URL ของ Espressif เพื่อให้ IDE ดาวน์โหลด Board Package มาติดตั้ง

### 3.1  เพิ่ม Board Manager URL:

เปิด Arduino IDE 2

เปิดเมนู  File → Preferences  (Windows/Linux)  หรือ  Arduino IDE → Settings  (macOS)

หาช่อง "Additional boards manager URLs" ด้านล่างของหน้าต่าง

คลิกไอคอน 📋 ด้านขวาของช่อง (กรณีมี URL อื่นอยู่แล้ว)

วาง URL นี้ในช่อง:

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

กด OK เพื่อปิดหน้าต่าง Preferences

| 💡 ทริป: หากมี URL อื่นอยู่แล้ว ให้คั่นด้วย Enter ขึ้นบรรทัดใหม่ ไม่ต้องลบ URL เดิมออก |
|---|

### 3.2  ติดตั้ง ESP32 Board Package:

ไปที่  Tools → Board → Boards Manager...  (หรือกด Ctrl+Shift+B)

รอสักครู่ให้ IDE โหลดรายการ

ในช่องค้นหา พิมพ์:  esp32

เลือก  "esp32 by Espressif Systems"

เลือกเวอร์ชัน  2.0.17  (แนะนำ — เสถียรที่สุดกับ TFT_eSPI + ESP32-S3)

คลิก "Install" แล้วรอดาวน์โหลด ประมาณ 300–500 MB ใช้เวลา 3–10 นาที

เมื่อสำเร็จ ปุ่มจะเปลี่ยนเป็น INSTALLED

| ⚠️ ทริป: ใช้เวอร์ชัน 2.0.x เท่านั้น! ห้ามใช้ 3.x เพราะ API เปลี่ยน (ledcSetup → ledcAttach) ทำให้โค้ดตัวอย่างพัง และ TFT_eSPI ยังมีบัคกับบางเวอร์ชัน 3.x |
|---|

| STEP 4    ตั้งค่าบอร์ด ESP32S3 Dev Module ใน Tools Menu |
|---|

ขั้นตอนนี้สำคัญที่สุด! ตั้งค่าผิดทำให้ PSRAM ไม่ทำงาน หรือโปรแกรม Crash ได้

ต่อสาย USB-C จากบอร์ด MOMO เข้าคอมพิวเตอร์

ไปที่  Tools → Board → esp32 → ESP32S3 Dev Module

ตั้งค่าทุกรายการใน Tools ตามตารางด้านล่างนี้:

| การตั้งค่า (Tools Menu) | ค่าที่ต้องเลือก | เหตุผล |
|---|---|---|
| Board | ESP32S3 Dev Module | ประเภทบอร์ดหลักของ ESP32-S3 |
| Upload Speed | 921600 | ความเร็วอัพโหลดสูงสุด (เร็วกว่า) |
| USB Mode | Hardware CDC and JTAG | ใช้ USB OTG ในตัว ESP32-S3 |
| USB CDC On Boot | Disabled | ปิด Serial ผ่าน USB (สำคัญ!) |
| USB Firmware MSC On Boot | Disabled | ปิด Mass Storage Mode |
| USB DFU On Boot | Disabled | ปิด DFU Mode |
| Upload Mode | UART0 / Hardware CDC | โหมดอัพโหลดมาตรฐาน |
| CPU Frequency | 240MHz (WiFi) | ความเร็วสูงสุด เหมาะกับ Robot |
| Flash Mode | QIO 80MHz | ความเร็วอ่าน Flash สูงสุด |
| Flash Size | 16MB (128Mb) ✅ | ต้องเลือก 16MB ตรงกับชิป N16 |
| Partition Scheme | Huge APP (3MB No OTA...) | พื้นที่โปรแกรมสูงสุด |
| Core Debug Level | None | ปิด Debug Log ลด noise |
| PSRAM | OPI PSRAM ✅ | สำคัญมาก! N16R8 ใช้ OPI Interface |
| Arduino Runs On | Core 1 | Core ที่ Arduino loop() ทำงาน |
| Events Run On | Core 1 | Core ที่ Event ทำงาน |
| Erase All Flash Before Sketch Upload | Disabled | ไม่ลบ Flash ก่อน Upload |

| 🔴 สำคัญที่สุด: PSRAM ต้องเลือก "OPI PSRAM" เท่านั้น! ถ้าเลือกผิดบอร์ดจะรีสตาร์ทวนซ้ำ (Crash Loop) เพราะชิป N16R8 ใช้ OPI Interface ไม่ใช่ QSPI |
|---|

| STEP 5    เลือก Port และอัพโหลดโปรแกรมแรก (Hello World) |
|---|

### 5.1  เลือก Serial Port:

ไปที่  Tools → Port

เลือก Port ที่ปรากฏขึ้นใหม่หลังต่อสาย USB:

Windows : COM3, COM4, COM5 ... (ตัวเลขอาจต่างกัน)

macOS   : /dev/cu.usbserial-xxxxxx  หรือ  /dev/cu.wchusbserial

Linux   : /dev/ttyUSB0  หรือ  /dev/ttyACM0

หากไม่เห็น Port → ตรวจสอบ Driver CH343P (STEP 2) และสาย USB-C ต้องเป็นสายส่งข้อมูล

| 💡 วิธีหา Port ที่ถูก: ถอดสาย USB → จดรายการ Port เดิม → เสียบสายคืน → Port ใหม่ที่ปรากฏขึ้นคือของบอร์ด MOMO |
|---|

### 5.2  สร้างและอัพโหลดโปรแกรม Hello World:

สร้าง Sketch ใหม่: File → New Sketch

ลบโค้ดเดิมออก แล้วพิมพ์โค้ดต่อไปนี้:

```
// ============================================================
//  MOMO ESP32-S3 — Hello World Test (โปรแกรมแรก)
//  by Massmore
// ============================================================

#define BUZZER  45   // Active Buzzer บนบอร์ด
#define SW_PIN   0   // User Button บนบอร์ด

int counter = 0;

void setup() {
  Serial.begin(115200);
  delay(1500);   // รอให้ USB CDC พร้อม
  pinMode(BUZZER, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);

  Serial.println("========================================");
  Serial.println("  MOMO ESP32-S3 by Massmore");
  Serial.println("  Hello World! - Board is Ready!");
  Serial.println("========================================");
  Serial.printf("  CPU Freq : %d MHz\n",   getCpuFrequencyMhz());
  Serial.printf("  PSRAM    : %d bytes\n", ESP.getPsramSize());
  Serial.printf("  Flash    : %d bytes\n", ESP.getFlashChipSize());
  Serial.printf("  Free RAM : %d bytes\n", ESP.getFreeHeap());
  Serial.println("========================================");

  // Beep 2 ครั้งเมื่อพร้อม
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER, HIGH); delay(150);
    digitalWrite(BUZZER, LOW);  delay(100);
  }
}

void loop() {
  counter++;
  Serial.printf("[%4d] Uptime: %lu s | Button: %s\n",
    counter, millis()/1000,
    digitalRead(SW_PIN)==LOW ? "PRESSED" : "Released");

  if (digitalRead(SW_PIN) == LOW) {
    digitalWrite(BUZZER, HIGH); delay(80);
    digitalWrite(BUZZER, LOW);
  }
  delay(500);
}
```

### 5.3  Compile และ Upload:

คลิกปุ่ม  ✔️  Verify  เพื่อ Compile ตรวจโค้ดก่อน — รอจนแถบล่างแสดง  Done compiling

คลิกปุ่ม  ➡️  Upload  เพื่ออัพโหลด — รอจนแถบล่างแสดง  Done uploading

| 🔥 Upload ไม่ได้? กด BOOT ค้าง → กด RST สั้นๆ แล้วปล่อย → ปล่อย BOOT → คลิก Upload ทันที บอร์ดจะเข้าโหมด Download อัตโนมัติ |
|---|

| STEP 6    ตรวจสอบผลลัพธ์ใน Serial Monitor |
|---|

ไปที่  Tools → Serial Monitor  (หรือกด Ctrl+Shift+M)

ตั้งค่า Baud Rate เป็น  115200  (มุมขวาล่าง)

กดปุ่ม RST บนบอร์ดเพื่อรีสตาร์ท

ตรวจสอบผลลัพธ์ที่ควรเห็น:

```
========================================
  MOMO ESP32-S3 by Massmore
  Hello World! - Board is Ready!
========================================
  CPU Freq : 240 MHz            <-- ถูกต้อง
  PSRAM    : 8388608 bytes      <-- ~8MB = OPI PSRAM ทำงานปกติ
  Flash    : 16777216 bytes     <-- 16MB ถูกต้อง
  Free RAM : 314056 bytes
========================================
[   1] Uptime: 0 s | Button: Released
[   2] Uptime: 1 s | Button: Released
...
// ลองกดปุ่ม SW บนบอร์ด จะเห็น:
[  10] Uptime: 5 s | Button: PRESSED
```

| ✅ ถ้าเห็น PSRAM: 8388608 bytes (8MB) แสดงว่าตั้งค่า OPI PSRAM ถูกต้อง บอร์ดพร้อมใช้งาน 100%! |
|---|

| ✓ | ขั้นตอน | ผลลัพธ์ที่ควรได้ |
|---|---|---|
| ☐ | STEP 1: ติดตั้ง Arduino IDE 2.3.x | เปิดโปรแกรม Arduino IDE 2 ได้ |
| ☐ | STEP 2: ติดตั้ง Driver CH343P | เห็น COM Port ใน Device Manager |
| ☐ | STEP 3: เพิ่ม esp32 Board Package v2.0.17 | พบ ESP32S3 Dev Module ใน Tools Board |
| ☐ | STEP 4: ตั้งค่า Flash 16MB + OPI PSRAM | การตั้งค่าครบตามตาราง |
| ☐ | STEP 5: เลือก Port + Upload สำเร็จ | แถบแสดง Done uploading |
| ☐ | STEP 6: Serial Monitor แสดงผล | PSRAM 8MB + CPU 240MHz + Button ทำงาน |

| 🎉 เมื่อผ่านครบทั้ง 6 ขั้นตอน บอร์ด MOMO ESP32-S3 พร้อมใช้งานจริง! ต่อไปติดตั้ง Library ในหัวข้อ 3.2 |
|---|

## 3.2 ติดตั้ง Library ที่จำเป็น

| 📦  ต้องติดตั้ง Library ทั้งหมดนี้ก่อนรันโปรแกรมตัวอย่าง: TFT_eSPI, Adafruit PWM Servo Driver Library, Adafruit BusIO |
|---|

### Library 1: TFT_eSPI (สำหรับหน้าจอ ST7789)

ไปที่ Sketch → Include Library → Manage Libraries

ค้นหา "TFT_eSPI" แล้วกด Install (by Bodmer)

เปิด File Manager ไปที่ Documents/Arduino/libraries/TFT_eSPI/

เปิดไฟล์ User_Setup.h และแก้ไขให้ตรงกับบอร์ด MOMO:

```
// ========= User_Setup.h สำหรับ MOMO ESP32-S3 =========
#define ST7789_DRIVER      // ไดรเวอร์หน้าจอ
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

#define TFT_MISO 13
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS   17   // IO11 บนบอร์ด
#define TFT_DC    3
#define TFT_RST  18

#define LOAD_GLCD   // ฟอนต์ขนาดเล็ก
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

#define SPI_FREQUENCY  40000000
```

| ⚠️ ทริป: ทุกครั้งที่ Update Library TFT_eSPI จะต้องตั้งค่า User_Setup.h ใหม่ทุกครั้ง! |
|---|

### Library 2: Adafruit PWM Servo Driver (สำหรับ PCA9685)

ไปที่ Sketch → Include Library → Manage Libraries

ค้นหา "Adafruit PWM Servo Driver" แล้วกด Install

เมื่อถามให้ติดตั้ง Dependencies ให้กด "Install All"

## 3.3 โปรแกรมตัวอย่าง (Example Code)

### ตัวอย่างที่ 1: อ่านค่า Analog Input (IO1–IO11)

ทดสอบการอ่านค่า ADC จากขา IO ทั้ง 11 ช่องบนบอร์ด พร้อมแสดงผลใน Serial Monitor

```
// ============================================================
//  MOMO ESP32-S3 — ทดสอบอ่านค่า Analog Input
//  by Massmore
// ============================================================

// ─── Pin Definitions ────────────────────────────────────────
#define IO1   1
#define IO2   2
#define IO3   4
#define IO4   5
#define IO5   6
#define IO6   7
#define IO7   8
#define IO8   9
#define IO9   10
#define IO10  16
#define IO11  17
#define BUZZER  45
#define SW_PIN   0

const int analogPins[] = {IO1,IO2,IO3,IO4,IO5,IO6,
                           IO7,IO8,IO9,IO10,IO11};
const int numPins = 11;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== MOMO ESP32-S3 Analog Test ===");
  pinMode(BUZZER, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
  // Beep เสียงสั้น 1 ครั้งเมื่อพร้อม
  digitalWrite(BUZZER, HIGH); delay(100);
  digitalWrite(BUZZER, LOW);
}

void loop() {
  Serial.println("--- Analog Read ---");
  for (int i = 0; i < numPins; i++) {
    int raw = analogRead(analogPins[i]);
    float volt = raw * 3.3 / 4095.0;
    Serial.printf("IO%d (GPIO%d): %4d  = %.2f V\n",
                   i+1, analogPins[i], raw, volt);
  }
  Serial.printf("Button: %s\n",
    digitalRead(SW_PIN)==LOW ? "PRESSED" : "Released");
  Serial.println();
  delay(500);
}
```

| 💡 ทริป: ESP32-S3 ADC มี 12-bit resolution (0–4095) แรงดันอ้างอิง 3.3V ใช้สูตร voltage = raw × 3.3 / 4095.0 เพื่อแปลงเป็นโวลต์ |
|---|

### ตัวอย่างที่ 2: แสดงผลบนหน้าจอ TFT ST7789 240×240

แสดงข้อความ สี และกราฟิกบนจอ TFT โดยใช้ Library TFT_eSPI

```
// ============================================================
//  MOMO ESP32-S3 — ทดสอบจอ TFT ST7789 240x240
//  Library: TFT_eSPI (ต้องตั้งค่า User_Setup.h ก่อน)
//  by Massmore
// ============================================================
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(0);       // 0=Portrait, 1=Landscape
  tft.fillScreen(TFT_BLACK);

  // ─── วาดหัวข้อ ─────────────────────────────────
  tft.fillRect(0, 0, 240, 50, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setTextSize(2);
  tft.setCursor(20, 15);
  tft.println("MOMO ESP32-S3");

  // ─── แสดงข้อความ ───────────────────────────────
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 65);
  tft.println("by Massmore");
  tft.setCursor(10, 85);
  tft.println("Robot Dev Board");

  // ─── วาดรูปทรง ─────────────────────────────────
  tft.fillCircle(120, 160, 40, TFT_RED);
  tft.drawCircle(120, 160, 45, TFT_WHITE);
  tft.fillRect(80, 210, 80, 20, TFT_YELLOW);
}

void loop() {
  // แสดงเวลาที่ผ่านไป
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 105);
  tft.printf("Uptime: %lu s  ", millis()/1000);
  delay(1000);
}
```

### ตัวอย่างที่ 3: ควบคุม Servo ด้วย PCA9685

ควบคุม Servo Motor ผ่าน I2C PCA9685 โดยใช้ Adafruit Library (Servo1=LED0 … Servo16=LED15)

```
// ============================================================
//  MOMO ESP32-S3 — ควบคุม Servo ด้วย PCA9685
//  Library: Adafruit PWM Servo Driver
//  I2C: SDA=GPIO14, SCL=GPIO15
//  by Massmore
// ============================================================
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// PCA9685 Address เริ่มต้น 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  150   // Pulse น้อยสุด (0 องศา)
#define SERVOMAX  600   // Pulse มากสุด (180 องศา)
#define SERVO_FREQ 50   // ความถี่ 50 Hz สำหรับ Servo

// แปลงองศาเป็น Pulse
int angleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(14, 15);        // SDA=14, SCL=15
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);
  Serial.println("PCA9685 Ready!");
}

void loop() {
  // Servo1 (LED0) กวาด 0 → 180 องศา
  Serial.println("Sweep 0 to 180");
  for (int angle = 0; angle <= 180; angle += 5) {
    pwm.setPWM(0, 0, angleToPulse(angle)); // Servo1=LED0
    delay(20);
  }
  delay(500);
  // กลับ 180 → 0 องศา
  Serial.println("Sweep 180 to 0");
  for (int angle = 180; angle >= 0; angle -= 5) {
    pwm.setPWM(0, 0, angleToPulse(angle));
    delay(20);
  }
  delay(500);
}
```

### ตัวอย่างที่ 4: ควบคุมมอเตอร์ DC 4 ช่อง (TB67H451AFNG)

ควบคุมมอเตอร์ทั้ง 4 ช่องด้วย EN/PH Mode — EN ควบคุมความเร็ว (PWM), PH ควบคุมทิศทาง

```
// ============================================================
//  MOMO ESP32-S3 — ควบคุม Motor TB67H451AFNG 4 ช่อง
//  EN/PH Mode: EN=PWM ความเร็ว, PH=ทิศทาง HIGH/LOW
//  by Massmore
// ============================================================

// ─── Motor Pin Definitions ──────────────────────────────────
#define M1_EN 21   #define M1_PH 38
#define M2_EN 39   #define M2_PH 40
#define M3_EN 41   #define M3_PH 42
#define M4_EN 47   #define M4_PH 48

// ─── PWM Channels ──────────────────────────────────────────
#define PWM_FREQ   1000    // 1 kHz
#define PWM_RES    8       // 8-bit (0-255)
#define CH_M1_EN   0
#define CH_M2_EN   1
#define CH_M3_EN   2
#define CH_M4_EN   3

void motorSetup() {
  // ตั้งค่า Direction pins
  pinMode(M1_PH, OUTPUT); pinMode(M2_PH, OUTPUT);
  pinMode(M3_PH, OUTPUT); pinMode(M4_PH, OUTPUT);
  // ตั้งค่า PWM สำหรับ EN pins
  ledcAttach(M1_EN, PWM_FREQ, PWM_RES);
  ledcAttach(M2_EN, PWM_FREQ, PWM_RES);
  ledcAttach(M3_EN, PWM_FREQ, PWM_RES);
  ledcAttach(M4_EN, PWM_FREQ, PWM_RES);
}

// ควบคุมมอเตอร์: motor=1-4, speed=0-255, dir=1(ไปข้างหน้า)/0(ถอย)
void motorControl(int motor, int speed, int dir) {
  speed = constrain(speed, 0, 255);
  int enPin, phPin;
  switch(motor) {
    case 1: enPin=M1_EN; phPin=M1_PH; break;
    case 2: enPin=M2_EN; phPin=M2_PH; break;
    case 3: enPin=M3_EN; phPin=M3_PH; break;
    case 4: enPin=M4_EN; phPin=M4_PH; break;
    default: return;
  }
  digitalWrite(phPin, dir ? HIGH : LOW);
  ledcWrite(enPin, speed);
}

void motorStop(int motor) { motorControl(motor, 0, 0); }
void allStop() { for(int i=1;i<=4;i++) motorStop(i); }

void setup() {
  Serial.begin(115200);
  motorSetup();
  Serial.println("Motor Ready!");
}

void loop() {
  Serial.println("Forward All");
  for(int i=1;i<=4;i++) motorControl(i, 180, 1);
  delay(2000);

  Serial.println("Reverse All");
  for(int i=1;i<=4;i++) motorControl(i, 180, 0);
  delay(2000);

  Serial.println("Stop");
  allStop();
  delay(1000);
}
```

| 💡 ทริป: Arduino ESP32 เวอร์ชัน 3.x ใช้ ledcAttach(pin, freq, res) แทน ledcSetup()+ledcAttachPin() แบบเก่า หากใช้เวอร์ชัน 2.x ให้ใช้ ledcSetup(ch, freq, res) แล้วตามด้วย ledcAttachPin(pin, ch) |
|---|

### ตัวอย่างที่ 5: Buzzer + Button (GPIO 45 + GPIO 0)

ทดสอบการใช้งาน Active Buzzer และปุ่ม User Button บนบอร์ด

```
// ============================================================
//  MOMO ESP32-S3 — ทดสอบ Buzzer & Button
//  BUZZER=GPIO45 (Active HIGH), SW_PIN=GPIO0 (Active LOW)
//  by Massmore
// ============================================================

#define BUZZER  45
#define SW_PIN   0

void beep(int duration_ms) {
  digitalWrite(BUZZER, HIGH);
  delay(duration_ms);
  digitalWrite(BUZZER, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
  Serial.println("Press button to beep!");
  beep(200);  // เสียงต้อนรับ
}

void loop() {
  if (digitalRead(SW_PIN) == LOW) {  // ปุ่มถูกกด
    Serial.println("Button Pressed!");
    beep(100); delay(50);
    beep(100); delay(50);
    beep(300);
    while(digitalRead(SW_PIN)==LOW); // รอปล่อยปุ่ม
  }
}
```

### ตัวอย่างที่ 6: All-in-One Demo (จอ + มอเตอร์ + Buzzer)

โปรแกรมรวมแสดงสถานะบนจอ TFT พร้อมควบคุมมอเตอร์และ Buzzer — โครงสำหรับพัฒนาต่อ

```
// ============================================================
//  MOMO ESP32-S3 — All-in-One Demo
//  แสดงสถานะบนจอ + มอเตอร์ + Buzzer
//  by Massmore
// ============================================================
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

TFT_eSPI tft = TFT_eSPI();
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define BUZZER  45
#define SW_PIN   0
#define M1_EN 21  #define M1_PH 38
#define M2_EN 39  #define M2_PH 40

void beep(int ms) {
  digitalWrite(BUZZER,HIGH); delay(ms); digitalWrite(BUZZER,LOW);
}

void drawStatus(String msg, uint16_t color) {
  tft.fillRect(0, 200, 240, 40, TFT_BLACK);
  tft.setTextColor(color, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 210);
  tft.println(msg);
}

void setup() {
  Serial.begin(115200);
  // Init TFT
  tft.init(); tft.setRotation(0);
  tft.fillScreen(TFT_NAVY);
  tft.setTextColor(TFT_WHITE, TFT_NAVY);
  tft.setTextSize(2);
  tft.setCursor(20,10); tft.println("MOMO S3");
  tft.setTextSize(1);
  tft.setCursor(10,40); tft.println("by Massmore");
  // Init I2C + PCA9685
  Wire.begin(14, 15);
  pwm.begin(); pwm.setPWMFreq(50);
  // Init Motors
  pinMode(M1_PH,OUTPUT); pinMode(M2_PH,OUTPUT);
  ledcAttach(M1_EN, 1000, 8);
  ledcAttach(M2_EN, 1000, 8);
  // Init Buzzer + Button
  pinMode(BUZZER,OUTPUT);
  pinMode(SW_PIN,INPUT_PULLUP);
  beep(300);
  drawStatus("Ready!", TFT_GREEN);
}

void loop() {
  if (digitalRead(SW_PIN) == LOW) {
    drawStatus("Motor Forward!", TFT_YELLOW);
    digitalWrite(M1_PH, HIGH); ledcWrite(M1_EN, 200);
    digitalWrite(M2_PH, HIGH); ledcWrite(M2_EN, 200);
    beep(100);
    delay(1500);
    ledcWrite(M1_EN, 0); ledcWrite(M2_EN, 0);
    drawStatus("Stopped.", TFT_WHITE);
    while(digitalRead(SW_PIN)==LOW);
  }
  // อัพเดตเวลา
  tft.setTextColor(TFT_CYAN, TFT_NAVY);
  tft.setCursor(10, 60);
  tft.printf("Uptime: %lu s  ", millis()/1000);
  delay(500);
}
```

### ตัวอย่างที่ 7: สแกนหาอุปกรณ์ I2C (I2C Scanner)

เครื่องมือ Debug ที่ต้องมี! ใช้ตรวจสอบว่าบอร์ดมองเห็น PCA9685 (Address 0x40) และเซนเซอร์ I2C อื่น ๆ ที่ต่อเข้ามาหรือไม่ ก่อนเขียนโปรแกรมควบคุมจริง

```
// ============================================================
//  MOMO ESP32-S3 — I2C Scanner (สแกนหาอุปกรณ์บนบัส I2C)
//  I2C: SDA=GPIO14, SCL=GPIO15
//  by Massmore
// ============================================================
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin(14, 15);            // SDA=14, SCL=15 เท่านั้น
  Serial.println("=== MOMO I2C Scanner ===");
}

void loop() {
  byte count = 0;
  Serial.println("Scanning...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.printf("  Found device at 0x%02X\n", addr);
      count++;
    }
  }
  if (count == 0) Serial.println("  No I2C devices found!");
  else Serial.printf("  Total: %d device(s)\n\n", count);
  delay(3000);
}
```

| 💡 ทริป: PCA9685 ใช้ Address 0x40 เป็นค่าเริ่มต้น ถ้าสแกนไม่เจอ 0x40 ให้ตรวจว่าเรียก Wire.begin(14,15) ถูกต้อง และบอร์ดได้รับไฟเลี้ยงครบ (บอร์ดมีตัวต้านทาน Pull-up 10kΩ บน SDA/SCL ให้แล้ว ไม่ต้องต่อเพิ่ม) |
|---|

### ตัวอย่างที่ 8: เชื่อมต่อ WiFi (Scan + Connect)

ทดสอบ WiFi 2.4 GHz ในตัว ESP32-S3 — สแกนเครือข่ายรอบตัว แล้วเชื่อมต่อเข้ากับ Router ของคุณ เป็นจุดเริ่มต้นของงาน IoT และการควบคุมหุ่นยนต์ผ่านเครือข่าย

```
// ============================================================
//  MOMO ESP32-S3 — WiFi Scan & Connect
//  by Massmore
// ============================================================
#include <WiFi.h>

const char* SSID = "YOUR_WIFI_NAME";       // << แก้เป็นชื่อ WiFi ของคุณ
const char* PASS = "YOUR_WIFI_PASSWORD";   // << แก้เป็นรหัสผ่าน

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 1) สแกนเครือข่ายรอบตัว
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  int n = WiFi.scanNetworks();
  Serial.printf("Found %d networks:\n", n);
  for (int i = 0; i < n; i++)
    Serial.printf("  %2d) %-24s  %4d dBm\n",
                  i+1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));

  // 2) เชื่อมต่อ
  Serial.printf("\nConnecting to %s ...\n", SSID);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.printf("\nConnected!  IP = %s\n", WiFi.localIP().toString().c_str());
}

void loop() {}
```

| 💡 ทริป: ESP32-S3 รองรับเฉพาะ WiFi 2.4 GHz (ไม่รองรับ 5 GHz) บอร์ด MOMO เป็นรุ่นเสาอากาศภายนอก (WROOM-1U) อย่าลืมต่อเสาอากาศก่อนใช้งานเพื่อสัญญาณที่ดีที่สุด |
|---|

### ตัวอย่างที่ 9: Bluetooth Low Energy (BLE)

ประกาศตัวอุปกรณ์ BLE ชื่อ "MOMO-ESP32S3" พร้อมส่งค่าตัวนับให้สมาร์ทโฟนอ่านผ่าน Characteristic — ใช้สำหรับรีโมตคอนโทรลหุ่นยนต์หรือส่งค่าเซนเซอร์แบบประหยัดพลังงาน

```
// ============================================================
//  MOMO ESP32-S3 — Bluetooth LE (BLE Server + Notify)
//  ทดสอบด้วยแอป nRF Connect บนมือถือ
//  by Massmore
// ============================================================
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "12345678-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "12345678-0001-1000-8000-00805f9b34fb"

BLECharacteristic* pChar;
uint32_t value = 0;

void setup() {
  Serial.begin(115200);
  BLEDevice::init("MOMO-ESP32S3");
  BLEServer* pServer = BLEDevice::createServer();
  BLEService* pService = pServer->createService(SERVICE_UUID);
  pChar = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pChar->addDescriptor(new BLE2902());
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("BLE advertising as 'MOMO-ESP32S3'");
}

void loop() {
  pChar->setValue((uint8_t*)&value, 4);
  pChar->notify();
  value++;
  delay(1000);
}
```

| 💡 ทริป: ดาวน์โหลดแอป nRF Connect (มีทั้ง Android/iOS) เพื่อค้นหาและทดสอบอุปกรณ์ "MOMO-ESP32S3" — BLE ใช้พลังงานต่ำกว่า Bluetooth Classic เหมาะกับงานแบตเตอรี่ |
|---|

## 3.4 โปรเจคตัวอย่าง: หุ่นยนต์รถ 4 ล้อ + กริปเปอร์ (Robot Car)

โปรเจครวมยอด! นำไดรเวอร์มอเตอร์ 4 ช่อง (TB67H451AFNG) มาทำหุ่นยนต์รถ 4 ล้อ ควบคุมการเดินหน้า–ถอยหลัง–เลี้ยว และเพิ่มแขนหนีบ (Gripper) ด้วยเซอร์โวผ่าน PCA9685 — เป็นโครงสร้างพื้นฐานที่ต่อยอดเป็นหุ่นยนต์ AI ได้

| 🔌  การต่อสาย (Wiring): |
|---|
| • ล้อซ้าย → Motor 1 (M1) และ Motor 3 (M3) |
| • ล้อขวา → Motor 2 (M2) และ Motor 4 (M4) |
| • แขนหนีบ (Gripper Servo) → ช่อง CH0 ของ PCA9685 |
| • ไฟเลี้ยง → แบตเตอรี่ผ่านขั้ว XT30 (ห้ามใช้ไฟ USB ขับมอเตอร์) |

```
// ============================================================
//  MOMO ESP32-S3 — Robot Car 4WD + Gripper
//  ล้อซ้าย=M1,M3  ล้อขวา=M2,M4  Gripper=PCA9685 CH0
//  by Massmore
// ============================================================
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// --- Motor pins (EN=ความเร็ว/PWM, PH=ทิศทาง) ---
const int EN[4] = {21, 39, 41, 47};   // M1..M4
const int PH[4] = {38, 40, 42, 48};   // M1..M4
#define PWM_FREQ 1000
#define PWM_RES  8

void motorSetup() {
  for (int i = 0; i < 4; i++) {
    pinMode(PH[i], OUTPUT);
    ledcAttach(EN[i], PWM_FREQ, PWM_RES);   // Arduino ESP32 core 3.x
  }
}
// ขับมอเตอร์ตัวที่ i ด้วยความเร็ว -255..255 (ลบ=ถอยหลัง)
void motor(int i, int spd) {
  spd = constrain(spd, -255, 255);
  digitalWrite(PH[i], spd >= 0 ? HIGH : LOW);
  ledcWrite(EN[i], abs(spd));
}
// ขับแบบรถถัง: กำหนดความเร็วล้อซ้าย/ขวา
void drive(int left, int right) {
  motor(0, left);  motor(2, left);     // ซ้าย = M1, M3
  motor(1, right); motor(3, right);    // ขวา = M2, M4
}
void gripperOpen()  { pwm.setPWM(0, 0, 600); }   // CH0
void gripperClose() { pwm.setPWM(0, 0, 150); }

void setup() {
  Serial.begin(115200);
  motorSetup();
  Wire.begin(14, 15);
  pwm.begin();
  pwm.setPWMFreq(50);
  Serial.println("Robot Ready!");
}


void loop() {
  drive(200, 200);  delay(1500);   // เดินหน้า
  drive(-200,-200); delay(1000);   // ถอยหลัง
  drive(200, -200); delay(700);    // เลี้ยวขวา (หมุนตัว)
  drive(0, 0);      delay(500);    // หยุด
  gripperClose();   delay(800);    // หนีบ
  gripperOpen();    delay(800);    // ปล่อย
}
```

| 💡 ทริป: ถ้าหุ่นยนต์เลี้ยวผิดทาง ให้สลับค่าความเร็วซ้าย/ขวา หรือสลับขา PH ของมอเตอร์ฝั่งนั้น และควรปรับค่ามุมกริปเปอร์ (150–600) ให้พอดีกับเซอร์โวที่ใช้ |
|---|

### การประยุกต์ใช้งาน (Applications)

| 🤖  ต่อยอดโปรเจคหุ่นยนต์ได้หลากหลาย: |
|---|
| • หุ่นยนต์เดินตามเส้น / หลบสิ่งกีดขวาง — ต่อเซนเซอร์ที่ขา ADC (IO1–IO11) |
| • หุ่นยนต์ AI Vision — ใช้ ESP-DL + กล้อง ตรวจจับใบหน้า/วัตถุ (ใช้ PSRAM 8MB) |
| • รถสำรวจกล้องความร้อน — ต่อ AMG8833 (มีตัวอย่างในโฟลเดอร์ Example_Project) |
| • แขนกลหลายแกน — ขับเซอร์โวได้สูงสุด 16 ตัวผ่าน PCA9685 |
| • รีโมตคอนโทรลผ่าน WiFi / BLE — ควบคุมผ่านเว็บหรือแอปมือถือ |

# 4. Resources (แหล่งข้อมูลและดาวน์โหลด)

## 4.1 GitHub & Library

| ชื่อ | URL / แหล่งดาวน์โหลด |
|---|---|
| Espressif Arduino Core (ESP32) | https://github.com/espressif/arduino-esp32 |
| TFT_eSPI Library (Bodmer) | https://github.com/Bodmer/TFT_eSPI |
| Adafruit PWM Servo Driver | https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library |
| PCA9685 Datasheet | https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf |

## 4.2 Datasheet ของ IC บนบอร์ด

| IC | หน้าที่ | แหล่ง Datasheet |
|---|---|---|
| ESP32-S3-WROOM-1 | MCU Module | https://www.espressif.com/sites/default/files/documentation/esp32-s3-wroom-1_wroom-1u_datasheet_en.pdf |
| CH343P | USB to Serial | https://www.wch-ic.com/products/CH343.html |
| PCA9685PW | 16-CH PWM I2C | https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf |
| TB67H451AFNG | Motor Driver | Document/Hardware/Datasheet/TB67H451AFNG_datasheet_en_20201217.pdf |
| ST7789 | TFT LCD Driver | https://www.newhavendisplay.com/appnotes/datasheets/LCDs/ST7789V.pdf |

## 4.3 เว็บไซต์อ้างอิงที่เป็นประโยชน์

Espressif ESP32-S3 Technical Reference Manual: https://www.espressif.com/sites/default/files/documentation/esp32-s3_technical_reference_manual_en.pdf

Arduino ESP32 Documentation: https://docs.espressif.com/projects/arduino-esp32

TFT_eSPI Documentation: https://doc-tft-espi.readthedocs.io

Adafruit PCA9685 Guide: https://learn.adafruit.com/16-channel-pwm-servo-driver

## 4.4 Troubleshooting — แก้ปัญหาที่พบบ่อย

| ปัญหา | สาเหตุ | วิธีแก้ไข |
|---|---|---|
| อัพโหลดไม่ได้ | ไม่เข้า Download Mode | กด BOOT → กด RST → ปล่อย RST → ปล่อย BOOT แล้วอัพโหลด |
| Serial Monitor ไม่มีข้อมูล | USB CDC เปิดอยู่ | ตั้งค่า USB CDC On Boot: Disable ใน Arduino Tools |
| หน้าจอไม่แสดงผล | User_Setup.h ผิด | ตรวจสอบ TFT_CS=17, TFT_DC=3, TFT_RST=18 ใน User_Setup.h |
| Servo กระตุก | ความถี่ PWM ไม่ถูก | ตั้งค่า setPWMFreq(50) และ setOscillatorFrequency(27000000) |
| มอเตอร์ไม่หมุน | ไม่ได้เรียก ledcAttach | ตรวจว่าเรียก ledcAttach() สำหรับทุก EN pin ก่อนใช้งาน |
| PCA9685 ไม่ตอบสนอง | I2C ผิดขา | ใช้ Wire.begin(14, 15) — SDA=14, SCL=15 เท่านั้น |
| ADC อ่านค่าผิด | ขาอื่นรบกวน | ตรวจ IO11 (GPIO17) ใช้ร่วมกับ TFT_CS อย่าใช้พร้อมกัน |

## 4.5 เคล็ดลับผู้เชี่ยวชาญ (Pro Tips)

| 🧠 ใช้ PSRAM: ประกาศตัวแปรขนาดใหญ่ด้วย ps_malloc() เพื่อใช้ PSRAM 8MBตัวอย่าง: uint8_t* buf = (uint8_t*)ps_malloc(1024*1024); |
|---|

| ⚡ เพิ่มความเร็ว TFT: ลอง SPI_FREQUENCY 80000000 ใน User_Setup.h หากหน้าจอรองรับ |
|---|

| 🤖 AI บน ESP32-S3: ใช้ ESP-DL หรือ TensorFlow Lite Micro สำหรับงาน Edge AI เช่น Keyword Detection หรือ Image Classification ด้วย PSRAM 8MB ที่มีบนบอร์ด |
|---|

| 🔋 ประหยัดพลังงาน: ใช้ esp_deep_sleep_start() สำหรับโหมดประหยัดพลังงาน ESP32-S3 ใช้กระแสเพียง ~7µA ในโหมด Deep Sleep |
|---|

MOMO ESP32-S3 Robot Development Board  |  by Massmore  |  Version 1.0
