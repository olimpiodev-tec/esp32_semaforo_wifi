/**
- PubSub: https://pubsubclient.knolleary.net/api
- LiquidCrystal_I2C: https://github.com/locple/LiquidCrystal_I2C_UTF8
- DHT: https://github.com/adafruit/DHT-sensor-library
- ArduinoJson: https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
*/

#include <PubSubClient.h>
#include <WiFi.h>

const char* SSID = "AAPM-510";
const char* PASSWORD = "aapmSenai#";

const char* MQTT_HOST = "broker.hivemq.com";
const int MQTT_PORT = 1883;

const char* MQTT_TOPIC_SEMAFORO_LIGAR = "semaforo/ligar";
const char* MQTT_TOPIC_SEMAFORO_DESLIGAR = "semaforo/desligar";

const char* MQTT_TOPIC_SEMAFORO_VERMELHO_LIGAR = "semaforo/2/dev/ligar/faixa/vermelho";

const int PINO_LED_VERMELHO = 21;
const int PINO_LED_AMARELO = 22;
const int PINO_LED_VERDE = 23;

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
  client.subscribe(MQTT_TOPIC_SEMAFORO_DESLIGAR);
}

void callback(char* topic, byte* payload, unsigned int length) {
  /*if (strcmp(topic, "semaforo/ligar") == 0) {
    showText("    Semaforo    ", "     Ligado     ", false);
    toggleSemaforo(1);
  } else if (strcmp(topic, "semaforo/desligar") == 0) {
    showText("    Semaforo    ", "   Desligado    ", false);
    toggleSemaforo(0);
  }*/

  if (strcmp(topic, "semaforo/2/dev/ligar/faixa/vermelho") == 0) {
    digitalWrite(PINO_LED_VERMELHO, HIGH);
  }
  Serial.println(topic);
}


void setup() {
  Serial.begin(9600);

  pinMode(PINO_LED_VERMELHO, OUTPUT);
  pinMode(PINO_LED_AMARELO, OUTPUT);
  pinMode(PINO_LED_VERDE, OUTPUT);

  Serial.println(" Inicializando Aguarde");

  conectarWifi();
  conectarMQTT();
}

void loop() {
  client.loop();
}