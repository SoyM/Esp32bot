#include "asoym.h"
#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int16.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "time.h"

#define ESP32

const char* ntpServer = "cn.pool.ntp.org";
const long  gmtOffset_sec = 28800;
const int   daylightOffset_sec = 0;

WiFiMulti wifiMulti;
WifiController WifiCon(&wifiMulti);
//WiFiClient espClient;/
//PubSubClient mqttClient(espClient);/
//MqttController MqttCon(&mqttClient);/

StaticJsonBuffer<300> JSONbuffer;
JsonObject& publishData = JSONbuffer.createObject();
char JSONmessageBuffer[300];

TaskHandle_t baseTaskHandle,realTaskHandle;

ros::NodeHandle  nh;
IPAddress rosServer(192, 168, 1, 106);
uint16_t rosServerPort = 11411;
std_msgs::Int16 str_msg;
ros::Publisher chatter("chatter", &str_msg);
char hello[13] = "hello world!";

MqController MqCon;
//NtpController NtpCon;


void messageCb( const std_msgs::Empty& toggle_msg){
  digitalWrite(13, HIGH-digitalRead(13));   // blink the led
}
ros::Subscriber<std_msgs::Empty> sub("toggle_led", &messageCb );

void setup() {
  baseInit();
  eepromInit();
  WifiCon.wifiConnect();
//  MqttCon.mqttConnect();/
  boardLedInit();

  nh.getHardware()->setConnection(rosServer, rosServerPort);

  nh.initNode();
  nh.subscribe(sub);

  // Another way to get IP
  Serial.print("IP = ");
  Serial.println(nh.getHardware()->getLocalIP());

  // Start to be polite
  nh.advertise(chatter);

  
  MqCon.mqInit();
//  NtpCon.ntpInit();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  xTaskCreate(realTask, "realTask", 5000, NULL, 1, &realTaskHandle);
  xTaskCreate(baseTask, "baseTask", 5000, NULL, 1, &baseTaskHandle);
//  vTaskStartScheduler(); 
}

void loop() {
    long rssi = WiFi.RSSI();
    Serial.printf("RSSI: %ddBm | ExecCore: %d\n", rssi, xPortGetCoreID());
    //ulTaskNotifyTake( pdTRUE, portMAX_DELAY); 
/*
    HTTPClient http;
    http.begin("http://192.168.1.106/path.html"); //HTTP
    int httpCode = http.GET();
    if(httpCode > 0) {
        Serial.printf("HTTP GET code: %d\n", httpCode);
        if(httpCode == HTTP_CODE_OK) {
//                String payload = http.getString();
//               Serial.println(payload);
        }
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
*/
//    Serial.printf("mq:%d\n",MqCon.readMQ());
//    NtpCon.ntpGet();

    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

    Serial.println("--------------------------");
    delay(1000);
}

void realTask(void* parameter) {
  while (1) {
//    MqttCon.mqttLoop();
    
    if (nh.connected())
    {
      str_msg.data = MqCon.readMQ();
      
      char result = chatter.publish( &str_msg );
      if(result>0){
        Serial.print(xPortGetCoreID());
      }
      //Serial.printf("rosmsg publish length: %d\n",result);
    }
    nh.spinOnce();
    
    delay(100);
//  xTaskNotifyGive( mainTaskHandle ); 
//    vTaskDelete(NULL);   
  }
}


void baseTask(void* parameter) {
  while(1){
    if(!WifiController::checkWifiConnect()){
        WifiController::wifiConnect();  
    }else{
      ledFlash();
    }
  }
}
