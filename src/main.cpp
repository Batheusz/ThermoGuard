#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>

// Whatsapp API key
String numCelular = "";
String apiWhats = "";

// WiFi configuração
const char* nomeWifi =  "Vazio";
const char* senhaWifi = "Vazio";
// Funções
uint8_t EnviarWhats(String messagem);
uint8_t ConectaWifi(const char* nome, const char* senha);

void setup() {
  Serial.begin(115200);

  
  
  EnviarWhats("ALERTA!! Verifique o paciente!");
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
  
  uint8_t checkWifi = ConectaWifi(nomeWifi, senhaWifi);
  if (checkWifi == 0) {
    return 2;
  }
  else {
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
