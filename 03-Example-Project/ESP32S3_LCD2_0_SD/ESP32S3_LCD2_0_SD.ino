#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI(); 

#define SD_CS 0
#define TFT_CS 17

// ตัวแปรเก็บสถานะ SD Card เพื่อนำไปโชว์ในหน้า UI โดยไม่ต้องอ่านซ้ำ
bool isSdReady = false; 

// ตัวแปรสำหรับทำกราฟิกเคลื่อนไหว
int angle = 0;
int radius = 80;
int centerX = 160; 
int centerY = 120; 

void setup() {
  Serial.begin(115200);
  delay(1000); 

  Serial.println("\n--- Massmore Board Booting ---");

  // 1. จัดการสถานะ CS ของทุกอุปกรณ์บนบัส SPI ให้เป็น HIGH (ปิด) ก่อนเสมอ
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);

  // 2. เริ่มต้นบัส SPI หลักเพียงครั้งเดียว (SCLK, MISO, MOSI, CS=-1 คือไม่ผูกมัด CS)
  SPI.begin(12, 13, 11, -1); 

  // 3. เริ่มต้น SD Card ก่อนจอเสมอ!
  Serial.println("Init SD Card...");
  if (!SD.begin(SD_CS, SPI, 4000000)) {
    Serial.println("SD Card Mount: FAILED");
    isSdReady = false;
  } else {
    Serial.println("SD Card Mount: SUCCESS");
    isSdReady = true;
  }

  // 4. เริ่มต้นจอ TFT เป็นลำดับถัดมา
  Serial.println("Init TFT...");
  tft.init();
  tft.setRotation(1); // แนวนอน (320x240)
  tft.fillScreen(TFT_BLACK);

  // 5. แสดง UI หน้าที่ 1
  showBootScreen();
  delay(3000); // ค้างหน้าตรวจสอบระบบไว้ 3 วินาที

  // 6. เคลียร์จอเพื่อเตรียมเข้าสู่ UI หน้าที่ 2
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  // วนลูปแสดง UI หน้าที่ 2 : กราฟิกเรดาร์
  showGraphicUI();
}

// ==========================================
// ฟังก์ชันหน้า UI ที่ 1 : ตรวจสอบระบบ
// ==========================================
void showBootScreen() {
  // วาดกรอบสไตล์ Tech
  tft.drawRect(10, 10, 300, 220, TFT_CYAN);
  tft.drawRect(12, 12, 296, 216, TFT_CYAN);
  
  tft.setTextColor(TFT_YELLOW);
  tft.setTextDatum(MC_DATUM); 
  tft.drawString("MASSMORE", 160, 80, 4); 
  
  tft.setTextColor(TFT_WHITE);
  tft.drawString("TFT ST7789 & SD Module", 160, 120, 2);

  // นำตัวแปร isSdReady ที่เช็คมาแล้วตั้งแต่ต้นมาแสดงผล
  if (!isSdReady) {
    tft.setTextColor(TFT_RED);
    tft.drawString("SD Card: FAILED OR NOT FOUND", 160, 170, 2);
  } else {
    tft.setTextColor(TFT_GREEN);
    tft.drawString("SD Card: OK! READY", 160, 170, 2);
  }
}

// ==========================================
// ฟังก์ชันหน้า UI ที่ 2 : แสดงลูกเล่นกราฟิก
// ==========================================
void showGraphicUI() {
  // Header
  tft.setTextColor(TFT_ORANGE, TFT_BLACK); 
  tft.setTextDatum(TC_DATUM); 
  tft.drawString("by MASSMORE", 160, 15, 4);

  // วาดวงกลมกรอบนอก
  tft.drawCircle(centerX, centerY + 20, radius, TFT_DARKGREY);
  tft.drawCircle(centerX, centerY + 20, radius - 1, TFT_CYAN);
  tft.drawCircle(centerX, centerY + 20, radius / 2, TFT_DARKGREY);
  
  // คำนวณพิกัด
  float rad = angle * 0.0174532925; 
  int x = centerX + (cos(rad) * (radius - 2));
  int y = (centerY + 20) + (sin(rad) * (radius - 2));

  // วาดเส้นเรดาร์
  tft.drawLine(centerX, centerY + 20, x, y, TFT_GREEN);

  // เอฟเฟกต์จุดรอบนอก
  int dotX = centerX + (cos(-rad * 2) * (radius + 15));
  int dotY = (centerY + 20) + (sin(-rad * 2) * (radius + 15));
  tft.fillCircle(dotX, dotY, 3, TFT_MAGENTA);

  delay(15);

  // ลบเส้นและจุดเดิมด้วยสีดำเพื่อวาดเฟรมต่อไป
  tft.drawLine(centerX, centerY + 20, x, y, TFT_BLACK);
  tft.fillCircle(dotX, dotY, 3, TFT_BLACK);

  // เพิ่มองศา
  angle += 3;
  if (angle >= 360) angle = 0;
  
  // Progress Bar ด้านล่าง
  int barWidth = map(sin(rad*2)*100, -100, 100, 10, 280);
  tft.fillRect(20, 220, 280, 10, TFT_BLACK); 
  tft.fillRect(20, 220, barWidth, 10, TFT_BLUE);
}