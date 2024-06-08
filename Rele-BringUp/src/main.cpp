#include <Arduino.h>
#include <EEPROM.h>


void setup() {
  //borrar los sectores a utilizar
  Serial.begin(115200);    

  // for (int i = 1; i < 20; ++i) {
  //   pinMode(i, OUTPUT);
    
  // }
  pinMode(5, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(14, INPUT);
  digitalWrite(5, LOW);
}


void loop() {
   if(Serial.available() > 1) {
     int val = Serial.parseInt();
      if (val == 1) digitalWrite(5, HIGH);
      else if (val == 2) digitalWrite(5, LOW);
      else if (val == 3) digitalWrite(12, HIGH);
      else if (val == 4) digitalWrite(12, LOW);
     //digitalWrite(val, HIGH);
  }
  Serial.println(digitalRead(14));
  delay(100);

  
}


