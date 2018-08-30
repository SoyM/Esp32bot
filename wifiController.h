#ifndef _WIFICONTROLLER_H
#define _WIFICONTROLLER_H

#include "Arduino.h"
#include <WiFiMulti.h>

#define AP_SSID      "esp32-soym"

class WifiController
{
  public:
    WifiController(WiFiMulti *wifiMulti);
    static void wifiConnect();
    static int checkWifiConnect();
  private:
    WiFiMulti *_wifiMulti;
};

#endif

