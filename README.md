# Projeto IoT com ESP32, MQTT, Node-RED e ThingSpeak

## Objetivo e Descrição
O nosso objetivo com a criação deste sistema é proporcionar uma solução inteligente e eficiente para o gerenciamento de energia em sua residência. Ao instalar um sensor de proximidade próximo à porta de entrada da sua casa, o sistema é capaz de identificar o momento exato em que você sai. Assim que você deixa a residência, todas as luzes são automaticamente apagadas, garantindo uma significativa redução no consumo de energia e, consequentemente, na sua conta de luz. Além disso, com um simples toque em um botão, é possível acender todas as luzes novamente quando necessário. Esta solução não só promove economia financeira, mas também contribui para a sustentabilidade ambiental.

## 📝 Sumário
1. [Introdução](#introdução)
2. [Configuração do Ambiente](#configuração-do-ambiente)
3. [Código](#código)
4. [Fluxos do Node-RED](#fluxos-do-node-red)
5. [Guia de Uso](#guia-de-uso)

## Introdução

Este projeto desenvolve um aplicativo IoT funcional integrando:
- ESP32
- Sensor DHT22 (temperatura e umidade)
- LED
- Botão
- Sensor de movimento PIR

### Funcionalidades
- Leitura de dados do DHT22 a cada 5 segundos com publicação em broker MQTT
- Controle do LED:
  - Acende com pressionar do botão
  - Apaga quando o sensor PIR detecta movimento
  - Controle remoto via MQTT através do Node-RED Dashboard
- Visualização dos dados em tempo real no Node-RED Dashboard e ThingSpeak
- Processamento e encaminhamento dos dados do MQTT para o ThingSpeak via Node-RED

## Configuração do Ambiente

### Wokwi Simulator

1. Acesse o [Wokwi Simulator](https://wokwi.com/)
2. Para importar o projeto:
   - Clique em "Start Simulation" ou "New Project"
   - Selecione "Import from GitHub" ou "Import Project"
   - Cole o conteúdo do arquivo `wokwi-project.json`
   - Clique em "Load"
3. Verifique se todos os componentes estão presentes e conectados conforme o diagrama

### Node-RED

1. Instalação:
   - Siga as instruções oficiais em [Node-RED Getting Started](https://nodered.org/docs/getting-started/)
   - Execute `node-red` no terminal
   - Acesse http://localhost:1880/

2. Instalação das paletas:
   - No Node-RED, acesse o menu → "Manage palette"
   - Vá para a aba "Install"
   - Instale `node-red-dashboard`
   - Instale outras paletas conforme necessidade

### ThingSpeak

1. Crie uma conta:
   - Acesse [ThingSpeak](https://thingspeak.com/)
   - Faça o registro

2. Configure um novo canal:
   - Vá em "Channels" → "My Channels" → "New Channel"
   - Configure:
     - Channel Name: IoT Sensor Data
     - Field 1: Temperature
     - Field 2: Humidity
   - Salve o canal

3. Obtenha a API Key:
   - No canal criado, acesse a aba "API Keys"
   - Guarde a "Write API Key" para uso no Node-RED

## Código

### Componentes Principais

#### Bibliotecas Utilizadas
- `WiFi.h`: Conexão Wi-Fi
- `PubSubClient.h`: Cliente MQTT
- `DHT.h`: Interface com sensor DHT22

#### Configurações Importantes
- **Conexão Wi-Fi**: Substitua "SEU_SSID" e "SUA_SENHA" com suas credenciais
- **MQTT**: Utiliza broker público test.mosquitto.org (porta 1883)
- **Função Callback MQTT**: Gerencia mensagens recebidas e controle do LED
- **Sensores**:
  - DHT22: Leitura a cada 5 segundos
  - Botão: Controle do LED
  - PIR: Detecção de movimento
- **Tópicos MQTT**: 
  - `sensor/temperature`
  - `sensor/humidity`

## Fluxos do Node-RED

### Arquivos Disponíveis
- `flow1.json`: Integração MQTT-ThingSpeak
- `flow2.json`: Dashboard de visualização e controle

### Como Importar

1. Acesse http://localhost:1880/
2. Menu → "Import" → "Clipboard"
3. Cole o conteúdo do arquivo de fluxo
4. Importe e configure as chaves de API necessárias

## Guia de Uso

### Wokwi Simulator

1. Carregue o projeto
2. Atualize credenciais Wi-Fi
3. Inicie simulação
4. Monitore console serial

### Dashboard Node-RED

1. Acesse http://localhost:1880/ui/
2. Funcionalidades:
   - Visualização de gráficos em tempo real
   - Controle do LED via switch
   - Monitoramento do estado no simulador

### ThingSpeak

1. Acesse seu canal
2. Visualize dados em:
   - Private View
   - Public View
3. Monitore atualizações dos gráficos

## 📌 Observações

- Mantenha as credenciais atualizadas
- Verifique conexões antes de iniciar
- Monitore o console para debug
