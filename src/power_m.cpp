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

void powerManagementLoop(void (*setDataSEE)(bool))
{
  static bool lastVoltageState = hasVoltageVolatile;

  setDataSEE(hasVoltageVolatile);

  if (hasVoltageVolatile != lastVoltageState)
  {
    setDataSEE(hasVoltageVolatile);

    boardLedUpdate(hasVoltageVolatile);

    lastVoltageState = hasVoltageVolatile;
  }
}
