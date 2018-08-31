#ifndef _ASOYM_H
#define _ASOYM_H

#include "Arduino.h"

#include "wifiController.h"
//#include "mqttController.h"/
#include "mqController.h"
#include "eepromController.h"
//#include "dhtController.h"
#include "ledController.h"
#include "ntpController.h"


/*
 led   dht    led2     mq     RGB_LED
 2     16     22       35     36,39,34
*/



/*
#define DHTPIN       16
#define DHTTYPE      DHT11
DHT dht(DHTPIN, DHTTYPE);

DhtController DhtCon(&dht);
float humidity,temperature,heatindex;
DhtCon.dhtInit();
void dhtReader(){
     if(DhtCon.readDHT(&humidity,&temperature,&heatindex)){
      publishData["Humidity"] = humidity;
      publishData["Temperature"] = temperature;
      publishData["Heat index"] = heatindex;
    }
    publishData["hall"] = hallRead();
    publishData["SSID"] = WiFi.SSID();
    publishData["sensorValue"] = MqCon.readMQ();
    
    publishData.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
    MqttCon.mqttPublish(JSONmessageBuffer);
}
*/

void baseInit(){
  Serial.begin(115200);
  Serial.println();
  uint64_t chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.
  Serial.print("ESP32 SDK: ");
  Serial.println(ESP.getSdkVersion());  
}

#endif

