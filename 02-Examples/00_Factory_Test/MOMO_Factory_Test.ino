#include "SPI.h"
#include "Wire.h"
#include "TFT_eSPI.h"
#include <Adafruit_PWMServoDriver.h>

// ==========================================
// CONFIG & PINS
// ==========================================
const int analogPins[] = {1, 2, 4, 5, 6, 7, 8, 9, 10, 16}; // เพิ่ม 16 แล้ว
const int numAnalogs = 10;

#define BUZZER 45
#define SW_PIN 0
#define I2C_SDA 14
#define I2C_SCL 15

// Motor TB67H450FNG
#define M1_EN 21
#define M1_PH 38
#define M2_EN 39
#define M2_PH 40
#define M3_EN 41
#define M3_PH 42
#define M4_EN 47
#define M4_PH 48

// ==========================================
// OBJECTS & GLOBALS
// ==========================================
TFT_eSPI tft = TFT_eSPI(); 
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

int currentPage = 0; 
bool lastSwState = HIGH;
unsigned long lastDebounceTime = 0;
bool pageInited = false; // สำหรับวาด Static UI ครั้งเดียว
int oldEndX, oldEndY;    // สำหรับลบเส้นเข็มหน้า Servo

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);
  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  
  // Motor Pins
  pinMode(M1_EN, OUTPUT); pinMode(M1_PH, OUTPUT);
  pinMode(M2_EN, OUTPUT); pinMode(M2_PH, OUTPUT);
  pinMode(M3_EN, OUTPUT); pinMode(M3_PH, OUTPUT);
  pinMode(M4_EN, OUTPUT); pinMode(M4_PH, OUTPUT);
  //analogWriteFrequency(5000); 

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);
  pwm.begin();
  pwm.setPWMFreq(50);

  tft.begin();
  tft.setRotation(0); 
  tft.fillScreen(TFT_BLACK);
  
  // Welcome
  tft.setTextColor(TFT_GOLD);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("MOMO ESP32-S3", 120, 110, 4);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("V2.0 PRO FIRMWARE", 120, 140, 2);
  beep(100); delay(100); beep(100);
  delay(1500);
  tft.fillScreen(TFT_BLACK);
}

// ==========================================
// MAIN LOOP
// ==========================================
void loop() {
  handleButton();

  switch(currentPage) {
    case 0: displayAnalogPage(); break;
    case 1: displayMotorPage(); break;
    case 2: displayServoPage(); break;
    case 3: displayI2CPage(); break;
  }
}

// ==========================================
// FUNCTIONS
// ==========================================
void beep(int duration) {
  digitalWrite(BUZZER, HIGH);
  delay(duration);
  digitalWrite(BUZZER, LOW);
}

void handleButton() {
  bool swState = digitalRead(SW_PIN);
  if (swState == LOW && lastSwState == HIGH && (millis() - lastDebounceTime) > 300) {
    currentPage++;
    if (currentPage > 3) currentPage = 0;
    
    // Reset Hardware States
    analogWrite(M1_EN, 0); analogWrite(M2_EN, 0);
    analogWrite(M3_EN, 0); analogWrite(M4_EN, 0);
    
    beep(50);
    tft.fillScreen(TFT_BLACK);
    pageInited = false; // สั่งให้วาด Background หน้าใหม่
    lastDebounceTime = millis();
  }
  lastSwState = swState;
}

// --- PAGE 0: ANALOG MONITOR ---
void displayAnalogPage() {
  if(!pageInited) {
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawString("PAGE 1: IO ANALOG TEST", 120, 15, 2);
    pageInited = true;
  }
  
  tft.setTextDatum(TL_DATUM);
  for (int i = 0; i < numAnalogs; i++) {
    int val = analogRead(analogPins[i]);
    int x = (i % 2 == 0) ? 10 : 130;
    int y = 45 + (i / 2) * 35;
    
    char buf[20];
    sprintf(buf, "A%d:%04d", analogPins[i], val);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(buf, x, y, 2);
    
    int barLen = map(val, 0, 4095, 0, 45);
    tft.fillRect(x + 65, y, barLen, 10, TFT_GREEN);
    tft.fillRect(x + 65 + barLen, y, 45 - barLen, 10, TFT_DARKGREY);
  }
  delay(50);
}

// --- PAGE 1: BI-DIRECTIONAL MOTOR ---
void displayMotorPage() {
  if(!pageInited) {
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString("PAGE 2: MOTOR SWEEP", 120, 15, 2);
    pageInited = true;
  }

  unsigned long t = millis();
  // รอบละ 6 วินาที: 0-3s FWD, 3-6s REV
  long cycle = t % 6000;
  int pwmValue;
  String dirText;
  uint16_t color;

  if (cycle < 3000) { // Forward 0 -> 255 -> 0
    pwmValue = (abs((int)(cycle % 3000) - 1500) * 255) / 1500;
    pwmValue = 255 - pwmValue; // ทำให้เป็น 0->255->0
    digitalWrite(M1_PH, HIGH); digitalWrite(M2_PH, HIGH);
    digitalWrite(M3_PH, HIGH); digitalWrite(M4_PH, HIGH);
    dirText = "FORWARD";
    color = TFT_GREEN;
  } else { // Reverse 0 -> 255 -> 0
    pwmValue = (abs((int)(cycle % 3000) - 1500) * 255) / 1500;
    pwmValue = 255 - pwmValue;
    digitalWrite(M1_PH, LOW); digitalWrite(M2_PH, LOW);
    digitalWrite(M3_PH, LOW); digitalWrite(M4_PH, LOW);
    dirText = "REVERSE";
    color = TFT_RED;
  }

  analogWrite(M1_EN, pwmValue); analogWrite(M2_EN, pwmValue);
  analogWrite(M3_EN, pwmValue); analogWrite(M4_EN, pwmValue);

  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(color, TFT_BLACK);
  tft.drawString("DIR: " + dirText + "    ", 120, 50, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("SPEED: " + String(pwmValue) + "  ", 120, 85, 4);

  for(int i=0; i<4; i++) {
    int y = 120 + (i * 28);
    int barLen = map(pwmValue, 0, 255, 0, 180);
    tft.fillRect(30, y, barLen, 12, color);
    tft.fillRect(30 + barLen, y, 180 - barLen, 12, TFT_BLACK);
    tft.drawRect(30, y, 180, 12, TFT_WHITE);
  }
}

// --- PAGE 2: SERVO (NO FLICKER) ---
void displayServoPage() {
  int centerX = 120;
  int centerY = 145;
  int radius = 70;

  if(!pageInited) {
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
    tft.drawString("PAGE 3: SMOOTH SERVO", 120, 15, 2);
    // วาดหน้าปัดครั้งเดียว
    tft.drawCircle(centerX, centerY, radius + 2, TFT_WHITE);
    tft.drawCircle(centerX, centerY, radius + 3, TFT_WHITE);
    pageInited = true;
  }

  unsigned long t = millis();
  float phase = ((t % 4000) / 4000.0) * 2.0 * PI;
  int microsec = 1500 + (sin(phase) * 1000);
  
  for (int i = 0; i < 16; i++) { pwm.writeMicroseconds(i, microsec); }
  int angle = map(microsec, 500, 2500, 0, 180);

  // ลบเข็มเก่า (วาดเส้นเดิมสีดำ)
  tft.drawLine(centerX, centerY, oldEndX, oldEndY, TFT_BLACK);

  // คำนวณเข็มใหม่
  float rad = (angle - 90) * 0.0174533; 
  int endX = centerX + (cos(rad) * radius);
  int endY = centerY + (sin(rad) * radius);
  
  // วาดเข็มใหม่
  tft.drawLine(centerX, centerY, endX, endY, TFT_MAGENTA);
  tft.fillCircle(centerX, centerY, 5, TFT_WHITE); // จุดหมุนกลาง

  oldEndX = endX; oldEndY = endY; // เก็บพิกัดไว้ลบในลูปถัดไป

  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("ANGLE: " + String(angle) + " Deg  ", 120, 50, 4);
  tft.drawString("CH 0 - 15 Active", 120, 225, 2);
  
  delay(15); 
}

// --- PAGE 3: I2C SCANNER ---
void displayI2CPage() {
  if(!pageInited) {
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.drawString("PAGE 4: I2C SCANNER", 120, 15, 2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Scanning...", 120, 50, 2);
    
    int foundCount = 0;
    int yOffset = 80;
    
    tft.fillRect(0, 40, 240, 200, TFT_BLACK); // Clear scan area
    
    for(byte address = 1; address < 127; address++ ) {
      Wire.beginTransmission(address);
      byte error = Wire.endTransmission();

      if (error == 0) {
        int col = foundCount % 2;
        int row = foundCount / 2;
        tft.setTextDatum(TL_DATUM);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        String addrHex = "0x" + String(address, HEX);
        addrHex.toUpperCase();
        tft.drawString("FOUND: " + addrHex, 20 + (col * 110), yOffset + (row * 25), 2);
        foundCount++;
      }
      if(foundCount >= 12) break; // จำกัด 12 addr ตามขอ
    }

    tft.setTextDatum(MC_DATUM);
    if (foundCount == 0) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.drawString("NO DEVICES FOUND", 120, 120, 2);
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawString("TOTAL: " + String(foundCount) + " DEVICES", 120, 220, 2);
    }
    pageInited = true;
  }
  // หน้านี้ Static จนกว่าจะกดข้ามไปหน้าอื่นแล้ววนกลับมาสแกนใหม่
  delay(100);
}