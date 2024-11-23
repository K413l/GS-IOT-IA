#Projeto IoT com ESP32, MQTT, Node-RED e ThingSpeak

##1. Introdução

Descrição geral do projeto e seus objetivos
Este projeto tem como objetivo desenvolver um aplicativo IoT funcional que integra um ESP32, um sensor DHT22 (temperatura e umidade), um LED, um botão e um sensor de movimento PIR. O sistema permite:

Leitura de dados do DHT22 a cada 5 segundos, publicando temperatura e umidade em um broker MQTT.
Controle do LED:
Acender quando o botão é pressionado.
Apagar quando o sensor PIR detecta movimento.
Controle remoto via MQTT através do Node-RED Dashboard.
Visualização dos dados em tempo real no Node-RED Dashboard e no ThingSpeak.
Processamento e encaminhamento dos dados do MQTT para o ThingSpeak utilizando o Node-RED.

##2. Configuração do Ambiente
Instruções para configurar o Wokwi
Acesse o Wokwi Simulator.
Clique em "Start Simulation" ou "New Project".
Importe o projeto usando o diagrama fornecido:
Clique em "Import Project".
Cole o conteúdo do arquivo wokwi-project.json na caixa de diálogo.
Clique em "Load" para carregar o projeto.
Verifique se todos os componentes (ESP32, DHT22, LED, botão e sensor PIR) estão presentes e conectados conforme o diagrama.
Instalação do Node-RED e paletas necessárias
Instalação do Node-RED:
Siga as instruções oficiais em Node-RED Getting Started.
Iniciando o Node-RED:
Execute o comando node-red no terminal.
Acesse o Node-RED em http://localhost:1880/.
Instalação de Paletas Necessárias:
No Node-RED, clique no menu no canto superior direito e selecione "Manage palette".
Vá para a aba "Install".
Pesquise por node-red-dashboard e clique em "Install".
Opcionalmente, instale outras paletas conforme necessário para o seu projeto.
Passos para configurar o ThingSpeak
Criar uma conta:
Acesse ThingSpeak e registre-se.
Criar um novo canal:
Após o login, vá em "Channels" > "My Channels" > "New Channel".
Preencha os detalhes:
Channel Name: IoT Sensor Data
Field 1: Temperature
Field 2: Humidity
Clique em "Save Channel".
Obter a chave da API:
No canal criado, vá para a aba "API Keys".
Anote a "Write API Key", que será usada no Node-RED.

##3. Código
Código completo do ESP32
O código do ESP32 está disponível no arquivo esp32-code.ino.

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Definição dos pinos
#define DHTPIN 15      // Pino do sensor DHT22
#define DHTTYPE DHT22
#define LEDPIN 2       // Pino do LED
#define BUTTON_PIN 4   // Pino do Botão
#define PIR_PIN 5      // Pino do Sensor PIR

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
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(PIR_PIN, INPUT);
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
      Serial.println("LED ligado via MQTT");
    } else {
      digitalWrite(LEDPIN, LOW);
      Serial.println("LED desligado via MQTT");
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leitura do Botão
  if (digitalRead(BUTTON_PIN) == HIGH) {
    digitalWrite(LEDPIN, HIGH); // Liga o LED
    Serial.println("LED ligado pelo botão");
  }

  // Leitura do Sensor PIR
  if (digitalRead(PIR_PIN) == HIGH) {
    digitalWrite(LEDPIN, LOW); // Desliga o LED
    Serial.println("LED desligado pelo sensor PIR");
  }

  // Leitura do sensor DHT22
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
Comentários e explicações importantes
Bibliotecas Utilizadas:
WiFi.h: Para conexão Wi-Fi.
PubSubClient.h: Cliente MQTT.
DHT.h: Para interagir com o sensor DHT22.
Conexão Wi-Fi:
Substitua "SEU_SSID" e "SUA_SENHA" pelas credenciais da sua rede Wi-Fi.
Configuração MQTT:
Utiliza o broker público test.mosquitto.org na porta 1883.
Função callback:
Recebe mensagens MQTT e controla o LED conforme o comando recebido.
Leitura dos Sensores:
DHT22: Lê temperatura e umidade a cada 5 segundos e publica no MQTT.
Botão: Liga o LED quando pressionado.
Sensor PIR: Desliga o LED ao detectar movimento.
Publicação dos Dados:
Os dados de temperatura e umidade são publicados nos tópicos sensor/temperature e sensor/humidity.

##4. Fluxos do Node-RED
Arquivos dos fluxos exportados (.json)
Fluxo 1: flow1.json - Recebe dados do MQTT e envia para o ThingSpeak.
Fluxo 2: flow2.json - Dashboard para visualizar dados e controlar o LED.
Instruções sobre como importar os fluxos no Node-RED
Abra o Node-RED em http://localhost:1880/.
Clique no menu no canto superior direito e selecione "Import".
Escolha a opção "Clipboard".
Cole o conteúdo do arquivo flow1.json ou flow2.json.
Clique em "Import" para adicionar o fluxo.
Repita o processo para ambos os fluxos.
Após importar, atualize as configurações necessárias, como chaves de API e tópicos MQTT.

##5. Guia de Uso
Como executar o simulador
No Wokwi, certifique-se de que o projeto está corretamente carregado.
Atualize o código do ESP32 com suas credenciais Wi-Fi.
Clique em "Start Simulation".
Monitore o console serial para verificar as mensagens de debug.
Como interagir com o dashboard do Node-RED
Certifique-se de que o Node-RED está em execução e que os fluxos foram importados.
Acesse o dashboard em http://localhost:1880/ui/.
Visualize os gráficos de Temperatura e Umidade.
Utilize o switch para ligar ou desligar o LED via MQTT.
Observe a mudança do estado do LED no simulador Wokwi.
Como verificar os dados no ThingSpeak
Acesse seu canal no ThingSpeak.
Vá para a aba "Private View".
Os gráficos de Temperatura e Umidade devem estar atualizando com os dados enviados pelo Node-RED.
