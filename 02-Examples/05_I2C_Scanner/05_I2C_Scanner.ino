// ============================================================
//  MOMO ESP32-S3 — 05 I2C Scanner  (SDA=14, SCL=15)
//  Should report 0x40 (PCA9685 servo driver).
// ============================================================
#include <Wire.h>
void setup() {
  Serial.begin(115200); delay(1000);
  Wire.begin(14, 15);
  Serial.println("=== MOMO I2C Scanner ===");
}
void loop() {
  byte count = 0;
  Serial.println("Scanning...");
  for (byte a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) { Serial.printf("  Found 0x%02X\n", a); count++; }
  }
  Serial.println(count ? "" : "  No I2C devices found!");
  Serial.printf("  Total: %d device(s)\n\n", count);
  delay(3000);
}
