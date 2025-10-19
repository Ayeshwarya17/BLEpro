#ifndef BLE_LOGGER_H
#define BLE_LOGGER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h> 

class BLELogger {
public:
  BLELogger();
  void begin();
  void sendLog(const String& log);
  bool isConnected();

private:
  BLECharacteristic* pCharacteristic;
  BLEServer* pServer;
  bool deviceConnected;
  int logCount;
  const int maxLogs = 20;

  class ServerCallbacks : public BLEServerCallbacks {
    BLELogger* parent;
  public:
    ServerCallbacks(BLELogger* p) : parent(p) {}
    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);
  };
};

#endif
