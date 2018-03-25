#include "dhtController.h"

DhtController::DhtController(DHT *dht){
  _dht = dht;
}

void DhtController::dhtInit(){
  _dht->begin();  
  Serial.println("dhtInit success");
}

char DhtController::readDHT(float *humidity,float *temperature,float *heatindex){
  float h = _dht->readHumidity();
  float t = _dht->readTemperature();
  Serial.println(h);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  } else {
    *humidity = h;
    *temperature = t;
    *heatindex = _dht->computeHeatIndex(*temperature, *humidity, false);
    return 1;
  }
}




