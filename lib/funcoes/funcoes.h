/**
 * @file funcoes.h
 * @brief Declaração das funções auxiliares para comunicação Wi-Fi e armazenamento.
 *
 * @details
 * Contém funções para:
 * - Conexão e desconexão do Wi-Fi
 * - Envio de dados via HTTP
 * - Armazenamento e carregamento de configurações na NVS
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
extern float setPointTemp;
extern uint32_t intervaloTemp;
extern Preferences prefs;

/**
 * @brief Envia uma mensagem via WhatsApp usando CallMeBot API.
 * 
 * @param mensagem Texto da mensagem a ser enviada.
 * @return uint8_t
 * @retval 1 Sucesso (HTTP 200)
 * @retval 0 Falha na requisição
 * @retval 2 Wi-Fi não conectado
 */
uint8_t EnviarWhats(String mensagem);

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
 * @param nome SSID da rede.
 * @param senha Senha da rede.
 * @return uint8_t
 * @retval 1 Conectado com sucesso
 * @retval 0 Falha na conexão
 */
uint8_t ConectaWifi(String nome, String senha);

/**
 * @brief Desconecta o ESP32 da rede Wi-Fi e desativa o Wi-Fi.
 */
void DesconectaWifi();

/**
 * @brief Salva as configurações fornecidas na memória não volátil (NVS).
 *
 * @param wifi Nome da rede Wi-Fi.
 * @param senha Senha da rede.
 * @param setPoint Temperatura alvo de referência.
 * @param intervalo Intervalo entre medições (em minutos).
 * @param numero Número de telefone para alertas.
 */
void SalvaConfig(String wifi, String senha, float setPoint, uint32_t intervalo, String numero);

/**
 * @brief Carrega as configurações armazenadas na NVS.
 */
void CarregarConfig();

#endif // FUNCOES_H