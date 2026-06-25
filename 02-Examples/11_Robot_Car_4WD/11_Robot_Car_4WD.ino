// ============================================================
//  MOMO ESP32-S3 — 11 Robot Car 4WD + Gripper
//  Left wheels = M1,M3   Right wheels = M2,M4   Gripper = PCA9685 CH0
//  *** Power motors/servo from the XT30 battery, not USB only ***
// ============================================================
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
const int EN[4] = {21,39,41,47};   // M1..M4 speed
const int PH[4] = {38,40,42,48};   // M1..M4 dir
#define PWM_FREQ 1000
#define PWM_RES  8
void motorSetup(){ for(int i=0;i<4;i++){ pinMode(PH[i],OUTPUT); ledcAttach(EN[i],PWM_FREQ,PWM_RES);} }
void motor(int i,int spd){ spd=constrain(spd,-255,255); digitalWrite(PH[i], spd>=0?HIGH:LOW); ledcWrite(EN[i], abs(spd)); }
void drive(int l,int r){ motor(0,l); motor(2,l); motor(1,r); motor(3,r); }    // tank style
void gripperOpen(){  pwm.setPWM(0,0,600); }   // CH0
void gripperClose(){ pwm.setPWM(0,0,150); }
void setup(){
  Serial.begin(115200); motorSetup();
  Wire.begin(14,15); pwm.begin(); pwm.setPWMFreq(50);
  Serial.println("Robot Ready!");
}
void loop(){
  drive(200,200);  delay(1500);   // forward
  drive(-200,-200);delay(1000);   // reverse
  drive(200,-200); delay(700);    // spin right
  drive(0,0);      delay(500);    // stop
  gripperClose();  delay(800);
  gripperOpen();   delay(800);
}
