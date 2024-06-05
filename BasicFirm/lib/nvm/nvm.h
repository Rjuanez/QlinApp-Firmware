#ifndef __NVM_QLIN__
#define __NVM_QLIN__

/* DEFINITIONS */
//Definir enum con donde va cada vairable
enum dataLocation{
  hash = 0x290000,
  device_id = 0x290020U,
  wifi_ssid = 0x291000,
  wifi_password = 0x291040,
  net_address = 0x291080,
  vpn_join_code = 0x291100
};

/* VARIABLES */
//Variable para todo lo que se tenga que leer

/* FUNCTIONS */
//Funcion que inicialice las vairables
unsigned char initVariables();


#endif