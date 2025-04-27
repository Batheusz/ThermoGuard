/**
 * @file main.cpp
 * @brief Projeto de monitoramento de temperatura e envio de alertas via WhatsApp e ThingSpeak usando ESP32.
 *
 * @details
 * - Leitura de temperatura com DS18B20.
 * - Envio de dados para ThingSpeak.
 * - Envio de mensagens de alerta via WhatsApp API (CallMeBot).
 * 
 * @author Matheus Orsini
 * @date 2025-04-26
 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Preferences.h>
#include <esp_sleep.h>
#include "funcoes.h"
#include "bleConfig.h"

// Whatsapp API key
String numCelular = "";
String apiWhats = "";

/** @brief Nome da rede Wi-Fi para conexão. */
String nomeWifi =  "";
/** @brief Senha da rede Wi-Fi. */
String senhaWifi = "";

// ThingSpeak config
/** @brief URL do servidor ThingSpeak para envio de dados. */
String server = "http://api.thingspeak.com/update";
/** @brief API key de acesso ao ThingSpeak. */
String apiKey = "";

// Sensor de temp
float setpointTemp;
uint32_t intervaloTemp = 60; //Segundos
/** @brief Instância do barramento OneWire para comunicação com o sensor DS18B20. */
OneWire PinOneWire(4); 
/** @brief Instância do sensor de temperatura DallasTemperature. */
DallasTemperature TempSensor(&PinOneWire);

// Dados RTC nao perdem no deep sleep somente com o reset
RTC_DATA_ATTR float ultimaTemp = 0;
// Dados NV nao perdem nem em reset
Preferences prefs;


// BLE
/** @brief Flag para controle de acionamento por BLE (interrupção). */
volatile bool flagBle = false;
int count = 0;
/** @brief Timestamp atual da última interrupção do botão. */
volatile uint32_t botaoTime = 0;
/** @brief Timestamp da última validação de botão (para debounce). */
volatile uint32_t botaoLastTime = 0;
volatile bool deviceConnected = false;
extern BLECharacteristic TemperaturaChar;

void setup() {
  Serial.begin(115200);

  // attachInterrupt(GPIO_NUM_15,botaoISR,RISING);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_15, 1);

  // esp_sleep_enable_timer_wakeup(intervaloTemp * 1000000ULL);

  TempSensor.begin();
  TempSensor.requestTemperatures();
  ultimaTemp = TempSensor.getTempCByIndex(0);

  switch (esp_sleep_get_wakeup_cause())
  {
    case ESP_SLEEP_WAKEUP_TIMER:{
      ConectaWifi(nomeWifi, senhaWifi);
      EnviarWeb(ultimaTemp);
      DesconectaWifi();
      Serial.println("Acordei pelo timer!");
      Serial.print("Leitura de:");
      Serial.println(ultimaTemp);
      break;
    }
    case ESP_SLEEP_WAKEUP_EXT0:{
      Serial.println("Acordei pelo botão!");
      IniciarBLE();
      uint32_t tempo = millis();
      while ((millis() - tempo) < 5 * 60 * 1000){
        if (deviceConnected) {
          
          Serial.println(ultimaTemp);
          TemperaturaChar.setValue(ultimaTemp);
          TemperaturaChar.notify();
          Serial.println("Envio BLE");
          delay(1000);
          break;
        }
      }
      EncerrarBLE();
      break;
    }
    default:{
      Serial.println("Acordei aleatório!");
      break;
    } 
  }
  
  esp_deep_sleep_start();
}

void loop() {
  Serial.println("Nunca Roda");
  // put your main code here, to run repeatedly:
}