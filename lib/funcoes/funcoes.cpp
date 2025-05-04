/**
 * @file funcoes.cpp
 * @brief Implementação das funções auxiliares para o projeto ThermoGuard ESP32.
 */

#include "funcoes.h"

/**
 * @brief Envia mensagem WhatsApp via CallMeBot API.
 */
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
void SalvaConfig(String wifi, String senha, float setPoint, uint32_t intervalo, String numero) {
prefs.begin("config", false);
prefs.putString("wifi", wifi);
prefs.putString("senha", senha);
prefs.putFloat("setPoint", setPoint);
prefs.putUInt("intervalo", intervalo);
prefs.putString("cel", numero);
prefs.end();
}

/**
 * @brief Carrega configurações armazenadas na memória NVS.
 * 
 * Lê o nome do Wi-Fi, senha, setpoint de temperatura e intervalo
 * de leitura, restaurando os valores padrão caso ainda não existam.
 */
void CarregarConfig() {
prefs.begin("config", true);
nomeWifi = prefs.getString("wifi", "");
senhaWifi = prefs.getString("senha", "");
setPointTemp = prefs.getFloat("setPoint", 38.0);
intervaloTemp = prefs.getUInt("intervalo", 15); // 15 min padrão
numCelular = prefs.getString("cel", "");
prefs.end();
}