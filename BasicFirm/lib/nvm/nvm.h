#ifndef __NVM_QLIN__
#define __NVM_QLIN__

/* DEFINITIONS */
//Definir enum con donde va cada vairable

#define hash 0x290000U;
#define device_id 0x290020U;
#define wifi_ssid 0x291000;
#define wifi_password 0x291040;
#define net_address 0x291080;
#define vpn_join_code 0x291100;


/* VARIABLES */
//Variable para todo lo que se tenga que leer

extern char ssid[32];
extern char password[64];
extern unsigned int id;

/* FUNCTIONS */
//Funcion que inicialice las vairables
bool initVariables();
void writeWifiData();


#endif