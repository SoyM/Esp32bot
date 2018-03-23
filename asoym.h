#ifndef _ASOYM_H
#define _ASOYM_H
#include "Arduino.h"
#include <math.h>
#include <ArduinoJson.h>
#include "EEPROM.h"
#include <PubSubClient.h>
#include "DHT.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <Servo.h>
#include <WiFiUdp.h>

#include "wifiController.h"
#include "mqttController.h"
#include "eepromController.h"
#include "dhtController.h"
#include "mqController.h"
#include "servoController.h"
#include "ledController.h"
/*
 led   dht    led2     mq     RGB_LED
 2     16     22       35     36,39,34
*/
#define DHTPIN       16
#define DHTTYPE      DHT11
#define EEPROM_SIZE  8
#define AP_SSID      "esp32-soym"
#define MQ_PIN              35
#define MQC_CHANNEL_7       7
#define LED_PIN                     2
#define LED_PIN2                    22
#define LEDC_CHANNEL_0              0
#define LEDC_TIMER_13_BIT          13
#define LEDC_BASE_FREQ             5000

void baseInit();

#endif

