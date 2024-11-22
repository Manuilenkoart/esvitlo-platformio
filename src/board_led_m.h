#ifndef board_led_m
#define board_led_m

void boardLedSetup();
void boardLedUpdate(bool isOn);
void boardLedBlinkforSetup(int attemptCounter, int interval);

#endif