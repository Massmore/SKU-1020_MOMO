// ============================================================
//  MOMO ESP32-S3 — 01 Hello World (first sketch / board check)
//  Board: "ESP32S3 Dev Module" · PSRAM: OPI · Flash: 16MB
//  by Massmore · github.com/Massmore/SKU-1020_MOMO
// ============================================================
#define BUZZER 45     // Active buzzer
#define SW_PIN 0      // User button (active LOW)
int counter = 0;

void setup() {
  Serial.begin(115200);
  delay(1500);                       // wait for USB CDC
  pinMode(BUZZER, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
  Serial.println("========================================");
  Serial.println(" MOMO ESP32-S3 by Massmore");
  Serial.println(" Hello World! - Board is Ready!");
  Serial.println("========================================");
  Serial.printf(" CPU Freq : %d MHz\n", getCpuFrequencyMhz());
  Serial.printf(" PSRAM    : %d bytes\n", ESP.getPsramSize());   // ~8388608 = OPI OK
  Serial.printf(" Flash    : %d bytes\n", ESP.getFlashChipSize());
  Serial.printf(" Free RAM : %d bytes\n", ESP.getFreeHeap());
  Serial.println("========================================");
  for (int i = 0; i < 2; i++) { digitalWrite(BUZZER, HIGH); delay(150); digitalWrite(BUZZER, LOW); delay(100); }
}

void loop() {
  counter++;
  Serial.printf("[%4d] Uptime: %lu s | Button: %s\n",
                counter, millis()/1000, digitalRead(SW_PIN)==LOW ? "PRESSED" : "Released");
  if (digitalRead(SW_PIN) == LOW) { digitalWrite(BUZZER, HIGH); delay(80); digitalWrite(BUZZER, LOW); }
  delay(500);
}
