#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define DEVICE_ID "xlab-1"
#define SERVICE_UUID        "dedc07fa-e0b0-4aa3-b411-38c95242ebc7"
#define CHARACTERISTIC_UUID "c63bcbc3-af3f-46da-9893-556e6e763f1e"

BLECharacteristic *pCharacteristic;

class ServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("onConnect");
  };

  void onDisconnect(BLEServer* pServer) {
    Serial.println("onDisconnect");
  }
};

class CharacteristicCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    int value = String(pCharacteristic->getValue().c_str()).toInt();
    Serial.println(value);
  }
};

void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init(DEVICE_ID);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setCallbacks(new CharacteristicCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
}

void loop() {}