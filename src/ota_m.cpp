#include <ArduinoOTA.h>

void otaSetup(){
  ArduinoOTA.begin();
}

void otaLoop(){
  ArduinoOTA.handle();
}