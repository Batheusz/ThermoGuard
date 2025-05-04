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
String apiWhats = "6043662";

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
float setPointTemp = 0.0;
uint32_t intervaloTemp = 15; //Minutos
/** @brief Instância do barramento OneWire para comunicação com o sensor DS18B20. */
OneWire PinOneWire(4); 
/** @brief Instância do sensor de temperatura DallasTemperature. */
DallasTemperature TempSensor(&PinOneWire);

// Dados RTC nao perdem no deep sleep somente com o reset
RTC_DATA_ATTR float ultimaTemp = 0;
// Dados NV nao perdem nem em reset
Preferences prefs;

// BLE
volatile bool deviceConnected = false;
extern BLECharacteristic TemperaturaChar;
extern BLECharacteristic SetPointChar;
extern BLECharacteristic IntervaloChar;
extern BLECharacteristic WifiNomeChar;
extern BLECharacteristic WifiSenhaChar;
extern BLECharacteristic TelefoneChar;
/**
 * @brief Função de configuração inicial do dispositivo.
 * 
 * Inicializa o monitor serial, configura o sensor de temperatura, define
 * as causas de despertar do deep sleep (timer e botão físico) e realiza
 * a ação apropriada conforme a causa do despertar:
 * - Timer: conecta ao Wi-Fi e envia dados ao ThingSpeak.
 * - Botão: inicia o servidor BLE para envio de dados via Bluetooth.
 * - Outro: apenas imprime mensagem no console.
 */
void setup() {
  CarregarConfig();

  Serial.begin(115200);
  
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_15, 1);

  esp_sleep_enable_timer_wakeup(intervaloTemp * 1000000ULL);
  
  Serial.print("ultima temperatura lida:"); Serial.println(ultimaTemp);
  Serial.print("set point lidos:"); Serial.println(setPointTemp);
  Serial.print("intervalo lidos:"); Serial.println(intervaloTemp);
  Serial.print("wifi nome lidos:"); Serial.println(nomeWifi);
  Serial.print("wifi senha lidos:"); Serial.println(senhaWifi);
  Serial.print("telefone lido:"); Serial.println(numCelular);

  TempSensor.begin();
  TempSensor.requestTemperatures();
  ultimaTemp = TempSensor.getTempCByIndex(0);

  switch (esp_sleep_get_wakeup_cause())
  {
    case ESP_SLEEP_WAKEUP_TIMER:{
      if(ConectaWifi(nomeWifi, senhaWifi)){
        if (ultimaTemp >= setPointTemp){
          EnviarWhats("ALERTA!!\nTemperatura acima da configurada!\nVerifique o paciente!!");
        }
        if(EnviarWeb(ultimaTemp)){
          Serial.println("Enviado com sucesso");
        }else{
          Serial.println("Falha ao enviar");
        }
        DesconectaWifi();
        Serial.println("Acordei pelo timer!");
        Serial.print("Leitura de:");
        Serial.println(ultimaTemp);
      }else{
        Serial.println("Nao foi possível se conectar ao wifi");
      }
      break;
    }
    case ESP_SLEEP_WAKEUP_EXT0:{
      Serial.println("Acordei pelo botão!");
      IniciarBLE();
      uint32_t tempo = millis();
      while (((millis() - tempo) < 1 * 60 * 1000) || deviceConnected){
        if (deviceConnected) {
          // TempSensor.requestTemperatures();
          // ultimaTemp = TempSensor.getTempCByIndex(0);
          // String tempStr = String(ultimaTemp,2);
          
          // TemperaturaChar.setValue(tempStr.c_str());
          // TemperaturaChar.notify();

          // SetPointChar.setValue(setPointTemp);
          // IntervaloChar.notify();

          // IntervaloChar.setValue(intervaloTemp);
          // IntervaloChar.notify();

          // WifiNomeChar.setValue(nomeWifi.c_str());
          // WifiNomeChar.notify();

          // WifiSenhaChar.setValue(senhaWifi.c_str());
          // WifiSenhaChar.notify();

          // TelefoneChar.setValue(numCelular.c_str());
          // TelefoneChar.notify();
            
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
  Serial.println("Dormindo...");
  SalvaConfig(nomeWifi,senhaWifi,setPointTemp,intervaloTemp,numCelular);
  esp_deep_sleep_start();
}

void loop() {
  Serial.println("Nunca Roda");
  // put your main code here, to run repeatedly:
}