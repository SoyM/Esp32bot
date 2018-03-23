#ifndef _DHTCONTROLLER_H
#define _DHTCONTROLLER_H

#include "asoym.h"

void dhtInit();
char readDHT(float *humidity,float *temperature,float *heatindex);

#endif


