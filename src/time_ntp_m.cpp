#include <Arduino.h>
#include "time.h"

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

void timeNtpSetup()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

char *getFormattedTime()
{
  static char formattedTime[20];
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return nullptr;
  }

  // Форматуємо дату у вигляді dd.mm.yyyy:HH:MM
  strftime(formattedTime, sizeof(formattedTime), "%d.%m.%Y:%H:%M", &timeinfo);

  return formattedTime;
}