// ============================================================
//  MOMO ESP32-S3 — 07 DC Motor x4 (TB67H451AFNG, EN/PH mode)
//  EN = speed (PWM), PH = direction.  *** Power from XT30 battery ***
//  Arduino-ESP32 core 3.x: ledcAttach(pin, freq, res)
// ============================================================
#define M1_EN 21
#define M1_PH 38
#define M2_EN 39
#define M2_PH 40
#define M3_EN 41
#define M3_PH 42
#define M4_EN 47
#define M4_PH 48
#define PWM_FREQ 1000
#define PWM_RES  8     // 0..255

void motorSetup() {
  int ph[4] = {M1_PH,M2_PH,M3_PH,M4_PH};
  int en[4] = {M1_EN,M2_EN,M3_EN,M4_EN};
  for (int i=0;i<4;i++){ pinMode(ph[i],OUTPUT); ledcAttach(en[i],PWM_FREQ,PWM_RES); }
}
// motor 1..4, speed 0..255, dir 1=fwd / 0=rev
void motorControl(int motor,int speed,int dir){
  speed = constrain(speed,0,255);
  int en,ph;
  switch(motor){ case 1:en=M1_EN;ph=M1_PH;break; case 2:en=M2_EN;ph=M2_PH;break;
                 case 3:en=M3_EN;ph=M3_PH;break; case 4:en=M4_EN;ph=M4_PH;break; default:return; }
  digitalWrite(ph, dir?HIGH:LOW); ledcWrite(en, speed);
}
void allStop(){ for(int i=1;i<=4;i++) motorControl(i,0,0); }

void setup(){ Serial.begin(115200); motorSetup(); Serial.println("Motor Ready!"); }
void loop(){
  Serial.println("Forward All");  for(int i=1;i<=4;i++) motorControl(i,180,1); delay(2000);
  Serial.println("Reverse All");  for(int i=1;i<=4;i++) motorControl(i,180,0); delay(2000);
  Serial.println("Stop");         allStop();                                    delay(1000);
}
