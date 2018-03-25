#include "mqController.h"

void MqController::mqInit(){
  if (adcAttachPin(MQ_PIN)) {
    adcStart(MQ_PIN);
    Serial.println("mqInit success");
  }
}

int MqController::readMQ(){
  return analogRead(MQ_PIN);  
}

