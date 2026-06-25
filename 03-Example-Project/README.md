# 03 · Example Projects

Ready-to-run, more advanced project sketches for the MOMO ESP32-S3 board.
Each folder is a complete Arduino sketch — open it, install the listed libraries, and **Upload**.

> Board settings: **ESP32S3 Dev Module · PSRAM = OPI · Flash = 16 MB**.

| Project | Description | Hardware / Libraries |
|---|---|---|
| [`ESP32S3_2inch_Capacitive_Touch`](ESP32S3_2inch_Capacitive_Touch/) | 2.0" capacitive touch LCD demo (includes `cst816d` driver) | CST816D touch · TFT_eSPI |
| [`ESP32S3_ECG_AD8232`](ESP32S3_ECG_AD8232/) | Read a heart-rate / ECG waveform | AD8232 ECG module (analog) |
| [`ESP32S3_I2C_AMG88xx_LCD`](ESP32S3_I2C_AMG88xx_LCD/) | 8×8 thermal-camera image on the LCD | AMG8833 (I²C) · TFT_eSPI |
| [`ESP32S3_I2C_MIC_REC`](ESP32S3_I2C_MIC_REC/) | I²S/I²C microphone audio capture | digital mic |
| [`ESP32S3_LCD2_0_SD`](ESP32S3_LCD2_0_SD/) | 2.0" LCD + microSD card read/write | SD (SPI) · TFT_eSPI |
| [`ESP32S3_OLED1_5INCH_TEST`](ESP32S3_OLED1_5INCH_TEST/) | 1.5" OLED display test | SSD1327/SH1107 (I²C/SPI) |
| [`ESP32S3_PWM_Servo_Gripper`](ESP32S3_PWM_Servo_Gripper/) | Control a servo gripper via PCA9685 PWM | PCA9685 (I²C 0x40) |

---

Install any libraries flagged by the IDE via **Sketch → Include Library → Manage Libraries**.
For LCD projects, configure **TFT_eSPI** `User_Setup.h` as shown in [02-Examples](../02-Examples/).
