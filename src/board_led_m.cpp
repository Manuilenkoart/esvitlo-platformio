#include <Arduino.h>
#include "board_led_m.h"

#define LED_PIN 8

void boardLedSetup()
{
  pinMode(LED_PIN, OUTPUT);
}

void boardLedUpdate(bool isOn)
{
  digitalWrite(LED_PIN, isOn ? LOW : HIGH); // Turn the LED on (Note that LOW is the voltage level
                                            // but actually the LED is on; this is because
                                            // it is active low on the ESP-01)
}
