// ============================================================
//  MOMO ESP32-S3 — 08 WiFi Scan & Connect (2.4 GHz)
// ============================================================
#include <WiFi.h>
const char* SSID = "YOUR_WIFI_NAME";       // << edit
const char* PASS = "YOUR_WIFI_PASSWORD";   // << edit
void setup() {
  Serial.begin(115200); delay(1000);
  WiFi.mode(WIFI_STA); WiFi.disconnect();
  int n = WiFi.scanNetworks();
  Serial.printf("Found %d networks:\n", n);
  for (int i=0;i<n;i++) Serial.printf("  %2d) %-24s %4d dBm\n", i+1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
  Serial.printf("\nConnecting to %s ...\n", SSID);
  WiFi.begin(SSID, PASS);
  while (WiFi.status()!=WL_CONNECTED){ delay(500); Serial.print("."); }
  Serial.printf("\nConnected! IP = %s\n", WiFi.localIP().toString().c_str());
}
void loop() {}
