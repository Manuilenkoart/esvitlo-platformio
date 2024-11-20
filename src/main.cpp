#include <Arduino.h>

#include "wifi_m.h"
#include "server_m.h"
#include "power_m.h"
#include "ota_m.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n");

  for (uint8_t t = 4; t > 0; t--)
  {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  wifiSetup();
  SSESetup();
  powerManagementSetup();
  otaSetup();
}

void loop()
{
  wifiLoop();
  powerManagementLoop(setDataSEE);
  otaLoop();
}
