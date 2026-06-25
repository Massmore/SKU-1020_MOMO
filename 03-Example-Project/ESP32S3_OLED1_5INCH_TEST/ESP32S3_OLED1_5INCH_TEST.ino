#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <math.h> // เรียกใช้ไลบรารีคณิตศาสตร์สำหรับฟังก์ชัน sin()

#define I2C_ADDRESS 0x3C 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define OLED_RESET -1

Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// กำหนดขา I2C สำหรับ ESP32-S3
#define SDA_PIN 14 
#define SCL_PIN 15

// --- ส่วนกำหนดค่าสำหรับ Sine Wave ---
float phase = 0.0;            // จุดเริ่มต้นของคลื่น (ใช้สำหรับทำให้คลื่นขยับ)
float phaseSpeed = 0.15;      // ความเร็วในการวิ่งของคลื่น (ค่ายิ่งมาก คลื่นยิ่งวิ่งเร็ว)
float amplitude = 28.0;       // ความสูงของยอดคลื่น (Amplitude)
float frequency = 0.08;       // ความถี่ของคลื่น (จำนวนลูกคลื่นในหน้าจอ)
int waveCenterY = 45;         // ตำแหน่งแกน Y ตรงกลางของคลื่น

void setup() {
  Serial.begin(115200);
  delay(250);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(I2C_ADDRESS, true)) {
    Serial.println(F("SH1107 allocation failed"));
    while (true);
  }
}

void loop() {
  display.clearDisplay(); 

  // 1. วาดกรอบหน้าจอมุมโค้ง
  display.drawRoundRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 8, SH110X_WHITE);

  // 2. วาดเส้นอ้างอิงตรงกลาง (เส้นประคล้ายจอ Oscilloscope)
  for (int x = 4; x < SCREEN_WIDTH - 4; x += 4) {
    display.drawPixel(x, waveCenterY, SH110X_WHITE);
  }

  // 3. คำนวณและวาดเส้นคลื่น Sine Wave
  int prevX = 4; // เริ่มวาดถัดจากขอบซ้ายเข้ามาเล็กน้อย
  // คำนวณจุด Y เริ่มต้น
  int prevY = waveCenterY + (sin(prevX * frequency + phase) * amplitude);

  for (int x = 5; x < SCREEN_WIDTH - 4; x++) {
    // คำนวณตำแหน่ง Y ของจุดปัจจุบันตามสมการ y = A * sin(f*x + phase)
    int y = waveCenterY + (sin(x * frequency + phase) * amplitude);
    
    // ลากเส้นเชื่อมจากจุดก่อนหน้ามายังจุดปัจจุบัน (ทำให้เส้นดูต่อเนื่อง)
    display.drawLine(prevX, prevY, x, y, SH110X_WHITE);
    
    prevX = x;
    prevY = y;
  }

  // อัปเดตค่า phase เพื่อให้คลื่นขยับในเฟรมถัดไป
  phase += phaseSpeed;
  // รีเซ็ตค่า phase เพื่อป้องกันไม่ให้ตัวแปรมีค่ามากเกินไป (2 * PI = 1 รอบคลื่น)
  if (phase >= 2 * PI) {
    phase -= 2 * PI;
  }

  // 4. วาดข้อความ by "MASSMORE" ให้คงที่ด้านล่าง
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(55, 90); 
  display.print("by");

  display.setTextSize(2);
  // คำนวณให้ MASSMORE อยู่กึ่งกลาง
  int textX = (SCREEN_WIDTH - 96) / 2; 
  display.setCursor(textX, 105);
  display.print("MASSMORE");

  // 5. สั่งอัปเดตหน้าจอ
  display.display();

  // ควบคุมความสมูทของการเคลื่อนไหว
  delay(15); 
}