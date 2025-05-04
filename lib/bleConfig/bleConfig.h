/**
 * @file bleConfig.h
 * @brief Declarações relacionadas à configuração do BLE no ESP32.
 *
 * @details
 * Define UUIDs, variáveis globais e funções públicas para controle BLE.
 * Contém os protótipos das funções IniciarBLE() e EncerrarBLE().
 */

#ifndef BLECONFIG_H
#define BLECONFIGS_H

#define SERVICE_UUID BLEUUID((uint16_t)0x1809)
#define SERVICE_WIFI_UUID "7be95774-8734-4f46-a326-573d290c1e9f"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

extern volatile bool deviceConnected;

/**
 * @brief Inicializa o serviço BLE com todas as características e inicia o advertising.
 */
void IniciarBLE();

/**
 * @brief Encerra a comunicação BLE e libera recursos.
 */
void EncerrarBLE();

#endif
