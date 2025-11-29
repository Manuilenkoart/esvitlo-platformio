#include <Arduino.h>
// #include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "telegram_m.h"

bool messageSent = false;
long lastUpdateId = 0;
const char *telegramHost = "api.telegram.org";

const String BOT_TOKEN = "YOUR_TELEGRAM_BOT_TOKEN";
const String CHAT_ID = "YOUR_TELEGRAM_CHAT_ID";

void telegram_sendMessage(String message)
{
  // Use WiFiClientSecure for HTTPS connection to Telegram
  WiFiClientSecure client;
  client.setInsecure(); // Trust the Telegram server certificate

  if (!client.connect(telegramHost, 443))
  {
    Serial.println("Telegram: Connection to host failed.");
    return;
  }

  // URL Encode the message text
  String encodedMessage = "";
  for (char c : message)
  {
    if (c == ' ')
    {
      encodedMessage += '+';
    }
    else if (c == '&' || c == '=' || c == '?')
    {
      encodedMessage += '%';
      encodedMessage += String(c, HEX);
    }
    else
    {
      encodedMessage += c;
    }
  }

  // Construct the full API request URL
  String url = "/bot" + BOT_TOKEN + "/sendMessage?chat_id=" + CHAT_ID + "&text=" + encodedMessage;

  Serial.println("Telegram: Sending message...");

  // Send the HTTP request
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + telegramHost + "\r\n" +
               "User-Agent: ESP32-C3-Alert\r\n" +
               "Connection: close\r\n\r\n");

  // Wait for response (simplified reading)
  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 5000)
  {
    while (client.available())
    {
      client.read(); // Read and discard to clear the buffer
      timeout = millis();
    }
    delay(1);
  }

  Serial.println("Telegram: Send request complete.");
  client.stop();
}

void telegram_processCommands()
{
  if (WiFi.status() != WL_CONNECTED)
    return;

  WiFiClientSecure client;
  client.setInsecure();

  // Use offset to only get new updates since the last check
  String url = "/bot" + BOT_TOKEN + "/getUpdates?offset=" + String(lastUpdateId + 1);

  if (client.connect(telegramHost, 443))
  {
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + telegramHost + "\r\n" +
                 "User-Agent: ESP32-C3-Poller\r\n" +
                 "Connection: close\r\n\r\n");

    // Read headers and skip to body
    while (client.connected() && !client.available())
    {
      delay(1);
    }
    while (client.available())
    {
      String line = client.readStringUntil('\n');
      if (line == "\r")
        break;
    }

    // Read the entire JSON body for parsing
    String responseBody = client.readString();
    client.stop();

    // --- Minimalist Parsing for /status command ---
    int statusIndex = responseBody.indexOf("\"text\":\"/status\"");

    if (statusIndex > 0)
    {
      Serial.println("Telegram: Command /status received.");

      // Extract the update_id (simplified string search)
      int idKeyIndex = responseBody.lastIndexOf("\"update_id\"", statusIndex);
      if (idKeyIndex > 0)
      {
        int idValueStart = responseBody.indexOf(':', idKeyIndex) + 1;
        int idValueEnd = responseBody.indexOf(',', idValueStart);

        if (idValueStart > idKeyIndex && idValueEnd > idValueStart)
        {
          String idString = responseBody.substring(idValueStart, idValueEnd);
          long newUpdateId = idString.toInt();

          if (newUpdateId > lastUpdateId)
          {
            lastUpdateId = newUpdateId;

            // Send the status message.
            int currentState = digitalRead(0); // gpio D0
            String stateMessage = currentState == HIGH ? "🟢 Pin " + String(TRIGGER_PIN) + " is currently HIGH (Alert Triggered)." : "⚪ Pin " + String(TRIGGER_PIN) + " is currently LOW (All Clear).";
            telegram_sendMessage("Status Check:\n" + stateMessage);
            Serial.println("Telegram: Sent status response.");
          }
        }
      }
    }
  }
  else
  {
    Serial.println("Telegram: Error connecting for updates.");
  }
}

void telegramSendMessage(bool isHigh)
{
  if (!messageSent)
  {
    const char *voltage = isHigh ? "🟢" : "🔴";

    String message = "status " + voltage;
    telegram_sendMessage(message);
    messageSent = true; // Set flag to true after sending
  }
}
void telegramGetCommandsLoop()
{
  // Check for Telegram commands periodically (e.g., every 5 seconds)
  static unsigned long lastCommandCheckTime = 0;
  if (millis() - lastCommandCheckTime >= 5000)
  {
    telegram_processCommands();
    lastCommandCheckTime = millis();
  }
}