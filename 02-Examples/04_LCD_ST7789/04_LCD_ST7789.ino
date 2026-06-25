// ============================================================
//  MOMO ESP32-S3 — 04 LCD ST7789 240x240 (TFT_eSPI)
//  Use the TFT_eSPI library configured for MOMO:
//  ST7789, MOSI 11, SCLK 12, CS 17, DC 3, RST 18, MISO 13.
//  (User_Setup.h is provided in /libraries of this repo.)
// ============================================================
#include <TFT_eSPI.h>
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.fillRect(0, 0, 240, 50, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE); tft.setTextSize(2);
  tft.setCursor(20, 15); tft.println("MOMO ESP32-S3");
  tft.setTextColor(TFT_GREEN, TFT_BLACK); tft.setTextSize(1);
  tft.setCursor(10, 65); tft.println("by Massmore");
  tft.setCursor(10, 85); tft.println("Robot Dev Board");
  tft.fillCircle(120, 160, 40, TFT_RED);
  tft.drawCircle(120, 160, 45, TFT_WHITE);
  tft.fillRect(80, 210, 80, 20, TFT_YELLOW);
}
void loop() {
  tft.setTextColor(TFT_CYAN, TFT_BLACK); tft.setTextSize(1);
  tft.setCursor(10, 105); tft.printf("Uptime: %lu s ", millis()/1000);
  delay(1000);
}
