#include "mqController.h"

void mqInit(){
  if (adcAttachPin(MQ_PIN)) {
    adcStart(MQ_PIN);
    Serial.println("mqInit success");
  }
}

int readMQ(){
  return analogRead(MQ_PIN);  
}

