// ============================================================
//  MOMO ESP32-S3 — 10 All-in-One Demo (LCD + Motor + Buzzer)
//  Press the user button to drive M1/M2 forward for 1.5s.
//  Requires TFT_eSPI (configured for MOMO) + Adafruit PWM lib.
// ============================================================
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
TFT_eSPI tft = TFT_eSPI();
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
#define BUZZER 45
#define SW_PIN 0
#define M1_EN 21
#define M1_PH 38
#define M2_EN 39
#define M2_PH 40
void beep(int ms){ digitalWrite(BUZZER,HIGH); delay(ms); digitalWrite(BUZZER,LOW); }
void drawStatus(String m, uint16_t c){
  tft.fillRect(0,200,240,40,TFT_BLACK); tft.setTextColor(c,TFT_BLACK);
  tft.setTextSize(1); tft.setCursor(10,210); tft.println(m);
}
void setup(){
  Serial.begin(115200);
  tft.init(); tft.setRotation(0); tft.fillScreen(TFT_NAVY);
  tft.setTextColor(TFT_WHITE,TFT_NAVY); tft.setTextSize(2);
  tft.setCursor(20,10); tft.println("MOMO S3");
  tft.setTextSize(1); tft.setCursor(10,40); tft.println("by Massmore");
  Wire.begin(14,15); pwm.begin(); pwm.setPWMFreq(50);
  pinMode(M1_PH,OUTPUT); pinMode(M2_PH,OUTPUT);
  ledcAttach(M1_EN,1000,8); ledcAttach(M2_EN,1000,8);
  pinMode(BUZZER,OUTPUT); pinMode(SW_PIN,INPUT_PULLUP);
  beep(300); drawStatus("Ready!",TFT_GREEN);
}
void loop(){
  if (digitalRead(SW_PIN)==LOW){
    drawStatus("Motor Forward!",TFT_YELLOW);
    digitalWrite(M1_PH,HIGH); ledcWrite(M1_EN,200);
    digitalWrite(M2_PH,HIGH); ledcWrite(M2_EN,200);
    beep(100); delay(1500);
    ledcWrite(M1_EN,0); ledcWrite(M2_EN,0);
    drawStatus("Stopped.",TFT_WHITE);
    while(digitalRead(SW_PIN)==LOW);
  }
  tft.setTextColor(TFT_CYAN,TFT_NAVY); tft.setCursor(10,60);
  tft.printf("Uptime: %lu s ", millis()/1000);
  delay(500);
}
