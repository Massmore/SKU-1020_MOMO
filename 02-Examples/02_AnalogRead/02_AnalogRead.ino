// ============================================================
//  MOMO ESP32-S3 — 02 Analog / Digital Input (IO1–IO11)
//  Reads all 11 ADC/GPIO channels and prints volts.
// ============================================================
const int analogPins[] = {1,2,4,5,6,7,8,9,10,16,17}; // IO1..IO11 -> GPIO
const int numPins = 11;
#define BUZZER 45
#define SW_PIN 0

void setup() {
  Serial.begin(115200); delay(1000);
  Serial.println("=== MOMO ESP32-S3 Analog Test ===");
  pinMode(BUZZER, OUTPUT); pinMode(SW_PIN, INPUT_PULLUP);
  digitalWrite(BUZZER, HIGH); delay(100); digitalWrite(BUZZER, LOW);
}

void loop() {
  Serial.println("--- Analog Read ---");
  for (int i = 0; i < numPins; i++) {
    int raw = analogRead(analogPins[i]);            // 0..4095 (12-bit)
    float volt = raw * 3.3 / 4095.0;
    Serial.printf("IO%-2d (GPIO%-2d): %4d = %.2f V\n", i+1, analogPins[i], raw, volt);
  }
  Serial.printf("Button: %s\n\n", digitalRead(SW_PIN)==LOW ? "PRESSED" : "Released");
  delay(500);
}
