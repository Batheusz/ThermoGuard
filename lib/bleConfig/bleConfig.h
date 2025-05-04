/**
 * @file bleConfig.h
 * @brief Declarações para configuração do Bluetooth Low Energy (BLE).
 *
 * @details 
 * Define o UUID do serviço BLE e declara as funções públicas para
 * iniciar e encerrar o BLE.
 * 
 * @author
 * Matheus Orsini
 * @date
 * 2025-04-26
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

void IniciarBLE();

void EncerrarBLE();

#endif
