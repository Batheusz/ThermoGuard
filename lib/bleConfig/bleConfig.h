#ifndef BLECONFIG_H
#define BLECONFIGS_H

#define SERVICE_UUID BLEUUID((uint16_t)0x1809)

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

extern volatile bool deviceConnected;

void IniciarBLE();

void EncerrarBLE();

#endif
