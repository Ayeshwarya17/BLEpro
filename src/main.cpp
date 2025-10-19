#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

// Logging control
bool enableSerialLog = true;
bool enableBLELog = true;
int logCount = 0;
const int maxLogs = 20;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    if (enableSerialLog) {
      Serial.println("BLE device connected");
    }
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    if (enableSerialLog) {
      Serial.println("BLE device disconnected");
    }
    // Reset log count if you want to allow reconnect and resend
    logCount = 0;
  }
};

void setup() {
  Serial.begin(115200);
  if (enableSerialLog) {
    Serial.println("Starting BLE Logger...");
  }

  BLEDevice::init("ESP32 Logger");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

  if (enableSerialLog) {
    Serial.println("BLE service started. Waiting for connection...");
  }
}

void loop() {
  if (deviceConnected && enableBLELog && logCount < maxLogs) {
    String log = "Log #" + String(logCount + 1) + ": millis = " + String(millis());

    if (enableSerialLog) {
      Serial.println(log);
    }

    pCharacteristic->setValue(log.c_str());
    pCharacteristic->notify();

    logCount++;
    delay(2000);
  }
}
