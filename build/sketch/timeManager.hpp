#ifndef TIMEMANAGER
#define TIMEMANAGER
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//https://github.com/arduino-libraries/NTPClient
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP);
class TimeManager
{
    public:
    static void setup()
    {
        timeClient.begin();
        timeClient.setUpdateInterval(3600000);
    }
    static void updateTime()
    {
        timeClient.update();
        Serial.println(timeClient.getFormattedTime());
    }
};
#endif