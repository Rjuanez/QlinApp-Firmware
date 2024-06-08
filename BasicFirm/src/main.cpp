#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <WiFi.h>
#include <Husarnet.h>
#include <WebServer.h>
#include "nvm.h"


BLECharacteristic *pCharacteristic;
String comando = "";
String cmd = "";
volatile bool cmd_recived = false;
volatile bool setup_done = false;
unsigned char setUpIndex = 0U;
String provisional_ssid = "";
String provisional_pass = "";

/* Wifi Variables */
// char ssid[32] = "";
// char password[64] = "";

/* Server variables */
WebServer server(80);
const char *hostName = "esp32-webserver";
const char *husarnetJoinCode = "fc94:b01d:1803:8dd8:b293:5c7d:7639:932a/aqZ3kYukBCCi4FHDmngcJT";

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Nuevo dispositivo conectado");
  }

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Desconectado !!!!");
    pServer->startAdvertising();
  }
};

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    Serial.print("Msg recived: ");
    Serial.println(value.c_str());
    comando = String(value.c_str());
    int inicio = comando.indexOf('!');
    if(inicio != -1){
      int fin = comando.indexOf('$' ,inicio);
      if(fin != -1){
        cmd = comando.substring(inicio+1, fin);
      }
    }
    if (cmd == "Test") {
      digitalWrite(12, HIGH);
      delay(2000);
      digitalWrite(12, LOW);
    }else if (cmd[0] == 'S') {
      provisional_ssid.concat(cmd.substring(1, cmd.length()));
      Serial.println(provisional_ssid);
    }else if (cmd[0] == 'P') {
      provisional_pass.concat(cmd.substring(1, cmd.length()));
      Serial.println(provisional_pass);
    }else if (cmd[0] == 'U') {
      strcpy(ssid, provisional_ssid.c_str());
      strcpy(password, provisional_pass.c_str());
      writeWifiData();
      setup_done = true;
    }
      cmd = "";
  }
};

void initBLE() {
  char buff[32];
  sprintf(buff, "Box%d", id);
  BLEDevice::init(buff);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(BLEUUID("4fafc201-1fb5-459e-8fvv-c5c9c331914b"));

  
  pCharacteristic = pService->createCharacteristic(
                                         BLEUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"),
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  
  //  Anunciar el servicio
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  Serial.println("Esperando conexion BLE...");
}

void initWifi() {
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("waiting for Wi-Fi...");
  }
  Serial.print("IP adquired: ");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  String message = "hello from " + id;
  server.send(200, "text/plain", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void initServer() {
  char buff[12];
  sprintf(buff, "box%d", id);
  Husarnet.join(husarnetJoinCode, buff);
  Husarnet.start();

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/turnon", []() {
    digitalWrite(5, HIGH);
    server.send(200, "text/plain", "on");
  });

  server.on("/turnoff", []() {
    digitalWrite(5, LOW);
    server.send(200, "text/plain", "off");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void setup() {
  pinMode(5, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  Serial.begin(115200);
  digitalWrite(12, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  delay(100);
  digitalWrite(12, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  //Solo se hara si no esta inicializada
  if (!initVariables()) {
    initBLE();
    while (!setup_done);
    BLEDevice::deinit();
    abort();
  }else {
    initWifi();
    initServer();
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  delay(20);
}


