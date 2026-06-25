const int LO_MINUS_PIN = 11;  // L0-
const int LO_PLUS_PIN = 12;   // L0+
const int OUTPUT_PIN = 13;    // Analog OUT

void setup() {
  Serial.begin(115200); // แนะนำใช้ 115200 สำหรับ ESP32
  pinMode(LO_MINUS_PIN, INPUT);
  pinMode(LO_PLUS_PIN, INPUT);
}

void loop() {
  // 1. ล็อคแกน Y ให้เหมือนในรูป (ขอบบน 1000, ขอบล่าง 200)
  Serial.print(800); 
  Serial.print(" ");
  Serial.print(200);  
  Serial.print(" ");

  // 2. ตรวจสอบสถานะสายหลุด
  if ((digitalRead(LO_PLUS_PIN) == 1) || (digitalRead(LO_MINUS_PIN) == 1)) {
    Serial.println(512); // ส่งค่ากึ่งกลาง (512) ไปให้กราฟเป็นเส้นตรงเมื่อไม่ได้วัด
  } else {
    // 3. อ่านค่าจาก ESP32 (0-4095)
    int ecgValue = analogRead(OUTPUT_PIN);
    
    // 4. แปลงค่า (Map) จาก 12-bit ให้เป็น 10-bit เหมือน Arduino Uno (0-1023)
    int mappedEcg = map(ecgValue, 0, 4095, 0, 1023);
    
    // พิมพ์ค่าที่แปลงแล้วออกไปวาดกราฟ
    Serial.println(mappedEcg);
  }

  // 5. หน่วงเวลา 2-3 มิลลิวินาที ให้ความกว้างกราฟ (แกน X) เรียงสวยเหมือนในรูป
  delay(10); 
}