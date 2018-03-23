#include "dhtController.h"

DHT dht(DHTPIN, DHTTYPE);

void dhtInit(){
  dht.begin();  
  Serial.println("dhtInit success");
}

char readDHT(float *humidity,float *temperature,float *heatindex){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.println(h);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  } else {
    *humidity = h;
    *temperature = t;
    *heatindex = dht.computeHeatIndex(*temperature, *humidity, false);
    return 1;
  }
}



