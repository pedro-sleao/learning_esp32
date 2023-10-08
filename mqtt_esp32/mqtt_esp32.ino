#include <WiFi.h>
#include <PubSubClient.h>

#define POTENCIOMETRO_PIN 36

const char* ssid = "Nome da rede";
const char* password = "senha da rede";

const char* mqtt_server = "Endereco do broker MQTT";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void connect_wifi(){
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Conectando à rede...");
  }

  Serial.println("Conectado");
  Serial.println(WiFi.localIP());
}

void connect_MQTT(){
  while (!client.connected()){
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect("esp32")){
      Serial.println("Conectado ao broker");
    } else {
      Serial.println(client.state());
    }
    delay(3000);
  }
}

void setup() {
  pinMode(POTENCIOMETRO_PIN, INPUT);
  Serial.begin(9600);
  
  connect_wifi();

  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()){
    connect_MQTT();
  }

  int valorPotenciometro = analogRead(POTENCIOMETRO_PIN);

  Serial.println(valorPotenciometro);

  String valorString = String(valorPotenciometro);

  client.publish("esp_data", valorString.c_str());
  
  client.loop();

  delay(200);
}
