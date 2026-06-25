// ============================================================
//  MOMO ESP32-S3 — 09 Bluetooth LE (server + notify)
//  Test with the nRF Connect mobile app -> "MOMO-ESP32S3"
// ============================================================
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#define SERVICE_UUID        "12345678-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "12345678-0001-1000-8000-00805f9b34fb"
BLECharacteristic* pChar; uint32_t value = 0;
void setup() {
  Serial.begin(115200);
  BLEDevice::init("MOMO-ESP32S3");
  BLEServer* s = BLEDevice::createServer();
  BLEService* svc = s->createService(SERVICE_UUID);
  pChar = svc->createCharacteristic(CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pChar->addDescriptor(new BLE2902());
  svc->start(); s->getAdvertising()->start();
  Serial.println("BLE advertising as 'MOMO-ESP32S3'");
}
void loop() { pChar->setValue((uint8_t*)&value,4); pChar->notify(); value++; delay(1000); }
