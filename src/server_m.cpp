#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <Ticker.h>

#include "env_m.h"

AsyncWebServer server(PORT_HTTP);
AsyncEventSource events("/events");
Ticker ticker;

boolean hasPower = false;
unsigned long idMessage = 0;

void addDefaultHeaders();
void handleOptionsRequest(AsyncWebServerRequest *request);
void handleSSEPing();
void handleReset(AsyncWebServerRequest *request);

void SSESetup()
{
  addDefaultHeaders();

  events.onConnect([](AsyncEventSourceClient *client)
                   {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID it got is: %u\n", client->lastId());
    }
    const char *voltage = hasPower ? "ON" : "OFF";
    String message = "{ \"name\":\"connection\", \"id\":\"" + String(client->lastId()) + "\", \"voltage\":\"" + voltage + "\", \"timestamp\":\"" + String(millis()) + "\" }";
    client->send(message.c_str(), NULL, idMessage, 1000);
    idMessage++; });

  server.addHandler(&events);

  server.on("/events", HTTP_OPTIONS, handleOptionsRequest);

  server.on("/reset", HTTP_GET, handleReset);

  server.onNotFound([](AsyncWebServerRequest *request)
                    {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(404);
    } });

  handleSSEPing();

  server.begin();
  Serial.println("Server started");
}

void addDefaultHeaders()
{
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  DefaultHeaders::Instance().addHeader("Cache-Control", "no-cache");
}
void handleOptionsRequest(AsyncWebServerRequest *request)
{
  request->send(200); // No content response for OPTIONS
}

void handleSSEPing()
{
  ticker.attach(1, []()
                {
    const char *voltage = hasPower ? "ON" : "OFF";
    String message = "{ \"name\":\"ping\", \"id\":\"" + String(idMessage) + "\", \"voltage\":\"" + voltage + "\", \"timestamp\":\"" + String(millis()) + "\" }";
    events.send(message.c_str(), "ping", idMessage, 1000);
    idMessage++; });
}

void setDataSEE(bool isHigh)
{
  hasPower = isHigh;
}

void handleReset(AsyncWebServerRequest *request)
{
  request->send(200, "text/plain", "reset ok");
  ESP.restart();
}