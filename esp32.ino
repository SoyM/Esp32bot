#include "asoym.h"

TaskHandle_t mainTaskHandle,realTaskHandle;
StaticJsonBuffer<300> JSONbuffer;
JsonObject& publishData = JSONbuffer.createObject();


DHT dht(DHTPIN, DHTTYPE);
DhtController DhtCon(&dht);

WiFiMulti wifiMulti;
WifiController WifiCon(&wifiMulti);

WiFiClient espClient;
PubSubClient mqttClient(espClient);
MqttController MqttCon(&mqttClient);
MqController MqCon;

void setup() {
  baseInit();
  eepromInit();
  WifiCon.wifiConnect();
  MqttCon.mqttConnect();
  MqCon.mqInit();
  DhtCon.dhtInit();
  boardLedInit();
  xTaskCreate(realTask, "realTask", 5000, NULL, 1, &realTaskHandle);
//  xTaskCreate(mainTask, "mainTask", 5000, NULL, 0, &mainTaskHandle);
//  xTaskCreate(servoTask, "servoTask", 10000, NULL, 1, &servoTaskHandle);
//  vTaskStartScheduler(); 
}

void loop() {
    Serial.println("mainTask Start");
    //ulTaskNotifyTake( pdTRUE, portMAX_DELAY); 
    float humidity,temperature,heatindex;
   
    if(DhtCon.readDHT(&humidity,&temperature,&heatindex)){
      publishData["Humidity"] = humidity;
      publishData["Temperature"] = temperature;
      publishData["Heat index"] = heatindex;
    }
    publishData["hall"] = hallRead();
    publishData["SSID"] = WiFi.SSID();
    publishData["sensorValue"] = MqCon.readMQ();
    
    char JSONmessageBuffer[300];
    publishData.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
    MqttCon.mqttPublish(JSONmessageBuffer);
    //client.loop();
    
    Serial.println("--------------------------");
    delay(2000);
}

void realTask(void* parameter) {
  while (1) {
    MqttCon.mqttLoop();
    ledFlash();
//  xTaskNotifyGive( mainTaskHandle ); 
//    vTaskDelete(NULL);   
  }
}



