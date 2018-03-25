#ifndef _ASOYM_H
#define _ASOYM_H

#include "Arduino.h"
#include <math.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include "DHT.h"
#include <PubSubClient.h>

#include "wifiController.h"
#include "mqttController.h"
#include "eepromController.h"
#include "dhtController.h"
#include "mqController.h"
#include "ledController.h"

#define DHTPIN       16
#define DHTTYPE      DHT11



/*
 led   dht    led2     mq     RGB_LED
 2     16     22       35     36,39,34
*/


void baseInit();

#endif

