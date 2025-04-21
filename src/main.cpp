#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <UrlEncode.h>

// Blynk config
#define BLYNK_TEMPLATE_ID "TMPL2KunrIRsv"
#define BLYNK_TEMPLATE_NAME "ThermoGuard"
#define BLYNK_AUTH_TOKEN "oXNoqXQ6U49cD_OIWfaX756gTmnXg3Bx"
#define BLYNK_PRINT Serial // Debuging blynk on serial
#include <BlynkSimpleEsp32.h>

// Whatsapp API key
String numCelular = "";
String apiWhats = "";

// WiFi configuração
const char* nomeWifi =  "";
const char* senhaWifi = "";

// ThingSpeak config
const char* server = "http://api.thingspeak.com/update";
String apiKey = "";

// Funções
uint8_t EnviarWhats(String messagem);
uint8_t ConectaWifi(const char* nome, const char* senha);

void setup() {
  Serial.begin(115200);
  ConectaWifi(nomeWifi,senhaWifi);
  
  float temperatura = 25.35;  // valor fictício, substitua pelo sensor

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey + "&field1=" + String(temperatura);
    http.begin(url);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.print("Resposta do servidor: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Erro: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }

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
