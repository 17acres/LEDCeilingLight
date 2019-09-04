#ifndef WEBSERVER
#define WEBSERVER
#include <ESP8266WebServer.h>
#include "animationManager.hpp"
#include "lightSwitch.hpp"
#include "emailSender.hpp"
//https://techtutorialsx.com/2017/03/26/esp8266-webserver-accessing-the-body-of-a-http-request/

class WebServer
{
private:
    static ESP8266WebServer server;
    static bool isWakeupSoon;
    static time_t wakeupStartTime;

public:
    static void setup()
    {
        server.on("/lightOn", handleSwitchRequest);
        server.on("/wakeupLight", handleWakeupLightRequest);
        server.on("/setMode", handleSetModeRequest);
        server.begin();
    }
    static void update()
    {
        server.handleClient();
        if ((isWakeupSoon) && (TimeManager::getTime() > wakeupStartTime))
        {
            Serial.println("Slow On Started");
            Animations::AnimationManager::getInstance()->setAnimation(Animations::SlowOn::getInstance());
            Animations::AnimationManager::getInstance()->startAnimation();
            isWakeupSoon = false;
        }
    }
    static void handleSwitchRequest()
    {
        String message = "Switch request recieved\n";
        server.send(200, "text/plain", message);
        Serial.println(message);
        LightSwitch::getInstance()->handleSwitchToggle();
        EmailSender::sendEmail("Switch web request received");
    }
    static void handleWakeupLightRequest()
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
        EmailSender::sendEmail("Wakeup light request recieved","Args: "+server.arg("plain"));
    }

    static void handleSetModeRequest()
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
        EmailSender::sendEmail("Mode selection received","Args: "+server.arg("plain"));
    }
};
#endif