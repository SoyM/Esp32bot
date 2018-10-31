/*
 * usage:
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

#ifndef _DHTCONTROLLER_H
#define _DHTCONTROLLER_H

#include <DHT.h>

class DhtController{
  public:
    DhtController(DHT *dht);
    void dhtInit();
    char readDHT(float *humidity,float *temperature,float *heatindex);
  private:
    uint8_t _dht_pin;
    uint8_t _dht_type;
    
    DHT *_dht;
};

#endif
