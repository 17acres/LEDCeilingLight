#include <Arduino.h>
#line 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
#line 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
#define FASTLED_ALLOW_INTERRUPTS 0

#include <GDBStub.h>
#include <ESP8266WebServer.h>
#include "hvLeds.hpp"
#include "addrLeds.hpp"
#include "defs.hpp"
#include "animationManager.hpp"
#include "timeManager.hpp"
#include "lightSwitch.hpp"
#include "EEPROM.h"
HvLeds *hvLeds;
AddrLeds *addrLeds;
Animations::AnimationManager *animMan;
ESP8266WebServer server(80);

bool isWakeupSoon;
time_t wakeupStartTime;
#line 19 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void setup();
#line 47 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void loop();
#line 61 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void handleSwitchRequest();
#line 68 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void handleWakeupLightRequest();
#line 95 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void handleSetModeRequest();
#line 19 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void setup()
{
    hvLeds = HvLeds::getInstance();
    addrLeds = AddrLeds::getInstance();
    animMan = Animations::AnimationManager::getInstance();
    Serial.begin(115200);
    //gdbstub_init();
    EEPROM.begin(sizeof(unsigned int));
    unsigned int numSeconds;
    EEPROM.get(0, numSeconds);
    Serial.print("On Mode Second Count ");
    Serial.println(numSeconds);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    TimeManager::setup();
    server.on("/lightOn", handleSwitchRequest);
    server.on("/wakeupLight", handleWakeupLightRequest);
    server.on("/setMode", handleSetModeRequest);
    server.begin();
}

void loop()
{
    Utils::doUpdates();
    server.handleClient();
    if ((isWakeupSoon) && (TimeManager::getTime() > wakeupStartTime))
    {
        Serial.println("Slow On Started");
        Animations::AnimationManager::getInstance()->setAnimation(Animations::SlowOn::getInstance());
        Animations::AnimationManager::getInstance()->startAnimation();
        isWakeupSoon = false;
    }
}

//https://techtutorialsx.com/2017/03/26/esp8266-webserver-accessing-the-body-of-a-http-request/
void handleSwitchRequest()
{
    String message = "Switch request recieved\n";
    server.send(200, "text/plain", message);
    Serial.println(message);
    LightSwitch::getInstance()->handleSwitchToggle();
}
void handleWakeupLightRequest()
{
    String message = "Wakeup request recieved:\n";
    message += server.arg("plain");
    message += "\n";
    server.send(200, "text/plain", message);
    Serial.println(message);
    String args = server.arg("plain");
    int minutesBefore = args.substring(args.indexOf(';') + 1).toInt();
    int hours = args.substring(0, args.indexOf(':')).toInt();
    if ((args.indexOf("PM") > -1) && hours != 12)
        hours += 12;
    int minutes = args.substring(args.indexOf(':') + 1, args.indexOf(';') - 3).toInt();

    time_t currentTime = TimeManager::getTime();
    tm *currentTimeStruct = localtime(&currentTime);
    tm *targetTimeStruct = currentTimeStruct;
    targetTimeStruct->tm_hour = hours;
    targetTimeStruct->tm_min = minutes;
    wakeupStartTime = mktime(targetTimeStruct);

    wakeupStartTime -= minutesBefore * 60; //subtract number of seconds
    int timeUntil = wakeupStartTime - currentTime;
    Serial.println(timeUntil);
    isWakeupSoon = true;
}

void handleSetModeRequest()
{
    String message = "Mode selection recieved:\n";
    server.send(200, "text/plain", message);
    Serial.println(message);
    String args = server.arg("plain");
    if (args.equals("slowOn"))
    {
        Animations::AnimationManager::getInstance()->setAnimation(Animations::SlowOn::getInstance());
        Animations::AnimationManager::getInstance()->startAnimation();
        Serial.println("Slow On mode by direct request");
    }
    else if (args.equals("on"))
    {
        Animations::AnimationManager::getInstance()->setAnimation(Animations::On::getInstance());
        Animations::AnimationManager::getInstance()->startAnimation();
        Serial.println("On mode by direct request");
    }
    else if (args.equals("fun"))
    {
        Animations::AnimationManager::getInstance()->setAnimation(Animations::Fun::getInstance());
        Animations::AnimationManager::getInstance()->startAnimation();
        Serial.println("Fun mode by direct request");
    }
    else if (args.equals("nightLight"))
    {
        Animations::AnimationManager::getInstance()->setAnimation(Animations::NightLight::getInstance());
        Animations::AnimationManager::getInstance()->startAnimation();
        Serial.println("Night light mode by direct request");
    }
    else if (args.equals("off"))
    {
        Animations::AnimationManager::getInstance()->setAnimation(Animations::Off::getInstance());
        Animations::AnimationManager::getInstance()->startAnimation();
        Serial.println("Off mode by direct request");
    }
    else
    {
        Serial.println("Invalid selection");
    }
}
