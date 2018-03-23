#include "servoController.h"

Servo servoA;

void servoInit(){
  servoA.attach(21);
  Serial.println("servoInit success");
}

void servoHandle(int value){
  servoA.write (value);
}

