#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HX711.h>


//SSID e senha para se conectar à internet
#define WIFI_SSID "LENOVO-PC"
#define WIFI_PASSWORD "lenovopc"

// MQTT CONFIGs
#define BROKER_URL "mqtt.tago.io"
#define BROKER_TOKEN "aff30dba-aa54-4a2a-82ea-68ce0acad533"
#define BROKER_CLIENT "ESP32"
#define BROKER_ID "esp32"

// DEVICE'S MAC
#define DEVICE_MAC "98CDAC2EE8EC"

// VARIABLE AREA HX711
#define LOADCELL_DOUT_PIN 34
#define LOADCELL_SCK_PIN 35

float medida = 0.00;
float calibration_factor = 34730;
HX711 scale; // offset = -8388495

typedef struct{
  char mac[14];
  char sensor[5];
  float value;
} struct_message;

struct_message message;

void setupWiFi(){
  
  WiFi.mode(WIFI_STA);

  //Mandamos conectar na rede
  WiFi.begin(WIF_SSID, WIFI_PASSWORD);
  Serial.println("");

  //Enquanto não estiver conectado à rede WiFi
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  //Se chegou aqui está conectado
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  
  Serial.print("macAddress: "); 
  Serial.println(WiFi.macAddress());
}

void connectToMQTTServer() {
    
  Serial.println("Connecting to MQTT Server...");
  //Se conecta ao id que definimos
  if (client.connect(BROKER_ID, BROKER_CLIENT, BROKER_TOKEN)) {
    //Se a conexão foi bem sucedida
    
    client.publish("98CDAC2EE8EC/status", "connected"); // You can activate the retain flag by setting the third parameter to true
    Serial.println("connected");
  } else {
    //Se ocorreu algum erro
    Serial.print("error = ");
    Serial.println(client.state());
  }
}

void sendToMQTT() {

  char topic[30] = "\0";
  Serial.println(DEVICE_MAC);
  Serial.println(message.mac);
  Serial.println(message.sensor);
  sprintf(topic, "%s/%s", message.mac, message.sensor);
  client.publish(topic, String(message.value).c_str());
}

void setupHX711(){

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // CONFIGURANDO OS PINOS DA BALANÇA
  scale.set_scale(); // LIMPANDO O VALOR DA ESCALA

  delay(500);
  scale.tare(); // ZERANDO A BALANÇA PARA DESCONSIDERAR A MASSA DA ESTRUTURA

  Serial.println("Balança Zerada");
}

void setup() {
  Serial.begin(115200);

  strncpy(message.mac, "98CDAC2EE8EC", 13);
  strncpy(message.sensor, "peso", 5);

  setupWiFi();

  setupHX711();

  connectToMQTTServer();
}

void loop(){
  
  Serial.print("Peso: ");       
  medida = scale.get_units(); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
  Serial.println(scale.get_units(2), 3); // ENVIANDO PARA MONITOR SERIAL A MEDIDA COM 3 CASAS DECIMAIS  
  Serial.print(" kg");
  
  message.value = medida;

  sendToMQTT();
  
  ESP.deepSleep(5e6);
}
