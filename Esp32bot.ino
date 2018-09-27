#include "asoym.h"
#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int16.h>
#include <std_msgs/UInt8.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "time.h"

#define ESP32

const char* ntpServer = "cn.pool.ntp.org";
const long  gmtOffset_sec = 28800;
const int   daylightOffset_sec = 0;
const int buttonPin = 0;

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
std_msgs::UInt8 str_msg;
ros::Publisher light_status("light_status", &str_msg);
char hello[13] = "hello world!";
uint32_t message_count = 0;

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
  pinMode(buttonPin, INPUT);

  nh.getHardware()->setConnection(rosServer, rosServerPort);

  nh.initNode();
  nh.subscribe(sub);

  // Another way to get IP
  Serial.print("IP = ");
  Serial.println(nh.getHardware()->getLocalIP());

  // Start to be polite
  nh.advertise(light_status);

  
  MqCon.mqInit();
//  NtpCon.ntpInit();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  xTaskCreate(realTask, "realTask", 5000, NULL, 1, &realTaskHandle);
  xTaskCreate(baseTask, "baseTask", 5000, NULL, 1, &baseTaskHandle);
//  vTaskStartScheduler(); 
}

void loop() {
    int buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      Serial.printf("high");
    }else{
      Serial.printf("low");
    }
    long rssi = WiFi.RSSI();
    Serial.printf("RSSI: %ddBm | ExecCore: %d | message_count: %d\n", rssi, xPortGetCoreID(), message_count);
    message_count = 0;
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

//    struct tm timeinfo;
//    if(!getLocalTime(&timeinfo)){
//      Serial.println("Failed to obtain time");
//      return;
//    }
//    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

    Serial.println("--------------------------");
    delay(1000);
}

void realTask(void* parameter) {
  bool check_sum = true;
  while (1) {
//    MqttCon.mqttLoop();
    
    if (nh.connected())
    {
//      if(MqCon.readMQ()>4050){
        if(digitalRead(buttonPin)==LOW){
        
        if(check_sum==true){
          str_msg.data = 0xFF;
        }else{
          str_msg.data = 0x0F;
        }
      }else{
        
        if(check_sum==true){
          str_msg.data = 0xF0;
        }else{
          str_msg.data = 0x00;
        }
      }

      check_sum = check_sum==true?false:true;
      
      char result = light_status.publish( &str_msg );
      if(result>0){
        //Serial.print(xPortGetCoreID());
        message_count++;
      }
//      Serial.printf("rosmsg publish length: %d\n",result);
    }
    nh.spinOnce();
    
    delay(49);
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
    delay(20);
  }
}
