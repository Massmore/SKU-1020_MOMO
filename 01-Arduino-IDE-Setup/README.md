# 01 · Arduino IDE Setup & CH343 USB Driver

This guide gets your **MOMO ESP32-S3** board ready to program with the Arduino IDE — step by step.

---

## STEP 1 · Install Arduino IDE

1. Download **Arduino IDE 2.x** from <https://www.arduino.cc/en/software>.
2. Install and open it.

## STEP 2 · Install the CH343 USB driver

MOMO uses the **WCH CH343P** USB-to-serial chip. If your computer does not show a COM port when the board is plugged in, install the driver:

| OS | Driver |
|---|---|
| **Windows** | `CH343SER.EXE` — <https://www.wch-ic.com/downloads/CH343SER_EXE.html> |
| **macOS** | `CH34xSER_MAC.ZIP` — <https://www.wch-ic.com/downloads/CH341SER_MAC_ZIP.html> |
| **Linux** | Built into kernel 4.x+ (`ch341`/`ch9102`) — usually no install needed |

**Windows install:** run `CH343SER.EXE` → click **INSTALL** → wait for "Driver install success".

> 💡 Most Windows 11 and recent macOS systems detect CH343 automatically. Only install the driver if no COM port appears.

## STEP 3 · Add the ESP32 board package

1. Open **File → Preferences**.
2. In **Additional boards manager URLs**, paste:
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
3. Open **Tools → Board → Boards Manager**, search **esp32**, and install **"esp32 by Espressif Systems"** (use **3.x**).

## STEP 4 · Connect the board & select the port

1. Connect MOMO to your PC with a **USB-C** cable.
2. **Tools → Board → ESP32 Arduino → "ESP32S3 Dev Module"**.
3. **Tools → Port →** select the COM port that appeared (e.g. `COM5`, or `/dev/cu.wchusbserial*`).

## STEP 5 · Configure board options

Set these under the **Tools** menu — important for the 16 MB Flash / 8 MB PSRAM module:

| Option | Value |
|---|---|
| **USB CDC On Boot** | Disable |
| **Flash Size** | **16 MB (128 Mb)** |
| **PSRAM** | **OPI PSRAM** |
| **Partition Scheme** | 16M Flash (3MB APP/9.9MB FATFS) |
| **Upload Speed** | 921600 |

## STEP 6 · Upload your first sketch

1. Open [`../02-Examples/01_HelloWorld`](../02-Examples/01_HelloWorld/).
2. Click **Upload (→)**.
3. Open **Serial Monitor** and set baud to **115200**.

> ✅ If it prints `PSRAM : 8388608 bytes`, your **OPI PSRAM** setting is correct — the board is fully working.

---

### Troubleshooting

| Symptom | Fix |
|---|---|
| No COM port | Install **CH343 driver** (Step 2); try another USB-C cable (data, not charge-only). |
| Upload fails / timeout | Hold **BOOT**, tap **RST**, release **BOOT**, then upload. |
| Garbled Serial output | Set Serial Monitor baud to **115200**. |
| PSRAM not detected | Set **PSRAM = OPI PSRAM** and re-upload. |

Next → [**02-Examples**](../02-Examples/)
