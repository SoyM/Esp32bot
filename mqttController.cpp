#include "mqttController.h"

static const char* mqttServer = "m13.cloudmqtt.com";
static const int mqttPort = 15873;
static const char* mqttUser = "lytlmnde";
static const char* mqttPassword = "3mtD81MmaVqW";

extern WiFiClient espClient;
PubSubClient mqttClient(espClient);

void mqttConnect() {
  unsigned char count = 0;
  Serial.println("mqttConnect");
  Serial.println(mqttServer);
  mqttClient.setServer(mqttServer, mqttPort);
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");
    if (mqttClient.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("mqtt connected");
      mqttClient.subscribe("admin");
      mqttClient.subscribe("servoAin");
    } else {
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      count++;
      if(count==5){
        ESP.restart();
      }
      delay(2000);
    }
  }
  mqttClient.setCallback(callback);
  Serial.println("mqttConnect Success");
}

void mqttReconnect() {
  if(!checkWifiConnect()){
    wifiConnect();  
  }
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    mqttConnect();
  }
}

int mqttIsConnected(){
  return(mqttClient.connected());
}

void mqttPublish(char* JSONmessageBuffer){
    if (mqttClient.publish("esp/test", JSONmessageBuffer) == true) {
      Serial.println("Success sending message");
    } else {
      Serial.println("Error sending message");
      mqttReconnect();
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
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
  else if (topic_s.equals("servoAin")) {
    unsigned int values = 0;
    Serial.println(length);

    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
      values = values + ((int)payload[i] - 48) * pow(10, (length - i - 1));
      //values = values.concat((String)payload[i]);
      //values = values+((int)payload[i]-48)*10^i;
    }
    EEPROM.write(1, values);
    Serial.println();
    EEPROM.commit();
    for (int i = 0; i < EEPROM_SIZE; i++) {
      Serial.print(byte(EEPROM.read(i))); Serial.print(" ");
    }
    JSONencoder["servoA"] = EEPROM.read(1);
    Serial.println(EEPROM.read(1));
    char JSONmessageBuffer[50];
    JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    mqttClient.publish("esp/servoAout", JSONmessageBuffer);
    Serial.println(JSONmessageBuffer);
    servoHandle(EEPROM.read(1));
  }
  Serial.println(); Serial.println("----------------------------------");
}

void mqttLoop(){
  mqttClient.loop();
}
  

