# 02 · Arduino IDE Examples (ESP32-S3)

Beginner-friendly sketches — one for each on-board feature of the MOMO board.
Open any folder, **Upload**, and watch the Serial Monitor @ **115200**.

> Board settings: **ESP32S3 Dev Module · PSRAM = OPI · Flash = 16 MB** (see [01-Arduino-IDE-Setup](../01-Arduino-IDE-Setup/)).

| # | Sketch | What it shows | Key pins / IC |
|---|---|---|---|
| 00 | [`00_Factory_Test`](00_Factory_Test/) | Full factory self-test (LCD, buzzer, motors, servo, I/O) | all |
| 01 | [`01_HelloWorld`](01_HelloWorld/) | Serial print + chip info, confirms PSRAM/Flash | — |
| 02 | [`02_AnalogRead`](02_AnalogRead/) | Read analog values from the ADC/GPIO header | IO1–IO11 |
| 03 | [`03_Buzzer_Button`](03_Buzzer_Button/) | Beep the buzzer when the user button is pressed | Buzzer 45 · SW 0 |
| 04 | [`04_LCD_ST7789`](04_LCD_ST7789/) | Draw text/graphics on the 1.3" IPS LCD | ST7789 (TFT_eSPI) |
| 05 | [`05_I2C_Scanner`](05_I2C_Scanner/) | Scan the I²C bus, find PCA9685 @ 0x40 | SDA 14 · SCL 15 |
| 06 | [`06_Servo_PCA9685`](06_Servo_PCA9685/) | Sweep servos via the 16-ch PCA9685 driver | PCA9685 0x40 |
| 07 | [`07_DC_Motor`](07_DC_Motor/) | Drive a DC motor fwd/rev with the TB67H451 | EN/PH 21/38 |
| 08 | [`08_WiFi_Scan_Connect`](08_WiFi_Scan_Connect/) | Scan Wi-Fi networks and connect | ESP32-S3 Wi-Fi |
| 09 | [`09_BLE`](09_BLE/) | Advertise a BLE service, echo characteristic | ESP32-S3 BLE 5 |
| 10 | [`10_AllInOne_Demo`](10_AllInOne_Demo/) | LCD + buzzer + button + servo + motor together | multiple |
| 11 | [`11_Robot_Car_4WD`](11_Robot_Car_4WD/) | 4-wheel-drive robot car motion demo | M1–M4 |

---

## Required libraries

Install via **Sketch → Include Library → Manage Libraries**:

| Library | Used by | Author |
|---|---|---|
| **TFT_eSPI** | 04, 10 | Bodmer |
| **Adafruit PWM Servo Driver** | 06, 10 | Adafruit |

### TFT_eSPI configuration (LCD examples)

After installing **TFT_eSPI**, edit its `User_Setup.h` to match MOMO's ST7789 wiring:

```cpp
#define ST7789_DRIVER
#define TFT_WIDTH   240
#define TFT_HEIGHT  240
#define TFT_MOSI    11
#define TFT_SCLK    12
#define TFT_CS      17
#define TFT_DC       3
#define TFT_RST     18
#define SPI_FREQUENCY  40000000
```
