#include "asoym.h"

WiFiClient espClient;
TaskHandle_t mainTaskHandle,realTaskHandle;
StaticJsonBuffer<300> JSONbuffer;
JsonObject& publishData = JSONbuffer.createObject();
WiFiUDP Udp;

const char * udpAddress = "192.168.191.2";
const int udpPort = 1982;
unsigned int localPort = 2390;
IPAddress timeServer(183,230,40,42);
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
char packetBuffer2[2048];

void setup() {
  baseInit();
  eepromInit();
  wifiConnect();
  mqttConnect();
  mqInit();
  dhtInit();
  servoInit();
  boardLedInit();

  xTaskCreate(realTask, "realTask", 5000, NULL, 1, &realTaskHandle);
//  xTaskCreate(mainTask, "mainTask", 5000, NULL, 0, &mainTaskHandle);
//  xTaskCreate(servoTask, "servoTask", 10000, NULL, 1, &servoTaskHandle);
//  vTaskStartScheduler(); 

  Udp.begin(localPort);
}

void loop() {
    Serial.println("mainTask Start");
    //ulTaskNotifyTake( pdTRUE, portMAX_DELAY); 
    float humidity,temperature,heatindex;
   
    if(readDHT(&humidity,&temperature,&heatindex)){
      publishData["Humidity"] = humidity;
      publishData["Temperature"] = temperature;
      publishData["Heat index"] = heatindex;
    }
    publishData["hall"] = hallRead();
    publishData["SSID"] = WiFi.SSID();
    publishData["sensorValue"] = readMQ();
    
    char JSONmessageBuffer[300];
    publishData.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
    mqttPublish(JSONmessageBuffer);
    //client.loop();
    
    Udp.beginPacket(udpAddress,udpPort);
    Udp.print("M-SEARCH * HTTP/1.1\r\nHOST:239.255.255.250:1982\r\nMAN:\"ssdp:discover\"\r\nST: wifi_bulb");
    Udp.endPacket();
    int count=0;
    while(1){
      int len = Udp.parsePacket();
      Serial.println(len);
      if(len||count==25){
        break;
      }
        count++;
        delay(500);
    }
    int len = Udp.read(packetBuffer2, 2048);
    if (len > 0) {
      packetBuffer2[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer2);
    
    Serial.println("--------------------------");
    delay(2000);
}

void realTask(void* parameter) {
  while (1) {
    mqttLoop();
    ledFlash();
//  xTaskNotifyGive( mainTaskHandle ); 
//    vTaskDelete(NULL);   
  }
}



