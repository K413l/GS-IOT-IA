#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Definição dos pinos
#define DHTPIN 15      // Pin do sensor DHT22
#define DHTTYPE DHT22
#define LEDPIN 2       // Pin do LED

// Credenciais Wi-Fi
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

// Configurações do MQTT
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* temperatureTopic = "sensor/temperature";
const char* humidityTopic = "sensor/humidity";
const char* ledTopic = "actuator/led";

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(115200);

  // Conexão Wi-Fi
  Serial.print("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado!");

  // Configuração do MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  // Inicialização do DHT22
  dht.begin();
}

void reconnect() {
  // Loop até reconectar
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println(" conectado!");
      client.subscribe(ledTopic);
    } else {
      Serial.print(" falhou, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos.");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String mensagem;
  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  Serial.print("Mensagem recebida em [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(mensagem);

  if (String(topic) == ledTopic) {
    if (mensagem == "1") {
      digitalWrite(LEDPIN, HIGH);
      Serial.println("LED ligado");
    } else {
      digitalWrite(LEDPIN, LOW);
      Serial.println("LED desligado");
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leitura do sensor
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Verificação de leituras válidas
  if (!isnan(temp) && !isnan(hum)) {
    // Publicação no MQTT
    client.publish(temperatureTopic, String(temp).c_str());
    client.publish(humidityTopic, String(hum).c_str());

    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.print(" °C | Umidade: ");
    Serial.print(hum);
    Serial.println(" %");
  } else {
    Serial.println("Falha ao ler do sensor DHT!");
  }

  delay(5000);
}
