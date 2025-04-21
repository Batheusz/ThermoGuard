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
// Funções
uint8_t EnviarWhats(String messagem);
uint8_t ConectaWifi(const char* nome, const char* senha);

void setup() {
  Serial.begin(115200);
  ConectaWifi(nomeWifi,senhaWifi);

  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
  while (!Blynk.connected()) {
    delay(100);
    Serial.print("#");
  }
  Blynk.virtualWrite(V0, 12.25);
  
  // WiFi.disconnect(true);
  // WiFi.mode(WIFI_OFF);
  
}

void loop() {
  // put your main code here, to run repeatedly:
}

/**
 * Envia uma mensagem de texto via WhatsApp utilizando a API do CallMeBot.
 *
 * Inputs:
 *   - String messagem: Texto da mensagem a ser enviada.
 *
 * Outputs:
 *   - uint8_t:
 *       - 1 se a mensagem foi enviada com sucesso (HTTP 200).
 *       - 0 se houve falha no envio.
 */
uint8_t EnviarWhats(String messagem) {
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
