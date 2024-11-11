#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

#include "env_m.h"
#include <wifi_m.h>

const char *ssid = STASSID;
const char *password = STAPSK;
const char *ssidAP = APSSID;
const char *passwordAP = APPSK;

const char *mDnsName = M_DNS_NAME;

const int retryDelay = 5000; // Delay between retries
const int maxRetries = 30;   // Maximum connection retries
unsigned long previousMillis = 0;
const unsigned long interval = 10000; // Interval for checking connection status

int retryCount = 0; // Counter for retry attempts

// private
bool startAPMode()
{
  Serial.println("WiFi up AP");

  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(WiFi.localIP(), WiFi.localIP(), IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssidAP, passwordAP);
  return true;
}

void connectToWiFi()
{
  Serial.printf("Connecting to Wi-Fi: %s\n", ssid);
  WiFi.begin(ssid, password);
  retryCount = 0;

  // Non-blocking retry mechanism
  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - startAttemptTime >= retryDelay)
    {
      retryCount++;
      Serial.print(".");
      startAttemptTime = millis(); // Reset timer for next attempt

      if (retryCount >= maxRetries)
      {
        Serial.println();
        Serial.println("Max retries reached. Could not connect to Wi-Fi.");
        return;
      }
    }
    yield(); // Let other tasks run while retrying
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void mDnsStart()
{
  if (!MDNS.begin(mDnsName))
  {
    Serial.println("Error setting up MDNS responder");
  }
  Serial.print("mDNS responder started: ");
  Serial.print(mDnsName);
  Serial.println(".local");

  MDNS.addService("http", "tcp", PORT_HTTP);
  // MDNS.addService("ws", "tcp", PORT_WS);
}

void checkRSSI()
{
  if (WiFi.status() != WL_CONNECTED)
    return;

  int rssi = WiFi.RSSI();
  Serial.print("Wi-Fi Signal Strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");

  // -30 до -50 дБм — сильний сигнал, гарна якість з'єднання.
  // -50 до -70 дБм — нормальний сигнал, з'єднання зазвичай стабільне, але можливі незначні збої при великих відстанях.
  // -70 до -80 дБм — слабкий сигнал, можлива нестабільність і низька швидкість.
  // -80 і менше — дуже слабкий сигнал, з'єднання може бути нестабільним або навіть зникати.
}

// public
void wifiSetup()
{
  Serial.println("WiFi start");
  
  connectToWiFi();

  if (WiFi.status() != WL_CONNECTED)
  {
    startAPMode();
  }

  mDnsStart();
  checkRSSI();
}

void wifiLoop()
{
  // MDNS.update();

  unsigned long currentMillis = millis();

  // Non-blocking check every 'interval'
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    // checkRSSI();

    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Wi-Fi connection lost. Reconnecting...");
      connectToWiFi();
    }
  }
}
