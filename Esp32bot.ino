#include "asoym.h"
#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int16.h>
#include <std_msgs/UInt8.h>
#include <ArduinoJson.h>
#include "websocket.h"
#include "wifiController.h"
#include "mqController.h"
#include "eepromController.h"
#include "ledController.h"

#define ESP32
#define EN_BatteryAdc
#define EN_BuzzerAlarm

const int buttonPin = 0;
char serverAddress[] = "192.168.1.106"; 
int ws_port = 9002;


WiFiMulti wifiMulti;
WifiController WifiCon(&wifiMulti);
WiFiClient wifi_client;

TaskHandle_t baseTaskHandle,realTaskHandle;

#if defined(_ROS_H_)
  ros::NodeHandle  nh;
  IPAddress rosServer(192, 168, 1, 106);
  uint16_t rosServerPort = 11411;
  std_msgs::UInt8 str_msg;
  ros::Publisher light_status("light_status", &str_msg);
  char hello[13] = "hello world!";
  uint32_t message_count = 0;
#endif

#if defined(EN_BatteryAdc)
  MqController MqCon;
#endif


#ifdef _WEBSOCKET_H
  WebSocketClient ws_client;
  String ws_readData;
  String ws_sendData;
#endif


#ifdef _ROS_H_
void messageCb( const std_msgs::Empty& toggle_msg){
  digitalWrite(13, HIGH-digitalRead(13));  
}
ros::Subscriber<std_msgs::Empty> sub("toggle_led", &messageCb );
#endif


void setup() {
  baseInit();
  eepromInit();
  WifiCon.wifiConnect();
  boardLedInit();
  
#ifdef _ROS_H_
  pinMode(buttonPin, INPUT);
  nh.getHardware()->setConnection(rosServer, rosServerPort);
  nh.initNode();
  nh.subscribe(sub);
  Serial.print("ROS IP = ");
  Serial.println(nh.getHardware()->getLocalIP());
  // Start to be polite
  nh.advertise(light_status);
#endif

#if defined(EN_BatteryAdc)
  MqCon.mqInit();
#endif

#ifdef _WEBSOCKET_H
  ws_client.path = "/";
  ws_client.host = "192.168.1.106";                                             
  if (ws_client.handshake(wifi_client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
  }
#endif


  xTaskCreate(realTask, "realTask", 5000, NULL, 1, &realTaskHandle);
  xTaskCreate(baseTask, "baseTask", 5000, NULL, 1, &baseTaskHandle);
//  vTaskStartScheduler(); 
}


int buttonState = 0;
int rssi = 0;


void loop() {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      Serial.printf("high");
    }else{
      Serial.printf("low");
    }

    if (!(&wifi_client)->connected()){
      #ifdef _WEBSOCKET_H
      if (ws_client.handshake(wifi_client)) {
        Serial.println("socket connect successful");
      } else {
        Serial.println("socket connect failed.");
      }
      #endif
    }
    rssi = WiFi.RSSI();
    Serial.printf("RSSI: %ddBm | ExecCore: %d | message_count: %d\n", rssi, xPortGetCoreID(), message_count);
    message_count = 0;
    
    //ulTaskNotifyTake( pdTRUE, portMAX_DELAY); 

    Serial.println("--------------------------");
    delay(500);
}


char* itostr(char *str, int i){
  sprintf(str, "%d", i);
  return str;
}


void realTask(void* parameter) {
#ifdef _ROS_H_
  bool check_sum = true;
  uint32_t sum_adc = 0;
  char ros_pub_result = 0;
#endif

#if defined(EN_BatteryAdc)
  uint32_t battery_count = 0;
  uint32_t adc_sample_num = 20;
  char battery_adc_c;
  int battery_adc = 0;
#endif
  
  while (1) {

#ifdef _WEBSOCKET_H
  #ifdef EN_BatteryAdc
    sum_adc = sum_adc + MqCon.readMQ();
    battery_count++;
    if(battery_count==adc_sample_num){
      battery_adc = sum_adc/adc_sample_num;

      Serial.printf("mq  |  %d\n",battery_adc);

      ws_sendData = "{\"battery_adc\":";
      ws_sendData.concat(itostr(&battery_adc_c,battery_adc));
      ws_sendData.concat("}");


      ws_client.sendData(ws_sendData);
      ws_client.getData(ws_readData);
      if (ws_readData.length() > 0) {
        Serial.print("Received data: ");
        Serial.println(ws_readData);
      }
    
      sum_adc = 0;
      battery_count = 0;
    }
  #endif
#endif


#ifdef _ROS_H_
    if (nh.connected())
    {
//      if(MqCon.readMQ()>4050){
//      Serial.printf("digitalRead(buttonPin): %d\n" , digitalRead(buttonPin));
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
      
      ros_pub_result = light_status.publish( &str_msg );
      if(ros_pub_result>0){
        //Serial.print(xPortGetCoreID());
        message_count++;
      }
//      Serial.printf("rosmsg publish length: %d\n",result);
    }
    nh.spinOnce();
#endif

    delay(48);
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

    delay(10);
  }
}
