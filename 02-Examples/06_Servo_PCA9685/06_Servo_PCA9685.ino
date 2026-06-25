// ============================================================
//  MOMO ESP32-S3 — 06 Servo via PCA9685  (I2C 0x40, SDA=14 SCL=15)
//  Library: Adafruit PWM Servo Driver
// ============================================================
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
#define SERVOMIN 150   // 0 deg
#define SERVOMAX 600   // 180 deg
#define SERVO_FREQ 50
int angleToPulse(int a){ return map(a, 0, 180, SERVOMIN, SERVOMAX); }

void setup() {
  Serial.begin(115200);
  Wire.begin(14, 15);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);
  Serial.println("PCA9685 Ready!");
}
void loop() {
  for (int a = 0; a <= 180; a += 5){ pwm.setPWM(0, 0, angleToPulse(a)); delay(20); } // Servo1 = CH0
  delay(500);
  for (int a = 180; a >= 0; a -= 5){ pwm.setPWM(0, 0, angleToPulse(a)); delay(20); }
  delay(500);
}
