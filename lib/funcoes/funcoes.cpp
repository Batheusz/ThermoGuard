/**
 * @file funcoes.cpp
 * @brief Implementação das funções auxiliares para envio de dados e armazenamento.
 *
 * @details
 * As funções incluem:
 * - Enviar mensagem via CallMeBot (WhatsApp)
 * - Enviar valor para ThingSpeak
 * - Salvar e carregar configurações com Preferences
 * - Conectar/desconectar Wi-Fi
 */

#include "funcoes.h"

uint8_t EnviarWhats(String mensagem) {
if (WiFi.status() == WL_CONNECTED) {
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + numCelular + "&apikey=" + apiWhats + "&text=" + urlEncode(mensagem);
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST(url);
    
    if (httpResponseCode == 200) {
    http.end();
    return 1;
    } else {
    http.end();
    return 0;
    }
}
return 2;
}

uint8_t EnviarWeb(float valor) {
if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = server + "?api_key=" + apiKey + "&field1=" + String(valor);
    http.begin(url);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode == 200) {
    http.end();
    return 1;
    } else {
    http.end();
    return 0;
    }
}
return 2;
}

uint8_t ConectaWifi(String nome, String senha) {
WiFi.begin(nome, senha);
unsigned long startTime = millis();
while(millis() - startTime < 10000)
    if (WiFi.status() == WL_CONNECTED)
        break;
if (WiFi.status() == WL_CONNECTED) {
    return 1;
} else {
    return 0;
}
}

void DesconectaWifi() {
WiFi.disconnect(true);
WiFi.mode(WIFI_OFF);
}

void SalvaConfig(String wifi, String senha, float setPoint, uint32_t intervalo, String numero) {
prefs.begin("config", false);
prefs.putString("wifi", wifi);
prefs.putString("senha", senha);
prefs.putFloat("setPoint", setPoint);
prefs.putUInt("intervalo", intervalo);
prefs.putString("cel", numero);
prefs.end();
}

void CarregarConfig() {
prefs.begin("config", true);
nomeWifi = prefs.getString("wifi", "");
senhaWifi = prefs.getString("senha", "");
setPointTemp = prefs.getFloat("setPoint", 38.0);
intervaloTemp = prefs.getUInt("intervalo", 15); // 15 min padrão
numCelular = prefs.getString("cel", "");
prefs.end();
}