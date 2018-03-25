#include "mqttController.h"

static const char* mqttServer = "m13.cloudmqtt.com";
static const int mqttPort = 15873;
static const char* mqttUser = "lytlmnde";
static const char* mqttPassword = "3mtD81MmaVqW";

MqttController::MqttController(PubSubClient *mqtt_client){
  _mqtt_client = mqtt_client;
}

void MqttController::mqttConnect() {
  unsigned char count = 0;
  Serial.println("mqttConnect");
  Serial.println(mqttServer);
  _mqtt_client->setServer(mqttServer, mqttPort);
  while (!_mqtt_client->connected()) {
    Serial.println("Connecting to MQTT...");
    if (_mqtt_client->connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("mqtt connected");
      _mqtt_client->subscribe("admin");
      _mqtt_client->subscribe("servoAin");
    } else {
      Serial.print("failed with state ");
      Serial.print(_mqtt_client->state());
      count++;
      if(count==5){
        ESP.restart();
      }
      delay(2000);
    }
  }
  _mqtt_client->setCallback(callback);
  Serial.println("mqttConnect Success");
}

void MqttController::mqttReconnect() {
  if(!WifiController::checkWifiConnect()){
    WifiController::wifiConnect();  
  }
  while (!_mqtt_client->connected()) {
    Serial.print("Attempting MQTT connection...");
    mqttConnect();
  }
}

int MqttController::mqttIsConnected(){
  return(_mqtt_client->connected());
}

void MqttController::mqttPublish(char* JSONmessageBuffer){
    if (_mqtt_client->publish("esp/test", JSONmessageBuffer) == true) {
      Serial.println("Success sending message");
    } else {
      Serial.println("Error sending message");
      mqttReconnect();
    }
}

void MqttController::callback(char* topic, byte* payload, unsigned int length) {
  WiFiClient espClient;
  PubSubClient mqttClient(espClient);
  StaticJsonBuffer<50> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String topic_s = topic;
  if (topic_s.equals("admin")) {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
      EEPROM.write(0, (int)payload[i] - 48);
    }
    Serial.println();
    EEPROM.commit();
    for (int i = 0; i < EEPROM_SIZE; i++) {
      Serial.print(byte(EEPROM.read(i))); Serial.print(" ");
    }
    JSONencoder["led"] = EEPROM.read(0);
    Serial.println(EEPROM.read(0));
    char JSONmessageBuffer[50];
    JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

    mqttClient.publish("esp/led", JSONmessageBuffer);
    Serial.println(JSONmessageBuffer);
    digitalWrite(LED_PIN2, -(EEPROM.read(0)) + 1);
  } 
  Serial.println(); Serial.println("----------------------------------");
}

void MqttController::mqttLoop(){
  _mqtt_client->loop();
}
  

