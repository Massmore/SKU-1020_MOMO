#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include <TFT_eSPI.h>

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 240

// ช่วงอุณหภูมิสำหรับการแสดงสี
#define MINTEMP 22
#define MAXTEMP 32

Adafruit_AMG88xx amg;
TFT_eSPI tft = TFT_eSPI();

const int matrix_size = 8;
float pixels[matrix_size * matrix_size];

void setup() {
    Serial.begin(115200);
    Wire.begin(14, 15); // ขา SDA, SCL สำหรับ ESP32
    
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);

    if (!amg.begin(0x69)) {
        Serial.println("AMG88xx not found. Check wiring!");
        while (1);
    }
    delay(100);
}

void loop() {
    amg.readPixels(pixels);
    drawHeatMap();
    displayCenterTemp(); // ฟังก์ชันแสดงตัวเลขที่เราเพิ่มเข้าไป
    delay(50); // ลด Delay ลงเล็กน้อยเพื่อให้การแสดงผลลื่นไหลขึ้น
}

void drawHeatMap() {
    float minVal = MINTEMP, maxVal = MAXTEMP;
    float range = maxVal - minVal;
    int rectSize = SCREEN_WIDTH / matrix_size;

    for (int y = 0; y < matrix_size; y++) {
        for (int x = 0; x < matrix_size; x++) {
            int index = y * matrix_size + x;
            float normalized = (pixels[index] - minVal) / range;
            normalized = constrain(normalized, 0.0, 1.0);
            uint16_t color = getColorFromValue(normalized);
            
            // วาดสี่เหลี่ยมแต่ละพิกเซล
            tft.fillRect(x * rectSize, y * rectSize, rectSize, rectSize, color);
        }
    }
}

// ฟังก์ชันใหม่สำหรับคำนวณและแสดงผลตัวเลขกลางหน้าจอ
void displayCenterTemp() {
    // คำนวณหาค่าเฉลี่ยจุดกึ่งกลาง (จากพิกเซลตำแหน่งที่ 27, 28, 35, 36 ใน Array 8x8)
    // หรือเอาง่ายๆ คือจุดที่ [3][3] ในระบบพิกัด 0-7
    float centerTemp = pixels[27]; 

    // วาดจุด Mark เล็กๆ ตรงกลาง (Optional)
    tft.drawRect((SCREEN_WIDTH/2)-2, (SCREEN_HEIGHT/2)-2, 4, 4, TFT_WHITE);

    // ตั้งค่าการแสดงผลตัวเลข
    tft.setTextColor(TFT_WHITE); // สีตัวอักษร
    tft.setTextSize(2);          // ขนาดตัวอักษร
    
    // วาดพื้นหลังตัวเลขเล็กน้อยเพื่อให้มองเห็นได้ชัดในทุกสีพื้นหลัง
    tft.setCursor((SCREEN_WIDTH / 2) - 20, (SCREEN_HEIGHT / 2) + 10);
    tft.print(centerTemp, 1);    // แสดงทศนิยม 1 ตำแหน่ง
    tft.print(" C");
}

uint16_t getColorFromValue(float value) {
    uint8_t r, g, b;
    value = constrain(value, 0.0, 1.0);
    if (value < 0.25) {
        r = 0; g = value * 4 * 255; b = 255;
    } else if (value < 0.5) {
        r = 0; g = 255; b = (1.0 - (value - 0.25) * 4) * 255;
    } else if (value < 0.75) {
        r = (value - 0.5) * 4 * 255; g = 255; b = 0;
    } else {
        r = 255; g = (1.0 - (value - 0.75) * 4) * 255; b = 0;
    }
    return tft.color565(r, g, b);
}