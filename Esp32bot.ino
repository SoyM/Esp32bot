#include "asoym.h"
#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>
#include <HTTPClient.h>

#define ESP32

WiFiMulti wifiMulti;
WifiController WifiCon(&wifiMulti);
WiFiClient espClient;
PubSubClient mqttClient(espClient);
MqttController MqttCon(&mqttClient);

StaticJsonBuffer<300> JSONbuffer;
JsonObject& publishData = JSONbuffer.createObject();
char JSONmessageBuffer[300];

TaskHandle_t mainTaskHandle,realTaskHandle;

ros::NodeHandle  nh;
IPAddress rosServer(192, 168, 1, 106);
uint16_t rosServerPort = 11411;
std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);
char hello[13] = "hello world!";


void messageCb( const std_msgs::Empty& toggle_msg){
  digitalWrite(13, HIGH-digitalRead(13));   // blink the led
}
ros::Subscriber<std_msgs::Empty> sub("toggle_led", &messageCb );

void setup() {
  baseInit();
  eepromInit();
  WifiCon.wifiConnect();
  MqttCon.mqttConnect();
  
  boardLedInit();
  xTaskCreate(realTask, "realTask", 5000, NULL, 1, &realTaskHandle);
//  xTaskCreate(mainTask, "mainTask", 5000, NULL, 0, &mainTaskHandle);
//  xTaskCreate(servoTask, "servoTask", 10000, NULL, 1, &servoTaskHandle);
//  vTaskStartScheduler(); 
  pinMode(13, OUTPUT);
  nh.getHardware()->setConnection(rosServer, rosServerPort);

  nh.initNode();
  nh.subscribe(sub);

  // Another way to get IP
  Serial.print("IP = ");
  Serial.println(nh.getHardware()->getLocalIP());

  // Start to be polite
  nh.advertise(chatter);

}

void loop() {
    Serial.println("mainTask Start");
    //ulTaskNotifyTake( pdTRUE, portMAX_DELAY); 
   
   if (nh.connected())
    {
      Serial.println("Connected");
      str_msg.data = hello;
      char result = chatter.publish( &str_msg );
      Serial.printf("%d",result);
    }
    nh.spinOnce();
    delay(1);
    Serial.println("--------------------------");
//    delay(2000);/

//
       HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        http.begin("http://192.168.1.106/path.html"); //HTTP

        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
       if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
//                String payload = http.getString();/
//               Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
}

void realTask(void* parameter) {
  while (1) {
    MqttCon.mqttLoop();
    ledFlash();
//  xTaskNotifyGive( mainTaskHandle ); 
//    vTaskDelete(NULL);   
  }
}





