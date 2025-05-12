/**
- PubSub: https://pubsubclient.knolleary.net/api
- LiquidCrystal_I2C: https://github.com/locple/LiquidCrystal_I2C_UTF8
- DHT: https://github.com/adafruit/DHT-sensor-library
- ArduinoJson: https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
*/

#include <PubSubClient.h>
#include <WiFi.h>

const char* SSID = "moto g22_4827";
const char* PASSWORD = "caroline";

const char* MQTT_HOST = "broker.hivemq.com";
const int MQTT_PORT = 1883;

const char* MQTT_TOPIC_SEMAFORO_LIGAR = "semaforo/2/dev/ligar";
const char* MQTT_TOPIC_SEMAFORO_DESLIGAR = "semaforo/2/dev/desligar";

const char* MQTT_TOPIC_SEMAFORO_VERMELHO_LIGAR = "semaforo/2/dev/ligar/faixa/vermelho";
const char* MQTT_TOPIC_SEMAFORO_AMARELO_LIGAR = "semaforo/2/dev/ligar/faixa/amarelo";
const char* MQTT_TOPIC_SEMAFORO_VERDE_LIGAR = "semaforo/2/dev/ligar/faixa/verde";

const int PINO_LED_VERMELHO = 21;
const int PINO_LED_AMARELO = 22;
const int PINO_LED_VERDE = 23;
const int PINO_LED_CONEXAO = 18;

WiFiClient espClient;
PubSubClient client(espClient);

void conectarWifi() {
  WiFi.begin(SSID, PASSWORD);
  Serial.println("Conectando WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  String ip = " " + WiFi.localIP().toString();
  Serial.println("Conectado WiFi");
  Serial.println(ip.c_str());
}

void conectarMQTT() {
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);

  while (!client.connected()) {
    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado broker MQTT HiveMQ");
    } else {
      String erroMQTT = "Codigo erro: " + String(client.state());
      Serial.println("Broker MQTT falhou");
      Serial.println(erroMQTT.c_str());
    }
  }

  client.subscribe(MQTT_TOPIC_SEMAFORO_VERMELHO_LIGAR);
  client.subscribe(MQTT_TOPIC_SEMAFORO_AMARELO_LIGAR);
  client.subscribe(MQTT_TOPIC_SEMAFORO_VERDE_LIGAR);

  client.subscribe(MQTT_TOPIC_SEMAFORO_DESLIGAR);
  digitalWrite(PINO_LED_CONEXAO, HIGH);
}

void callback(char* topic, byte* payload, unsigned int length) {

  if (strcmp(topic, "semaforo/2/dev/desligar") == 0) 
  {
    Serial.println("Desligando semáforo");
    digitalWrite(PINO_LED_VERMELHO, LOW);
    digitalWrite(PINO_LED_AMARELO, LOW);
    digitalWrite(PINO_LED_VERDE, LOW);
  } 
  else if (strcmp(topic, "semaforo/2/dev/ligar") == 0) 
  {
    Serial.println("Ligando semáforo");
    digitalWrite(PINO_LED_VERMELHO, HIGH);
    digitalWrite(PINO_LED_AMARELO, HIGH);
    digitalWrite(PINO_LED_VERDE, HIGH);
  }
  else if (strcmp(topic, "semaforo/2/dev/ligar/faixa/vermelho") == 0) 
  {
    digitalWrite(PINO_LED_VERMELHO, HIGH);
  } 
  else if (strcmp(topic, "semaforo/2/dev/ligar/faixa/amarelo") == 0) 
  {
    digitalWrite(PINO_LED_AMARELO, HIGH);
  }
  else if (strcmp(topic, "semaforo/2/dev/ligar/faixa/verde") == 0) 
  {
    digitalWrite(PINO_LED_VERDE, HIGH);
  }
  Serial.println(topic);
}


void setup() {
  Serial.begin(9600);

  pinMode(PINO_LED_VERMELHO, OUTPUT);
  pinMode(PINO_LED_AMARELO, OUTPUT);
  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_CONEXAO, OUTPUT);

  digitalWrite(PINO_LED_CONEXAO, LOW);

  Serial.println(" Inicializando Aguarde");

  conectarWifi();
  conectarMQTT();
}

void loop() {
  client.loop();
}