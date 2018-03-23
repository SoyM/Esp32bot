#include "ledController.h"

int brightness = 0;    // how bright the LED is
int fadeAmount = 51;    // how many points to fade the LED by

void boardLedInit(){
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);  
  
  pinMode(LED_PIN2, OUTPUT);
  Serial.println("boardLedInit success");
}

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * value;

  // write duty to LEDC
  ledcWrite(channel, duty);
}

void ledFlash(){
    ledcAnalogWrite(LEDC_CHANNEL_0, brightness);
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    delay(200);
}

