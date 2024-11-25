#include <Arduino.h>
#include "board_led_m.h"

#define LED_PIN 8

void boardLedBlinkforSetup(int attemptCounter, int interval);

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

// TODO: fix; block Setup
void boardLedBlinkforSetup(int attemptCounter, int interval)
{
  static unsigned long previousMillis = 0;
  static int counter = 0;
  static bool ledState = HIGH;

  while (counter < attemptCounter)
  {
    if (millis() - previousMillis >= interval)
    {
      previousMillis = millis();
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);

      if (ledState == HIGH)
      {
        counter++;
      }
    }

    if (counter >= attemptCounter)
    {
      digitalWrite(LED_PIN, HIGH);
    }
  }
}
