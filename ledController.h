#ifndef _LEDCONTROLLER_H
#define _LEDCONTROLLER_H

#include "asoym.h"

#define LED_PIN                     2
#define LED_PIN2                    22
#define LEDC_CHANNEL_0              0
#define LEDC_TIMER_13_BIT          13
#define LEDC_BASE_FREQ             5000

void boardLedInit();
void ledFlash();

#endif

