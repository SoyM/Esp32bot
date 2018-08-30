#ifndef _MQTTCONTROLLER_H
#define _MQTTCONTROLLER_H

#include "Arduino.h"
#include "wifiController.h"
#include "eepromController.h"
#include "ledController.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

class MqttController
{
  public:
    MqttController(PubSubClient *mqttClient);
    void mqttConnect();
    int mqttIsConnected();
    void mqttPublish(char* JSONmessageBuffer);
    void mqttLoop();
    void mqttReconnect();
    static void callback(char* topic, byte* payload, unsigned int length);
    
  private:
    PubSubClient *_mqtt_client;
    
};

#endif
