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
            isWakeupSoon = false;
            if (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::Off::getInstance())
            {
                IFDEBUG(Serial.println("Slow On Started"));
                Animations::AnimationManager::getInstance()->setAnimation(Animations::SlowOn::getInstance());
                Animations::AnimationManager::getInstance()->startAnimation();
            }
        }
    }
    static void handleSwitchRequest()
    {
        String message = "Switch request recieved\n";
        server.send(200, "text/plain", message);
        IFDEBUG(Serial.println(message));
        EmailSender::sendDebugEmail("Switch web request received");
        LightSwitch::getInstance()->handleSwitchToggle();
    }
    static void handleWakeupLightRequest()
    {
        if (isWakeupSoon)
        {
            server.send(429, "text/plain", "Wakeup time already scheduled");
            EmailSender::sendDebugEmail("Wakeup time already scheduled", "You are tearing me apart. You said one thing, now you say another");
        }
        else
        {
            String message = "Wakeup request recieved:\n";
            message += server.arg("plain");
            message += "\n";
            server.send(200, "text/plain", message);
            IFDEBUG(Serial.println(message));
            EmailSender::sendDebugEmail("Wakeup light request recieved", "Args: " + server.arg("plain"));

            String args = server.arg("plain");
            int minutesBefore = args.substring(args.indexOf(';') + 1).toInt();
            int semicolonLocation = args.indexOf(':');
            int hours = args.substring(semicolonLocation - 2, semicolonLocation).toInt();
            if ((args.indexOf("PM") > -1) && hours != 12)
                hours += 12;
            int minutes = args.substring(semicolonLocation + 1, semicolonLocation + 3).toInt();

            time_t currentTime = TimeManager::getTime();
            tm *currentTimeStruct = localtime(&currentTime);
            tm *alarmTimeStruct = currentTimeStruct;
            alarmTimeStruct->tm_hour = hours;
            alarmTimeStruct->tm_min = minutes;
            wakeupStartTime = mktime(alarmTimeStruct);

            wakeupStartTime -= minutesBefore * 60; //subtract number of seconds
            int timeUntil = wakeupStartTime - currentTime;
            IFDEBUG(Serial.println(timeUntil));
            isWakeupSoon = true;
            EmailSender::sendEmail("Wakeup light request recieved", "Args: " + server.arg("plain") + "<br>Alarm Time: " + asctime(alarmTimeStruct) +"<br>Start Time: " + asctime(localtime(&wakeupStartTime)) + "<br>Time Until Target: " + (timeUntil / (60 * 60)) + ":" + (timeUntil % (60 * 60) / 60) + ":" + (timeUntil % 60));
        }
    }

    static void handleSetModeRequest()
    {
        String message = "Mode selection recieved:\n";
        server.send(200, "text/plain", message);
        IFDEBUG(Serial.println(message));
        String args = server.arg("plain");
        EmailSender::sendDebugEmail("Mode selection received", "Args: " + server.arg("plain"));
        if (args.equals("slowOn"))
        {
            Animations::AnimationManager::getInstance()->setAnimation(Animations::SlowOn::getInstance());
            Animations::AnimationManager::getInstance()->startAnimation();
            IFDEBUG(Serial.println("Slow On mode by direct request"));
        }
        else if (args.equals("on"))
        {
            Animations::AnimationManager::getInstance()->setAnimation(Animations::On::getInstance());
            Animations::AnimationManager::getInstance()->startAnimation();
            IFDEBUG(Serial.println("On mode by direct request"));
        }
        else if (args.equals("fun"))
        {
            Animations::AnimationManager::getInstance()->setAnimation(Animations::Fun::getInstance());
            Animations::AnimationManager::getInstance()->startAnimation();
            IFDEBUG(Serial.println("Fun mode by direct request"));
        }
        else if (args.equals("nightLight"))
        {
            Animations::AnimationManager::getInstance()->setAnimation(Animations::NightLight::getInstance());
            Animations::AnimationManager::getInstance()->startAnimation();
            IFDEBUG(Serial.println("Night light mode by direct request"));
        }
        else if (args.equals("off"))
        {
            Animations::AnimationManager::getInstance()->setAnimation(Animations::Off::getInstance());
            Animations::AnimationManager::getInstance()->startAnimation();
            IFDEBUG(Serial.println("Off mode by direct request"));
        }
        else
        {
            IFDEBUG(Serial.println("Invalid selection"));
        }
    }
};
#endif