#include "wifiController.h"

extern WiFiMulti wifiMulti;

WifiController::WifiController(WiFiMulti *wifiMulti){
  _wifiMulti = wifiMulti;
}


int WifiController::checkWifiConnect(){
  return (wifiMulti.run() == WL_CONNECTED);
}

void WifiController::wifiConnect() {
  wifiMulti.addAP("ASUS", "00000000");
  wifiMulti.addAP("huang2", "hxz04010.");
  wifiMulti.addAP("TP-LINK_4A49", "Lanyinkejiccs");
  Serial.println("wifiConnect Start");
  for (int loops = 0; loops < 5; loops++) {
    if (checkWifiConnect()) {
      Serial.println("WiFiConnect success");
      break;
    }
    else {
      Serial.println(loops);
      delay(1000);
    }
  }
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("WiFi connect failed!Rebooting...");
    ESP.restart();
  }
  Serial.println(WiFi.SSID());
//  Serial.println("IP address: " + WiFi.localIP());/
  Serial.println("ESP Mac Address: " + WiFi.macAddress());
//  Serial.println("Subnet Mask: " + WiFi.subnetMask());/
//  Serial.println("Gateway IP: " + WiFi.gatewayIP());/
//  Serial.println("DNS: " + WiFi.dnsIP());
  Serial.println("wifiConnect end");
}


