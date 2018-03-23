#ifndef _MQTTCONTROLLER_H
#define _MQTTCONTROLLER_H

#include "asoym.h"

void mqttConnect();
int mqttIsConnected();
void mqttPublish(char* JSONmessageBuffer);
void callback(char* topic, byte* payload, unsigned int length);
void mqttLoop();

#endif
