#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Whatsapp API key
String numCelular = "";
String apiWhats = "";

// WiFi configuração
const char* nomeWifi =  "";
const char* senhaWifi = "";

// ThingSpeak config
const char* server = "http://api.thingspeak.com/update";
String apiKey = "";

// BLE
volatile bool flagBle = false;
void IRAM_ATTR botaoISR() {
  flagBle = true;
}

// Dados NV
RTC_DATA_ATTR uint16_t tempoLeitura = 20;  // 5 minutos
RTC_DATA_ATTR uint16_t tempoEnvio   = 30;   // 30 segundos
RTC_DATA_ATTR uint16_t segundosDesdeUltimaLeitura = 0;
RTC_DATA_ATTR uint16_t segundosDesdeUltimoEnvio   = 0;
RTC_DATA_ATTR float temperaturaLida = 0.0;

// Funções
uint8_t EnviarWhats(String messagem);
uint8_t EnviarWeb(float valor);
uint8_t ConectaWifi(const char* nome, const char* senha);
void DesconectaWifi();

void setup() {
  Serial.begin(115200);

  int tempoCiclo = 10;  // intervalo de deep sleep (segundos)
  Serial.println("Dispositivo acordou do deep sleep!");

  segundosDesdeUltimaLeitura += tempoCiclo;
  segundosDesdeUltimoEnvio   += tempoCiclo;
  Serial.println("Tempo desde última leitura: " + String(segundosDesdeUltimaLeitura));
  Serial.println("Tempo desde último envio: " + String(segundosDesdeUltimoEnvio));

  if (segundosDesdeUltimaLeitura >= tempoLeitura) {
    temperaturaLida = 36.0 + random(-30, 30) / 10.0;
    Serial.println("Temperatura lida: " + String(temperaturaLida) + "°C");
    segundosDesdeUltimaLeitura = 0;
  }

  if (segundosDesdeUltimoEnvio >= tempoEnvio) {
    ConectaWifi(nomeWifi,senhaWifi);
    EnviarWeb(temperaturaLida);
    DesconectaWifi();
    segundosDesdeUltimoEnvio = 0;
  }

  esp_sleep_enable_timer_wakeup(tempoCiclo * 1000000ULL);  // microssegundos
  esp_deep_sleep_start();

}

void loop() {
  // put your main code here, to run repeatedly:
}

/**
 * Envia uma mensagem de texto via WhatsApp utilizando a API do CallMeBot,
 * somente se a conexão Wi-Fi estiver ativa.
 *
 * Inputs:
 *   - String messagem: Texto da mensagem a ser enviada.
 *
 * Outputs:
 *   - uint8_t:
 *       - 1 se a mensagem foi enviada com sucesso (HTTP 200).
 *       - 0 se houve falha na requisição HTTP.
 *       - 2 se o Wi-Fi não está conectado.
 */
uint8_t EnviarWhats(String messagem) {
  if (WiFi.status() == WL_CONNECTED) {
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + numCelular + "&apikey=" + apiWhats + "&text=" + urlEncode(messagem);
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST(url);
    
    if (httpResponseCode == 200) {
      http.end();
      return 1;
    }
    else {
      http.end();
      return 0;
    }
}
else
{
  return 2;
}
}

/**
 * Envia um valor numérico (float) para um servidor web (ex: ThingSpeak),
 * via requisição HTTP GET, apenas se o Wi-Fi estiver conectado.
 *
 * Inputs:
 *   - float valor: Valor numérico a ser enviado.
 *
 * Outputs:
 *   - uint8_t:
 *       - 1 se o valor foi enviado com sucesso (HTTP 200).
 *       - 0 se houve falha na requisição HTTP.
 *       - 2 se o Wi-Fi não está conectado.
 */
uint8_t EnviarWeb(float valor) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey + "&field1=" + String(valor);
    http.begin(url);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode == 200) {
      http.end();
      return 1;
    } 
    else {
      http.end();
      return 0;
    }
  }
  else {
    return 2;
  }
}

/**
 * Conecta o dispositivo à rede Wi-Fi especificada.
 *
 * Inputs:
 *   - const char* nome: Nome (SSID) da rede Wi-Fi.
 *   - const char* senha: Senha da rede Wi-Fi.
 *
 * Outputs:
 *   - uint8_t:
 *       - 1 se a conexão foi bem-sucedida.
 *       - 0 se a conexão falhou após o tempo de espera.
 */
uint8_t ConectaWifi(const char* nome, const char* senha) {
  WiFi.begin(nome, senha);
  for (uint8_t i = 0; i < 20; i++) {
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) {
    return 1;
  }
  else{
    return 0;
  }
}

/**
 * Desconecta o dispositivo da rede Wi-Fi e desativa o módulo Wi-Fi.
 *
 * Inputs:
 *   - Nenhum.
 *
 * Outputs:
 *   - void (sem retorno).
 */
void DesconectaWifi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}
