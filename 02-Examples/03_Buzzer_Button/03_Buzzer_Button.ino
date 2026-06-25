// ============================================================
//  MOMO ESP32-S3 — 03 Buzzer & Button
//  BUZZER = GPIO45 (active HIGH), SW_PIN = GPIO0 (active LOW)
// ============================================================
#define BUZZER 45
#define SW_PIN 0
void beep(int ms){ digitalWrite(BUZZER,HIGH); delay(ms); digitalWrite(BUZZER,LOW); }

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT); pinMode(SW_PIN, INPUT_PULLUP);
  Serial.println("Press the button to beep!");
  beep(200);
}
void loop() {
  if (digitalRead(SW_PIN) == LOW) {
    Serial.println("Button Pressed!");
    beep(100); delay(50); beep(100); delay(50); beep(300);
    while (digitalRead(SW_PIN) == LOW);   // wait release
  }
}
