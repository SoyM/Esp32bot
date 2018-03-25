#ifndef _MQCONTROLLER_H
#define _MQCONTROLLER_H

#include "asoym.h"

#define MQ_PIN              35
#define MQC_CHANNEL_7       7

class MqController
{
  public:
    void mqInit();
    int readMQ();
};

#endif

