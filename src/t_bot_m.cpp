
#include <UniversalTelegramBot.h> // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <WiFiClientSecure.h>
#include "env_m.h"

// Initialize Telegram BOT
const char* BOTtoken = TELEGRAM_BOT_TOKEN; // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
const char *CHAT_IDS[] = TELEGRAM_CHAT_ALLOW_IDS; // //list of authorized chat IDs
const int NUM_CHAT_IDS = sizeof(CHAT_IDS) / sizeof(CHAT_IDS[0]);

WiFiClientSecure client;

UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 5 second.
int botRequestDelay = 5000;
unsigned long lastTimeBotRan;

void setupTelegramBot()
{
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  for (int i = 0; i < NUM_CHAT_IDS; ++i)
  {
    String welcome = "esp початок роботи.\n\n";
    welcome += "опитування датчика кожні 5 секунд\n\n";
    welcome += "команди управління:\n\n";
    welcome += "/ip \n\n";
    welcome += "/status \n";

    bot.sendMessage(CHAT_IDS[i], welcome, "");
  }
}
bool isAuthorizedUser(const String &chat_id)
{
  for (int j = 0; j < NUM_CHAT_IDS; ++j)
  {
    if (chat_id == CHAT_IDS[j])
    {
      return true;
    }
  }
  return false;
}

String makeMessage(bool isHigh)
{
  String voltage = isHigh ? "вкл ☀️" : "викл 🌑";
  String message = "статус: " + voltage;
  return message;
}

void handleNewMessages(int numNewMessages)
{
  for (int i = 0; i < numNewMessages; i++)
  {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);

    if (!isAuthorizedUser(chat_id))
    {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start")
    {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "команди управління:\n\n";
      welcome += "/ip \n\n";
      welcome += "/status \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/status")
    {
      bot.sendMessage(chat_id, makeMessage(digitalRead(0)), ""); // send voltage status
    }

    if (text == "/ip")
    {
      String ip = WiFi.localIP().toString();
      bot.sendMessage(chat_id, "IP адреса: " + ip, "");
    }
  }
}

void telegramSendMessage(bool isHigh)
{
  for (int i = 0; i < NUM_CHAT_IDS; ++i)
  {
    bot.sendMessage(CHAT_IDS[i], makeMessage(isHigh), "");
  }
}

void telegramGetCommandsLoop()
{
  if (millis() > lastTimeBotRan + botRequestDelay)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}