#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <WiFi.h>
#include <Husarnet.h>
#include <WebServer.h>


BLECharacteristic *pCharacteristic;
String comando = "";
String cmd = "";
volatile bool cmd_recived = false;
volatile bool setup_done = false;
unsigned char setUpIndex = 0U;

/* Wifi Variables */
char ssid[32] = "Rub";
char password[64] = "pepinopepino";

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
    } else if (cmd.substring(0,5) = "Update"){ //!Update<ssid>;<password>$
        int separador_wifi_ssid = cmd.indexOf(';' ,0);
        strcpy(ssid, cmd.substring(6, separador_wifi_ssid).c_str());
        int separador_wifi_pass = cmd.indexOf(';' ,separador_wifi_ssid+1);
        strcpy(password, cmd.substring(separador_wifi_ssid+1, separador_wifi_pass).c_str());
        setup_done = true;
    } 
      
        //Extraer datos e imprimirlos:
          //- widi_ssid
          //- wifi_password
      cmd = "";
  }
};

void initBLE() {
  BLEDevice::init("ESP32_BLE_Test");
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
  server.send(200, "text/plain", "hello from esp32!");
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
  Husarnet.join(husarnetJoinCode, hostName);
  Husarnet.start();

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void setup() {
  pinMode(12, OUTPUT);

  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  digitalWrite(12, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  delay(100);
  digitalWrite(12, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  //Solo se hara si no esta inicializada
  initBLE();
  while (!setup_done);
  BLEDevice::deinit();
  initWifi();
  initServer();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  delay(20);
}


