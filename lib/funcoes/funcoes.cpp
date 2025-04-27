/**
 * @file funcoes.cpp
 * @brief Implementação das funções auxiliares para o projeto ThermoGuard ESP32.
 */

#include "funcoes.h"

/**
 * @brief Rotina de interrupção associada ao botão BLE.
 */
void IRAM_ATTR botaoISR() {
botaoTime = millis();
if ((botaoTime - botaoLastTime) > 50) {
    flagBle = true;
    botaoLastTime = botaoTime;
}
}

/**
 * @brief Envia mensagem WhatsApp via CallMeBot API.
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
    } else {
    http.end();
    return 0;
    }
}
return 2;
}

/**
 * @brief Envia valor ao ThingSpeak.
 */
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

/**
 * @brief Conecta o ESP32 à rede Wi-Fi.
 */
uint8_t ConectaWifi(String nome, String senha) {
WiFi.begin(nome, senha);
for (uint8_t i = 0; i < 20; i++) {
    delay(500);
}
if (WiFi.status() == WL_CONNECTED) {
    return 1;
} else {
    return 0;
}
}

/**
 * @brief Desconecta e desativa o Wi-Fi.
 */
void DesconectaWifi() {
WiFi.disconnect(true);
WiFi.mode(WIFI_OFF);
}

/**
 * @brief Salva configurações na NVS.
 */
void SalvaConfig(String wifi, String senha, float setPoint, uint32_t intervalo) {
prefs.begin("config", false);
prefs.putString("wifi", wifi);
prefs.putString("senha", senha);
prefs.putFloat("setPoint", setPoint);
prefs.putUInt("intervalo", intervalo);
prefs.end();
}

/**
 * @brief Carrega configurações da NVS.
 */
void CarregarConfig() {
prefs.begin("config", true);
nomeWifi = prefs.getString("wifi", "");
senhaWifi = prefs.getString("senha", "");
setpointTemp = prefs.getFloat("setpoint", 38.0);
intervaloTemp = prefs.getUInt("intervalo", 300000000ULL); // 5 min padrão
prefs.end();
}