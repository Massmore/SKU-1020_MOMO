#include "SD.h"
#include "SPI.h"
// ตรวจสอบให้แน่ใจว่าไฟล์ cst816d.h และ cst816d.cpp อยู่ในโฟลเดอร์เดียวกับไฟล์ .ino
#include "cst816d.h" 

// --- Config Pins ---
// กำหนดขา SPI สำหรับ SD Card
#define SPI_MISO 13 
#define SPI_MOSI 11
#define SPI_SCLK 12
#define SD_CS 0

// กำหนดขา Touch Screen
#define TP_INT -1
#define TP_RST -1
#define TP_SDA 14
#define TP_SCL 15

// สร้างตัวแปร SPI
SPIClass fspi(FSPI);

uint64_t cardSize;
toucht_coords_t toucht_coords;
uint16_t sd_count = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 SD Card & Touch Test (No LCD)");

  // 1. เริ่มต้น Touch Screen
  // ฟังก์ชันนี้มาจากไลบรารี cst816d ที่คุณใช้
  touch_init(TP_SDA, TP_SCL, TP_RST, TP_INT);

  // 2. เริ่มต้น SD Card
  // เริ่มต้น SPI Bus
  fspi.begin(SPI_SCLK, SPI_MISO, SPI_MOSI, -1); 

  // เชื่อมต่อ SD Card
  if (!SD.begin(SD_CS, fspi)) {
    Serial.println("Card Mount Failed");
  } else {
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
      Serial.println("No SD card attached");
    } else {
      Serial.print("SD Card Type: ");
      if (cardType == CARD_MMC) {
        Serial.println("MMC");
      } else if (cardType == CARD_SD) {
        Serial.println("SDSC");
      } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC");
      } else {
        Serial.println("UNKNOWN");
      }
      
      cardSize = SD.cardSize() / (1024 * 1024);
      Serial.printf("SD Card Size: %lluMB\n", cardSize);
    }
  }
}

void loop() {
  // 1. อ่านค่า Touch
  // ถ้ามีการแตะ get_touch_coords จะคืนค่า true
  if (get_touch_coords(&toucht_coords))
  {
    // แสดงพิกัด X, Y ทาง Serial Monitor
    Serial.print("Touch Detected -> X: ");
    Serial.print(toucht_coords.x);
    Serial.print(" | Y: ");
    Serial.println(toucht_coords.y);
  }

  // 2. แสดงสถานะ SD Card วนลูป (เพื่อเช็คว่าโค้ดยังทำงานอยู่)
  if (sd_count++ > 200) {
    sd_count = 0;
    // แสดงขนาดความจุเพื่อยืนยันว่า SD ยังเชื่อมต่ออยู่
    if (cardSize > 0) {
        Serial.printf("[Keep Alive] SD Card Size: %lluMB\n", cardSize);
    } else {
        Serial.println("[Keep Alive] No SD Card");
    }
  }
  
  delay(10);
}