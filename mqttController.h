#ifndef _MQTTCONTROLLER_H
#define _MQTTCONTROLLER_H

#include "asoym.h"

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
