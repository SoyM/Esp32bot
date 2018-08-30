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


