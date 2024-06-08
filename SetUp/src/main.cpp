#include <Arduino.h>
#include <EEPROM.h>

/*    DEFINES     */
#define EEPROM_SIZE 12
#define HASH_LENGTH 32  

/*    GLOBAL VARIABLES     */
const uint32_t NVM_Offset = 0x290000;  // Offset Value For NVS Partition

/*    VARIABLES A LEER DE SERIAL    */
char hash[HASH_LENGTH] = {0,}; //32 de la variale de hash
uint32_t id = 0U;
char type_wr;


void setup() {
  //borrar los sectores a utilizar
  Serial.begin(115200);
  ESP.flashEraseSector(0x291000 / 4096);
  // /* LECTURA DE SERIAL  */
  
  // //Esperar a que lleguen los dos datos
  // while (Serial.available() <= 0);
  // type_wr = Serial.read();
 
  // if (type_wr == 'W') {
  //   Serial.readBytes(hash, sizeof(hash)); 
  //   id = Serial.parseInt();
  // }

  // /* ESCRITURA DE MEMORIA */
  // if (type_wr == 'W') {
  // //Borrar el sector que vamos a utilizar para las variables fijas
  // ESP.flashEraseSector(NVM_Offset / 4096); //Se divide por el tamaño de los sectores, para sacar el numero de sector
  
  // uint8_t val_hash = ESP.flashWrite(NVM_Offset,(uint32_t*)hash, sizeof(hash));
  // uint8_t val_id = ESP.flashWrite(NVM_Offset+32, &id, sizeof(id));

  // Serial.printf("%i\n",val_hash+val_id);
  // //Serial.printf("Hash escrito: \"%i\"\nId escrito: \"%i\"\n", val_hash, val_id);
  
  // } else {
  //   //Added an extra space, so it is displayed correctly through the serial
  //   char buff[33];
  //   ESP.flashRead(NVM_Offset,(uint32_t*)buff, sizeof(buff));
  //   ESP.flashRead(NVM_Offset+32, &id, sizeof(id));
  //   buff[32] = 0;
  //   Serial.printf("Hash: \"%s\"\nId: \"%i\"\n", buff, id);
  // }
}


void loop() {}


