/**
 * @file bleconfig.cpp
 * @brief Implementação da configuração BLE do ESP32.
 *
 * @details
 * Define as características BLE (temperatura, setpoint, intervalo, Wi-Fi e telefone).
 * Controla os eventos de conexão e escrita nas características, atualizando as variáveis
 * globais e salvando-as na NVS quando necessário.
 */

#include "bleConfig.h"
#include "funcoes.h" 

BLECharacteristic TemperaturaChar(BLEUUID((uint16_t)0x2A6E), 
                                  BLECharacteristic::PROPERTY_READ|
                                  BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor TemperaturaDesc(BLEUUID((uint16_t)0x2912));

BLECharacteristic IntervaloChar(BLEUUID((uint16_t)0x2B8F), 
                                BLECharacteristic::PROPERTY_READ|
                                BLECharacteristic::PROPERTY_NOTIFY|
                                BLECharacteristic::PROPERTY_WRITE|
                                BLECharacteristic::PROPERTY_INDICATE);
BLEDescriptor IntervaloDesc(BLEUUID((uint16_t)0x290E));

BLECharacteristic SetPointChar("22399fc7-2eef-48c8-924c-bba26ca25376", 
                               BLECharacteristic::PROPERTY_READ|
                               BLECharacteristic::PROPERTY_NOTIFY|
                               BLECharacteristic::PROPERTY_WRITE|
                               BLECharacteristic::PROPERTY_INDICATE);
BLEDescriptor SetPointDesc(BLEUUID((uint16_t)0x2903));

BLECharacteristic WifiNomeChar("09caff73-bfcf-4066-90a1-b142a385d7f6", 
                               BLECharacteristic::PROPERTY_READ|
                               BLECharacteristic::PROPERTY_NOTIFY|
                               BLECharacteristic::PROPERTY_WRITE|
                               BLECharacteristic::PROPERTY_INDICATE);
BLEDescriptor WifiNomeDesc(BLEUUID((uint16_t)0x2903));

BLECharacteristic WifiSenhaChar("74ddaf0d-dd25-4c8f-bb76-bee0df68c786", 
                                BLECharacteristic::PROPERTY_READ|
                                BLECharacteristic::PROPERTY_NOTIFY|
                                BLECharacteristic::PROPERTY_WRITE|
                                BLECharacteristic::PROPERTY_INDICATE);
BLEDescriptor WifiSenhaDesc(BLEUUID((uint16_t)0x2903));

BLECharacteristic TelefoneChar("bd96b429-e88d-4702-9450-f023c08e509c", 
                                BLECharacteristic::PROPERTY_READ|
                                BLECharacteristic::PROPERTY_NOTIFY|
                                BLECharacteristic::PROPERTY_WRITE|
                                BLECharacteristic::PROPERTY_INDICATE);
BLEDescriptor TelefoneDesc(BLEUUID((uint16_t)0x2903));

/**
 * @class MyServerCallbacks
 * @brief Callback de eventos do servidor BLE. Atualiza estado da conexão.
 *
 * @details
 * - onConnect(): Define 'deviceConnected = true'.
 * - onDisconnect(): Define 'deviceConnected = false'.
 */
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Conectando...");
      deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
      Serial.println("Desconectando...");
      deviceConnected = false;
    }
  };

/**
 * @class CharCallback
 * @brief Callback para escrita em características BLE. Atualiza variáveis globais.
 *
 * @details
 * - Interpreta strings recebidas do App via BLE.
 * - Atualiza variáveis globais (nome e senha do Wi-Fi, telefone, setpoint, intervalo).
 * - Chama a função SalvaConfig() para persistência imediata na NVS.
 */
class CharCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) override {
    std::string value = pCharacteristic->getValue();
    if (pCharacteristic == &WifiNomeChar) {
      nomeWifi = String(value.c_str());
      Serial.print("Novo nome do WiFi recebido: ");
      Serial.println(nomeWifi);
    } else if (pCharacteristic == &WifiSenhaChar) {
      senhaWifi = String(value.c_str());
      Serial.print("Nova senha do WiFi recebida: ");
      Serial.println(senhaWifi);
    } else if (pCharacteristic == &SetPointChar) {
      setPointTemp = String(value.c_str()).toFloat();
      Serial.print("Novo setpoint recebido: ");
      Serial.println(setPointTemp);
    } else if (pCharacteristic == &IntervaloChar) {
      intervaloTemp = String(value.c_str()).toInt();
      Serial.print("Novo intervalo recebido: ");
      Serial.println(intervaloTemp);
    } else if (pCharacteristic == &TelefoneChar) {
      numCelular = String(value.c_str());
      Serial.print("Número de celular recebido: ");
      Serial.println(numCelular);
    }
  }
};
  
/**
 * @brief Inicializa o Bluetooth Low Energy (BLE).
 * 
 * Configura o servidor BLE, cria as características e descritores
 * para temperatura, intervalo de leitura e dados de Wi-Fi, e inicia
 * o advertising BLE.
 */
void IniciarBLE() {
    BLEDevice::init("ThermoGuard");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    BLEService *pServiceWifi = pServer->createService(SERVICE_WIFI_UUID);

    // Temperatura
    TemperaturaDesc.setValue("Temperatura lida do sensor em celsius");
    TemperaturaChar.addDescriptor(&TemperaturaDesc);
    pService->addCharacteristic(&TemperaturaChar);
    SetPointDesc.setValue("Setpoint para alerta");
    SetPointChar.addDescriptor(&SetPointDesc);
    SetPointChar.setCallbacks(new CharCallback());
    pService->addCharacteristic(&SetPointChar);
    // Intervalo
    IntervaloDesc.setValue("Tempo entre leitura em minutos");
    IntervaloChar.addDescriptor(&IntervaloDesc);
    IntervaloChar.setCallbacks(new CharCallback());
    pService->addCharacteristic(&IntervaloChar);

    // Wifi
    WifiNomeDesc.setValue("Nome WiFi");
    WifiNomeChar.addDescriptor(&WifiNomeDesc);
    WifiNomeChar.setCallbacks(new CharCallback());
    pServiceWifi->addCharacteristic(&WifiNomeChar);
    WifiSenhaDesc.setValue("Senha WiFi");
    WifiSenhaChar.addDescriptor(&WifiSenhaDesc);
    WifiSenhaChar.setCallbacks(new CharCallback());
    pServiceWifi->addCharacteristic(&WifiSenhaChar);
    // Telefone
    TelefoneDesc.setValue("Numero de telefone");
    TelefoneChar.addDescriptor(&TelefoneDesc);
    TelefoneChar.setCallbacks(new CharCallback());
    pServiceWifi->addCharacteristic(&TelefoneChar);

    pService->start();
    pServiceWifi->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->addServiceUUID(SERVICE_WIFI_UUID);
    // pServer->getAdvertising()->start();
    BLEDevice::startAdvertising();
}

/**
 * @brief Encerra o Bluetooth Low Energy (BLE).
 * 
 * Finaliza o serviço BLE e libera os recursos alocados.
 */
void EncerrarBLE() {
    BLEDevice::deinit(true);
}