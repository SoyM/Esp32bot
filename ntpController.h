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

