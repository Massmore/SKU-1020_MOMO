/****************************************************************
 *  ESP32-S3 I2S Mic (MSM261S4030H0R)
 *  - Stream เสียงออก Web server (WAV)
 *  - บันทึกลง SD card ต่อเนื่อง ตัดไฟล์ทุก 30 นาที
 *  - flush + patch header ทุก 2 วิ กันไฟล์เสียเมื่อค้าง/ไฟดับ
 *  - SD self-test ตอนเริ่ม + สแกนเลขไฟล์กันทับ
 *  - ตั้งชื่อไฟล์ด้วย timestamp จาก NTP
 *  - ปุ่มหยุด/เริ่มบันทึกบนหน้าเว็บ
 *
 *  Hardware:
 *    I2S Mic: WS=GPIO4  SD=GPIO5  CLK=GPIO2  (L/R -> VDD = Right ch)
 *    SD(SPI): MISO=13  MOSI=11  SCLK=12  CS=0
 ****************************************************************/

#include <WiFi.h>
#include <WebServer.h>
#include <driver/i2s.h>
#include "freertos/ringbuf.h"
#include <SPI.h>
#include <SD.h>
#include <time.h>

// ==================== ตั้งค่าผู้ใช้ ====================
const char* ssid     = "massmore_2.4G";
const char* password = "zxc12345";

const char* NTP_SERVER = "pool.ntp.org";
const long  GMT_OFFSET = 7 * 3600;   // ไทย UTC+7
const int   DST_OFFSET = 0;

// ---------- I2S Pins ----------
#define PIN_WS   4
#define PIN_SD   5
#define PIN_CLK  2

// ---------- SD (SPI) Pins ----------
#define SD_MISO  13
#define SD_MOSI  11
#define SD_SCLK  12
#define SD_CS    0

// ---------- Audio ----------
#define I2S_PORT       I2S_NUM_0
#define SAMPLE_RATE    16000
#define SAMPLE_BITS    16
#define GAIN_SHIFT     12          // ยิ่งน้อยยิ่งดัง (11=ดังขึ้น, 13=เบาลง)

// ---------- Recording ----------
#define BYTES_PER_SEC       (SAMPLE_RATE * SAMPLE_BITS / 8)
#define FILE_DURATION_MS    (30UL * 60UL * 1000UL)   // ตัดไฟล์ทุก 30 นาที
#define FLUSH_INTERVAL_MS   2000                     // flush+patch ทุก 2 วินาที
// =====================================================

WebServer server(80);
SPIClass  sdSPI(FSPI);

RingbufHandle_t  rb = NULL;
volatile bool    streaming = false;
volatile bool    sdReady   = false;
volatile bool    recording = true;     // สถานะบันทึก (ปุ่มเว็บคุม)
volatile bool    timeValid = false;

// คำสั่งจาก web -> i2sTask (กัน race ข้าม task / core)
volatile bool    reqStart = false;
volatile bool    reqStop  = false;

int      fileIndex = 0;
File     wavFile;
uint32_t fileDataBytes = 0;

// ==================== I2S ====================
void i2sInit() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,   // L/R -> VDD
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 256,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };
  i2s_pin_config_t pin_config = {
    .bck_io_num   = PIN_CLK,
    .ws_io_num    = PIN_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num  = PIN_SD
  };
  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
  i2s_zero_dma_buffer(I2S_PORT);
}

// ==================== WAV Header ====================
void buildWavHeader(uint8_t* h, uint32_t sampleRate, uint16_t bits, uint16_t ch, uint32_t dataSize) {
  uint32_t byteRate   = sampleRate * ch * bits / 8;
  uint16_t blockAlign = ch * bits / 8;
  uint32_t chunkSize  = dataSize + 36;
  memcpy(h, "RIFF", 4);            memcpy(h + 4, &chunkSize, 4);
  memcpy(h + 8, "WAVE", 4);        memcpy(h + 12, "fmt ", 4);
  uint32_t sub1 = 16;  memcpy(h + 16, &sub1, 4);
  uint16_t fmt  = 1;   memcpy(h + 20, &fmt, 2);
  memcpy(h + 22, &ch, 2);          memcpy(h + 24, &sampleRate, 4);
  memcpy(h + 28, &byteRate, 4);    memcpy(h + 32, &blockAlign, 2);
  memcpy(h + 34, &bits, 2);        memcpy(h + 36, "data", 4);
  memcpy(h + 40, &dataSize, 4);
}

// ==================== SD Self-Test ====================
bool sdSelfTest() {
  Serial.println("\n--- SD Self-Test ---");
  uint8_t type = SD.cardType();
  if (type == CARD_NONE) { Serial.println("ไม่พบการ์ด"); return false; }
  const char* t = (type == CARD_MMC) ? "MMC" :
                  (type == CARD_SD)  ? "SDSC" :
                  (type == CARD_SDHC)? "SDHC" : "UNKNOWN";
  Serial.printf("ชนิดการ์ด : %s\n", t);
  Serial.printf("ความจุ    : %llu MB\n", SD.cardSize() / (1024ULL * 1024ULL));
  Serial.printf("ใช้ไป     : %llu MB\n", SD.usedBytes() / (1024ULL * 1024ULL));

  const char* tn  = "/.sdtest.tmp";
  const char* msg = "esp32-sd-ok";
  File f = SD.open(tn, FILE_WRITE);
  if (!f) { Serial.println("เขียนทดสอบไม่ได้"); return false; }
  f.print(msg); f.close();

  f = SD.open(tn, FILE_READ);
  if (!f) { Serial.println("อ่านทดสอบไม่ได้"); return false; }
  String back = f.readString(); f.close();
  SD.remove(tn);

  if (back != msg) { Serial.println("ข้อมูลอ่านกลับไม่ตรง"); return false; }
  Serial.println("เขียน/อ่าน : ผ่าน");
  Serial.println("--- SD พร้อมใช้งาน ---\n");
  return true;
}

// ==================== หาเลขไฟล์ถัดไป (กันทับ) ====================
void scanNextIndex() {
  fileIndex = 0;
  File root = SD.open("/");
  if (!root) return;
  for (File f = root.openNextFile(); f; f = root.openNextFile()) {
    String n = f.name();
    int p = n.lastIndexOf("rec_");
    if (p >= 0) {
      int num = n.substring(p + 4, p + 8).toInt();
      if (num >= fileIndex) fileIndex = num + 1;
    }
    f.close();
  }
  root.close();
  Serial.printf("ไฟล์ถัดไปจะเริ่มที่ #%04d\n", fileIndex);
}

// ==================== ชื่อไฟล์ + timestamp ====================
void makeFileName(char* out, size_t n) {
  struct tm tm;
  if (timeValid && getLocalTime(&tm, 50)) {
    snprintf(out, n, "/rec_%04d_%04d%02d%02d_%02d%02d%02d.wav",
             fileIndex, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
  } else {
    snprintf(out, n, "/rec_%04d.wav", fileIndex);
  }
}

// ==================== เปิด/ปิด/patch ไฟล์ ====================
bool openNewFile() {
  if (!sdReady) return false;
  char name[64];
  makeFileName(name, sizeof(name));
  wavFile = SD.open(name, FILE_WRITE);
  if (!wavFile) { Serial.printf("เปิดไฟล์ %s ไม่ได้\n", name); return false; }
  uint8_t h[44];
  buildWavHeader(h, SAMPLE_RATE, SAMPLE_BITS, 1, 0);   // placeholder
  wavFile.write(h, 44);
  fileDataBytes = 0;
  Serial.printf("เริ่มบันทึก %s\n", name);
  return true;
}

// patch header ระหว่างบันทึก (ไม่ปิดไฟล์) -> ทนการค้าง/ไฟดับ
void patchHeaderLive() {
  if (!wavFile) return;
  uint32_t pos = wavFile.position();
  uint8_t h[44];
  buildWavHeader(h, SAMPLE_RATE, SAMPLE_BITS, 1, fileDataBytes);
  wavFile.seek(0);
  wavFile.write(h, 44);
  wavFile.seek(pos);
  wavFile.flush();              // บังคับเขียนลงการ์ดจริง
}

void closeCurrentFile() {
  if (!wavFile) return;
  uint8_t h[44];
  buildWavHeader(h, SAMPLE_RATE, SAMPLE_BITS, 1, fileDataBytes);
  wavFile.seek(0);
  wavFile.write(h, 44);
  wavFile.flush();
  wavFile.close();
  Serial.printf("ปิดไฟล์ #%04d : %u bytes (%.1f นาที)\n",
                fileIndex, fileDataBytes, fileDataBytes / (float)BYTES_PER_SEC / 60.0);
}

// ==================== I2S Reader + SD Writer Task (core 1) ====================
void i2sTask(void* arg) {
  const int N = 256;
  int32_t raw[N];
  int16_t pcm[N];
  size_t  bytesRead;
  bool    fileOpen = false;

  uint32_t lastFlush = millis();
  uint32_t fileStart = millis();

  if (sdReady && recording) { fileOpen = openNewFile(); fileStart = millis(); }

  for (;;) {
    i2s_read(I2S_PORT, raw, sizeof(raw), &bytesRead, portMAX_DELAY);
    int samples = bytesRead / sizeof(int32_t);

    for (int i = 0; i < samples; i++) {
      int32_t s = raw[i] >> GAIN_SHIFT;
      if (s > 32767)  s = 32767;
      if (s < -32768) s = -32768;
      pcm[i] = (int16_t)s;
    }
    int nbytes = samples * sizeof(int16_t);

    // --- คำสั่งหยุดจากเว็บ ---
    if (reqStop) {
      reqStop = false;
      if (fileOpen) { closeCurrentFile(); fileIndex++; fileOpen = false; }
      recording = false;
      Serial.println(">> หยุดบันทึกแล้ว");
    }
    // --- คำสั่งเริ่มจากเว็บ ---
    if (reqStart) {
      reqStart = false;
      if (sdReady && !fileOpen) {
        fileOpen  = openNewFile();
        fileStart = millis();
        lastFlush = millis();
        recording = true;
        Serial.println(">> เริ่มบันทึกใหม่");
      }
    }

    // --- เขียน SD ---
    if (sdReady && recording && fileOpen) {
      wavFile.write((uint8_t*)pcm, nbytes);
      fileDataBytes += nbytes;

      uint32_t now = millis();

      // flush + patch header เป็นระยะ -> เสียมากสุดแค่ ~2 วินาทีสุดท้าย
      if (now - lastFlush >= FLUSH_INTERVAL_MS) {
        patchHeaderLive();
        lastFlush = now;
      }

      // ตัดไฟล์ทุก 30 นาที ตามเวลาจริง
      if (now - fileStart >= FILE_DURATION_MS) {
        closeCurrentFile();
        fileIndex++;
        fileOpen  = openNewFile();
        fileStart = now;
        lastFlush = now;
      }
    }

    // --- stream ---
    if (streaming) xRingbufferSend(rb, pcm, nbytes, 0);
  }
}

// ==================== Stream Handler ====================
void handleStream() {
  WiFiClient client = server.client();
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Content-Type: audio/wav\r\n");
  client.print("Connection: close\r\n");
  client.print("Access-Control-Allow-Origin: *\r\n\r\n");

  uint8_t h[44];
  buildWavHeader(h, SAMPLE_RATE, SAMPLE_BITS, 1, 0xFFFFFFFF);
  client.write(h, 44);

  size_t dummy; void* old;
  while ((old = xRingbufferReceive(rb, &dummy, 0)) != NULL) vRingbufferReturnItem(rb, old);
  streaming = true;

  while (client.connected()) {
    size_t len = 0;
    void* data = xRingbufferReceive(rb, &len, pdMS_TO_TICKS(100));
    if (data != NULL) {
      int w = client.write((uint8_t*)data, len);
      vRingbufferReturnItem(rb, data);
      if (w == 0) break;
    }
    delay(1);
  }
  streaming = false;
}

// ==================== Web control ====================
void handleStart() { reqStart = true; server.send(200, "text/plain", "starting"); }
void handleStop()  { reqStop  = true; server.send(200, "text/plain", "stopping"); }

void handleStatus() {
  char buf[128];
  snprintf(buf, sizeof(buf), "{\"recording\":%s,\"sd\":%s,\"file\":%d,\"time\":%s}",
           recording ? "true" : "false", sdReady ? "true" : "false",
           fileIndex, timeValid ? "true" : "false");
  server.send(200, "application/json", buf);
}

void handleRoot() {
  String html =
    "<!DOCTYPE html><html><head><meta charset='utf-8'><title>ESP32 Mic</title>"
    "<meta name='viewport' content='width=device-width,initial-scale=1'></head>"
    "<body style='font-family:sans-serif;text-align:center;margin-top:30px'>"
    "<h2>ESP32-S3 I2S Mic Stream + SD Record</h2>"
    "<audio controls src='/stream'></audio><br><br>"
    "<button onclick=\"fetch('/start').then(()=>setTimeout(upd,300))\" "
    "style='font-size:18px;padding:10px 24px;margin:5px'>\u25CF \u0E40\u0E23\u0E34\u0E48\u0E21\u0E1A\u0E31\u0E19\u0E17\u0E36\u0E01</button>"
    "<button onclick=\"fetch('/stop').then(()=>setTimeout(upd,300))\" "
    "style='font-size:18px;padding:10px 24px;margin:5px'>\u25A0 \u0E2B\u0E22\u0E38\u0E14\u0E1A\u0E31\u0E19\u0E17\u0E36\u0E01</button>"
    "<p id='st'>...</p>"
    "<script>"
    "function upd(){fetch('/status').then(r=>r.json()).then(j=>{"
    "document.getElementById('st').innerHTML="
    "'\u0E1A\u0E31\u0E19\u0E17\u0E36\u0E01: '+(j.recording?'<b style=\\'color:red\\'>\u0E01\u0E33\u0E25\u0E31\u0E07\u0E1A\u0E31\u0E19\u0E17\u0E36\u0E01</b>':'\u0E2B\u0E22\u0E38\u0E14')+"
    "' | SD: '+(j.sd?'OK':'\u0E44\u0E21\u0E48\u0E21\u0E35')+' | \u0E44\u0E1F\u0E25\u0E4C\u0E16\u0E31\u0E14\u0E44\u0E1B #'+j.file+"
    "' | NTP: '+(j.time?'\u0E15\u0E23\u0E07':'\u0E22\u0E31\u0E07\u0E44\u0E21\u0E48\u0E0B\u0E34\u0E07\u0E04\u0E4C');});}"
    "upd();setInterval(upd,3000);"
    "</script></body></html>";
  server.send(200, "text/html", html);
}

// ==================== Setup ====================
void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\n=== ESP32-S3 Mic Stream + SD Record ===");

  i2sInit();

  // ---- SD init + self-test ----
  sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  if (SD.begin(SD_CS, sdSPI, 20000000) && sdSelfTest()) {
    sdReady = true;
    scanNextIndex();
  } else {
    Serial.println("SD ไม่พร้อม - จะสตรีมอย่างเดียว");
  }

  // ---- Ring buffer (I2S -> stream) ----
  rb = xRingbufferCreate(32 * 1024, RINGBUF_TYPE_BYTEBUF);
  if (rb == NULL) { Serial.println("RingBuffer alloc failed!"); while (1) delay(1000); }

  // ---- WiFi ----
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) { delay(300); Serial.print("."); }
  Serial.println();
  Serial.print("IP: "); Serial.println(WiFi.localIP());

  // ---- NTP (sync ก่อนเริ่ม task เพื่อให้ไฟล์แรกมี timestamp) ----
  configTime(GMT_OFFSET, DST_OFFSET, NTP_SERVER);
  struct tm tm;
  if (getLocalTime(&tm, 5000)) {
    timeValid = true;
    Serial.printf("NTP OK: %04d-%02d-%02d %02d:%02d:%02d\n",
                  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                  tm.tm_hour, tm.tm_min, tm.tm_sec);
  } else {
    Serial.println("NTP ซิงค์ไม่ได้ - จะใช้ชื่อไฟล์แบบเลขลำดับ");
  }

  // ---- I2S task บน core 1 (WiFi อยู่ core 0) ----
  xTaskCreatePinnedToCore(i2sTask, "i2sTask", 8192, NULL, 5, NULL, 1);

  // ---- Web routes ----
  server.on("/", handleRoot);
  server.on("/stream", handleStream);
  server.on("/start",  handleStart);
  server.on("/stop",   handleStop);
  server.on("/status", handleStatus);
  server.begin();
  Serial.println("Web server เริ่มทำงาน");
}

void loop() {
  server.handleClient();
}