#include <Arduino.h>
#include <WiFi.h>
// #include <ESPmDNS.h>
#include "env_m.h"

void connectToWiFi();
void startAPMode();
void checkRSSI();
void mDnsStart();
void wifiSetup();
void wifiLoop();

// Global variables
unsigned long previousMillis = 0;
const unsigned long interval = 10000; // Example interval for RSSI check

void connectToWiFi()
{
  WiFi.begin(STASSID, STAPSK);
  Serial.print("Connecting to WiFi");
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 10)
  {
    delay(1000);
    Serial.print(".");
    retries++;
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected to WiFi");
  }
  else
  {
    Serial.println("Failed to connect to WiFi");
  }
}

void startAPMode()
{
  WiFi.softAP(APSSID, APPSK);
  Serial.println("Started Access Point Mode");
}

void checkRSSI()
{
  if (WiFi.status() != WL_CONNECTED)
    return;

  int rssi = WiFi.RSSI();
  Serial.print("Wi-Fi Signal Strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");

  // -30 to -50 dBm: Strong signal, good connection quality.
  // -50 to -70 dBm: Normal signal, usually stable connection, but minor issues possible at long distances.
  // -70 to -80 dBm: Weak signal, possible instability and low speed.
  // -80 and below: Very weak signal, connection may be unstable or even drop.
}

void mDnsStart()
{
  // if (!MDNS.begin("esvitlo")) 
  // {
  //   Serial.println("Error setting up MDNS responder!");
  //   while (1)
  //   {
  //     delay(1000);
  //   }
  // }
  // Serial.println("mDNS responder started");
  // MDNS.addService("http", "tcp", PORT_HTTP);
  // int totalTry = 5;
  //   while (!MDNS.begin("esvitlo") && totalTry > 0) {
  //       Serial.println(".");
  //       delay(1000);
  //       totalTry--;
  //   }

  //   Serial.println("[Wifi] mDNS responder started");
  //   Serial.print("[Wifi] You can now connect to: http://");
  //   Serial.print("esvitlo");
  //   Serial.println(".local");
}

void wifiSetup()
{
  Serial.println("WiFi start");

  connectToWiFi();

  if (WiFi.status() != WL_CONNECTED)
  {
    startAPMode();
  }

  // mDnsStart();
  checkRSSI();
}

void wifiLoop()
{
  unsigned long currentMillis = millis();

  // Non-blocking check every 'interval'
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    checkRSSI();

    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Wi-Fi connection lost. Reconnecting...");
      connectToWiFi();
    }
  }
}