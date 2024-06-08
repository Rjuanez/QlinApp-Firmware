#include <Arduino.h>
#include <EEPROM.h>
#include "nvm.h"

char ssid[32];
char password[64];
unsigned int id;

bool initVariables() {
  char buff[33];
  uint32_t offset = hash;
  ESP.flashRead(offset, (uint32_t*)buff, sizeof(buff));
  offset = device_id;
  ESP.flashRead(offset, &id, sizeof(id));
  buff[32] = 0;
  offset = wifi_ssid;
  ESP.flashRead(offset, (uint32_t*)&ssid, sizeof(ssid));
  offset = wifi_password;
  ESP.flashRead(offset, (uint32_t*)&password, sizeof(password));

  Serial.printf("Hash: \"%s\"\n\rId: \"%i\"\n\rWifi_ssid: \"%s\"\n\rWifi_pass: \"%s\"\n\Check: \"%i\"\n\r", buff, id, &ssid, &password, ssid[0]);

  return (ssid[0] != 0xFFU); //Valor que coge memoria si se borra con flashEraseSector
}

 void writeWifiData() {
   //Borrar el sector que vamos a utilizar para las variables fijas
   ESP.flashEraseSector(0x291000 / 4096); //Se divide por el tamaño de los sectores, para sacar el numero de sector
   uint32_t offset = wifi_ssid;
   uint8_t val_ssid = ESP.flashWrite(offset,(uint32_t*)ssid, sizeof(ssid));
   offset = wifi_password;
   uint8_t val_pass = ESP.flashWrite(offset,(uint32_t*)password, sizeof(password));
   Serial.println(val_ssid+val_pass);
}
