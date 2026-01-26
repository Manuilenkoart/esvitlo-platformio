#include <Arduino.h>
#include "wifi_m.h"
#include "board_led_m.h"
#include "t_bot_m.h"
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
  boardLedSetup();
  setupTelegramBot();
  powerManagementSetup(telegramSendMessage);
  otaSetup();
}

void loop()
{
  wifiLoop();
  powerManagementLoop(telegramSendMessage);
  telegramGetCommandsLoop();
  otaLoop();
}
