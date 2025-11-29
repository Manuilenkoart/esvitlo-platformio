#include <Arduino.h>

#include "board_led_m.h"

#define checkVoltagePin 0 // set interrupt gpio D0

volatile bool hasVoltageVolatile = false;

void IRAM_ATTR interruptChange()
{
  hasVoltageVolatile = digitalRead(checkVoltagePin);
}

void powerManagementSetup()
{
  pinMode(checkVoltagePin, INPUT);

  hasVoltageVolatile = digitalRead(checkVoltagePin);
  boardLedUpdate(hasVoltageVolatile);

  attachInterrupt(digitalPinToInterrupt(checkVoltagePin), interruptChange, CHANGE);
}

void powerManagementLoop(void (*telegramSendMessage)(bool))
{
  static bool lastVoltageState = hasVoltageVolatile;

  telegramSendMessage(hasVoltageVolatile);

  if (hasVoltageVolatile != lastVoltageState)
  {
    telegramSendMessage(hasVoltageVolatile);

    boardLedUpdate(hasVoltageVolatile);

    lastVoltageState = hasVoltageVolatile;
  }
}
