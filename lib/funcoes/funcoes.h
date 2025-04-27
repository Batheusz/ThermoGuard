/**
 * @file funcoes.h
 * @brief Declarações de funções auxiliares para o projeto ThermoGuard ESP32.
 *
 * @details
 * Contém as funções para:
 * - Envio de dados via HTTP para WhatsApp e ThingSpeak
 * - Gerenciamento da conexão Wi-Fi
 * - Armazenamento e recuperação de configurações na NVS
 * - Tratamento de interrupções para BLE
 * 
 * @author Matheus Orsini
 * @date 2025-04-26
 */

#ifndef FUNCOES_H
#define FUNCOES_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <Preferences.h>


extern String numCelular;
extern String apiWhats;
extern String nomeWifi;
extern String senhaWifi;
extern String apiKey;
extern String server;
extern float setpointTemp;
extern uint32_t intervaloTemp;
extern Preferences prefs;
extern volatile bool flagBle;
extern volatile uint32_t botaoTime;
extern volatile uint32_t botaoLastTime;

/**
 * @brief Rotina de interrupção associada ao botão BLE.
 * 
 * @details Implementa debounce para evitar múltiplos acionamentos.
 */
void IRAM_ATTR botaoISR();

/**
 * @brief Envia uma mensagem via WhatsApp usando CallMeBot API.
 * 
 * @param messagem Texto da mensagem a ser enviada.
 * @return uint8_t
 * @retval 1 Sucesso (HTTP 200)
 * @retval 0 Falha na requisição
 * @retval 2 Wi-Fi não conectado
 */
uint8_t EnviarWhats(String messagem);

/**
 * @brief Envia um valor numérico para ThingSpeak via HTTP GET.
 * 
 * @param valor Valor float a ser enviado.
 * @return uint8_t
 * @retval 1 Sucesso (HTTP 200)
 * @retval 0 Falha na requisição
 * @retval 2 Wi-Fi não conectado
 */
uint8_t EnviarWeb(float valor);

/**
 * @brief Conecta o ESP32 à rede Wi-Fi especificada.
 * 
 * @param nome Nome (SSID) da rede Wi-Fi.
 * @param senha Senha da rede Wi-Fi.
 * @return uint8_t
 * @retval 1 Sucesso
 * @retval 0 Falha
 */
uint8_t ConectaWifi(String nome, String senha);

/**
 * @brief Desconecta o ESP32 da rede Wi-Fi e desativa o Wi-Fi.
 */
void DesconectaWifi();

/**
 * @brief Salva as configurações recebidas (Wi-Fi, senha, setpoint e intervalo) na NVS.
 * 
 * @param wifi Nome da rede Wi-Fi.
 * @param senha Senha da rede Wi-Fi.
 * @param setPoint Valor do setpoint de temperatura.
 * @param intervalo Intervalo de tempo entre leituras (em microssegundos).
 */
void SalvaConfig(String wifi, String senha, float setPoint, uint32_t intervalo);

/**
 * @brief Carrega as configurações armazenadas na NVS.
 */
void CarregarConfig();

#endif // FUNCOES_H