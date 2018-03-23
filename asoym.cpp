#include "asoym.h"

void baseInit(){
  Serial.begin(115200);
  Serial.println();
  uint64_t chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.
  Serial.print("ESP32 SDK: ");
  Serial.println(ESP.getSdkVersion());  
}


