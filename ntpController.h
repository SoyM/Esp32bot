/*
 * usage:
 * 
 * #include "ntpController.h"
 * 
 * const char* ntpServer = "cn.pool.ntp.org";
 * const long  gmtOffset_sec = 28800;
 * const int   daylightOffset_sec = 0;
 * 
 * NtpController NtpCon;
 * NtpCon.ntpInit();
 * configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 * 
 * NtpCon.ntpGet();
 * struct tm timeinfo;
 * if(!getLocalTime(&timeinfo)){
 *   Serial.println("Failed to obtain time");
 *   return;
 * }
 * Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
 * 
 */




#ifndef _NTPCONTROLLER_H
#define _NTPCONTROLLER_H

#include "Arduino.h"
#include <WiFiUdp.h>


class NtpController
{
  public:
    void ntpInit();
    int ntpGet();
    unsigned long sendNTPpacket(IPAddress& address);
};

#endif
