 #include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ==========================================
// การตั้งค่า Debug (1 = เปิด Debug, 0 = ปิด Debug)
#define DEBUG_MODE 1

#if DEBUG_MODE
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif
// ==========================================

// กำหนดขา I2C สำหรับ ESP32-S3
#define SDA_PIN 14
#define SCL_PIN 15

// กำหนดขาปุ่มกด
#define BUTTON_PIN 0

// เรียกใช้ไลบรารี PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// ตั้งค่า Pulse ของ Servo
#define SERVOMIN  150 
#define SERVOMAX  600 
#define SERVO_CH  0   

// สถานะการทำงานของโปรแกรม
enum State { SET_CLOSE, SET_OPEN, RUNNING };
State currentState = SET_CLOSE;

int currentAngle = 0; 
int closeAngle = 0;   
int openAngle = 0;    

// ตัวแปรสำหรับการเช็คปุ่มกด
unsigned long buttonPressTime = 0;
bool isButtonPressed = false;
bool longPressHandled = false;

// ตัวแปรสำหรับการทำงานวนลูป 5 วินาที
unsigned long lastRunTime = 0;
bool isGripperClosed = true;

void setup() {
  Serial.begin(115200);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  Wire.begin(SDA_PIN, SCL_PIN);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);  
  delay(10);
  
  DEBUG_PRINTLN("\n[DEBUG] System Started!");
  DEBUG_PRINT("[DEBUG] SDA: "); DEBUG_PRINT(SDA_PIN);
  DEBUG_PRINT(" | SCL: "); DEBUG_PRINTLN(SCL_PIN);
  
  setServoAngle(currentAngle);
  
  Serial.println("--- เข้าสู่โหมดตั้งค่า องศาหุบ (Close) ---");
  Serial.println("- กดสั้นๆ เพื่อปรับองศาทีละ 5 องศา");
  Serial.println("- กดค้าง 5 วินาที เพื่อบันทึกค่า");
}

void loop() {
  bool currentBtnState = (digitalRead(BUTTON_PIN) == LOW);

  // ตรวจจับตอนเริ่มกดปุ่ม
  if (currentBtnState && !isButtonPressed) {
    isButtonPressed = true;
    buttonPressTime = millis();
    longPressHandled = false;
    DEBUG_PRINTLN("[DEBUG] Button PRESSED");
  }

  // ขณะที่ปุ่มกำลังถูกกดแช่ไว้
  if (currentBtnState && isButtonPressed) {
    if (!longPressHandled && (millis() - buttonPressTime >= 5000)) {
      DEBUG_PRINTLN("[DEBUG] Triggering LONG Press (>= 5000ms)");
      handleLongPress();
      longPressHandled = true; 
    }
  }

  // ตรวจจับตอนปล่อยปุ่ม
  if (!currentBtnState && isButtonPressed) {
    isButtonPressed = false;
    unsigned long holdDuration = millis() - buttonPressTime;
    
    DEBUG_PRINT("[DEBUG] Button RELEASED. Held for: ");
    DEBUG_PRINT(holdDuration);
    DEBUG_PRINTLN(" ms");

    // ถ้าปล่อยปุ่มก่อน 5 วินาที และไม่ใช่การเด้งของปุ่ม (Debounce > 50ms)
    if (!longPressHandled && holdDuration > 50) {
      DEBUG_PRINTLN("[DEBUG] Triggering SHORT Press");
      handleShortPress();
    }
  }

  // --- โหมดทำงานจริง (RUNNING) ---
  if (currentState == RUNNING) {
    if (millis() - lastRunTime >= 5000) {
      lastRunTime = millis(); 
      
      DEBUG_PRINTLN("[DEBUG] 5-second interval triggered");
      
      if (isGripperClosed) {
        setServoAngle(openAngle);
        isGripperClosed = false;
        Serial.println("สถานะ: อ้า (OPEN)");
      } else {
        setServoAngle(closeAngle);
        isGripperClosed = true;
        Serial.println("สถานะ: หุบ (CLOSE)");
      }
    }
  }
}

// ฟังก์ชันปรับองศา Servo
void setServoAngle(int angle) {
  int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  
  DEBUG_PRINT("[DEBUG] Setting Servo CH "); DEBUG_PRINT(SERVO_CH);
  DEBUG_PRINT(" to Angle: "); DEBUG_PRINT(angle);
  DEBUG_PRINT(" (Pulse: "); DEBUG_PRINT(pulse); DEBUG_PRINTLN(")");
  
  pwm.setPWM(SERVO_CH, 0, pulse);
}

// ฟังก์ชันจัดการเมื่อกดปุ่มสั้นๆ (ปรับองศา)
void handleShortPress() {
  if (currentState == SET_CLOSE || currentState == SET_OPEN) {
    currentAngle += 5; 
    if (currentAngle > 180) {
      currentAngle = 0; 
    }
    setServoAngle(currentAngle);
    
    Serial.print("ปรับองศาปัจจุบันเป็น: ");
    Serial.print(currentAngle);
    Serial.println(" องศา");
  }
}

// ฟังก์ชันจัดการเมื่อกดปุ่มค้าง 5 วินาที
void handleLongPress() {
  if (currentState == SET_CLOSE) {
    closeAngle = currentAngle;
    currentState = SET_OPEN;
    
    DEBUG_PRINTLN("[DEBUG] State changed: SET_CLOSE -> SET_OPEN");
    Serial.print(">> บันทึก 'องศาหุบ' ที่: ");
    Serial.print(closeAngle);
    Serial.println(" องศา เรียบร้อยแล้ว <<");
    Serial.println("\n--- เข้าสู่โหมดตั้งค่า องศาอ้า (Open) ---");
    
  } 
  else if (currentState == SET_OPEN) {
    openAngle = currentAngle;
    currentState = RUNNING;
    lastRunTime = millis(); 
    
    DEBUG_PRINTLN("[DEBUG] State changed: SET_OPEN -> RUNNING");
    Serial.print(">> บันทึก 'องศาอ้า' ที่: ");
    Serial.print(openAngle);
    Serial.println(" องศา เรียบร้อยแล้ว <<");
    Serial.println("\n--- เริ่มต้นการทำงาน (หุบ-อ้า สลับกันทุก 5 วินาที) ---");
    
    setServoAngle(closeAngle);
    isGripperClosed = true;
  }
}