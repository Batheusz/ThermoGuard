#include "bleConfig.h"

BLECharacteristic TemperaturaChar(BLEUUID((uint16_t)0x2A6E), BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor TemperaturaDesc(BLEUUID((uint16_t)0x2912));

BLECharacteristic IntervaloChar(BLEUUID((uint16_t)0x2B8F), BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE);
BLEDescriptor IntervaloDesc(BLEUUID((uint16_t)0x290E));

BLECharacteristic WifiNomeChar("09caff73-bfcf-4066-90a1-b142a385d7f6", BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE);
BLEDescriptor WifiNomeDesc(BLEUUID((uint16_t)0x2903));

BLECharacteristic WifiSenhaChar("bd96b429-e88d-4702-9450-f023c08e509c", BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE);
BLEDescriptor WifiSenhaDesc(BLEUUID((uint16_t)0x2903));

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
  };

void IniciarBLE() {
    BLEDevice::init("ThermoGuard");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    // Temperatura
    pService->addCharacteristic(&TemperaturaChar);
    TemperaturaDesc.setValue("Temperatura lida do sensor em celsius");
    TemperaturaChar.addDescriptor(&TemperaturaDesc);
    // Intervalo
    pService->addCharacteristic(&IntervaloChar);
    IntervaloDesc.setValue("Tempo entre leitura em minutos");
    IntervaloChar.addDescriptor(&IntervaloDesc);
    // Wifi
    pService->addCharacteristic(&WifiNomeChar);
    WifiNomeDesc.setValue("Nome WiFi");
    WifiNomeChar.addDescriptor(&WifiNomeDesc);
    pService->addCharacteristic(&WifiSenhaChar);
    WifiSenhaDesc.setValue("Senha WiFi");
    WifiSenhaChar.addDescriptor(&WifiSenhaDesc);

    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pServer->getAdvertising()->start();
}

void EncerrarBLE() {
    BLEDevice::deinit(true);
}