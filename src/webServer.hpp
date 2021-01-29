#ifndef WEBSERVER
#define WEBSERVER
#include <ESP8266WebServer.h>
#include "animation/animationManager.hpp"
#include "lightSwitch.hpp"
#include "emailSender.hpp"

#include <AsyncPing.h>
#include <ESP8266WiFi.h>
extern "C"
{
#include <lwip/icmp.h> // needed for icmp packet definitions
}
//https://techtutorialsx.com/2017/03/26/esp8266-webserver-accessing-the-body-of-a-http-request/

class WebServer
{
private:
    static ESP8266WebServer server;
    static WiFiServer tcpServer;
    static bool isWakeupSoon;
    static time_t wakeupStartTime;

public:
    static void setup()
    {
        server.on("/lightOn", handleSwitchRequestHttp);
        server.on("/wakeupLight", handleWakeupLightRequestHttp);
        server.on("/setMode", handleSetModeRequestHttp);
        server.begin();
        tcpServer.begin();
    }
    static void update()
    {
        server.handleClient();
        periodicPingTest();
        if ((isWakeupSoon) && (TimeManager::getTime() > wakeupStartTime))
        {
            isWakeupSoon = false;
            if (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::Off::getInstance())
            {
                IFDEBUG(Serial.println("Slow On Started"));
                Animations::AnimationManager::getInstance()->setAnimation(Animations::SlowOn::getInstance());
                Animations::AnimationManager::getInstance()->restartAnimation();
            }
        }

        checkTcpServer();
    }

    static void handleSwitchRequestHttp()
    {
        server.send(200, "text/plain", handleSwitchRequest());
    }

    static void handleWakeupLightRequestHttp()
    {
        server.send(200, "text/plain", handleWakeupLightRequest(server.arg("plain")));
    }

    static void handleSetModeRequestHttp()
    {
        server.send(200, "text/plain", handleSetModeRequest(server.arg("plain")));
    }

    static void periodicPingTest()
    {
        static unsigned long lastRunTime = 0;
        if ((millis() - lastRunTime) > 5 * 60 * 1000) //every 5 minutes
        {
            runPingTest();
            lastRunTime = millis();
        }
    }
    static void checkTcpServer()
    {
        static unsigned int failCnt=0;
        static unsigned long lastRxTime = 0;

        WiFiClient client = tcpServer.available();
        if (client)
        {
            if (millis() < (lastRxTime + 1000))
            {
                client.stop();
                failCnt++;
                if(failCnt>10){
                    EmailSender::sendEmail("Being attacked??????????");
                    tcpServer.stop();
                }
            }
            else
            {
                char readArr[64];
                int readLen;
                readLen = client.readBytesUntil('\n', readArr, 63);
                readArr[readLen] = '\0';
                String readString(readArr);

                IFDEBUG(Serial.println(readString));
                String path = readString.substring(0, min(readString.indexOf('?'), (int)readString.length()));
                String args = readString.substring(max(readString.indexOf('=') + 1, 0), (int)readString.length());
                IFDEBUG(Serial.println(path));
                IFDEBUG(Serial.println(args));
                if (path == "/lightOn")
                {
                    client.println(handleSwitchRequest());
                }
                else if (path == "/wakeupLight")
                {
                    client.println(handleWakeupLightRequest(args));
                }
                else if (path == "/setMode")
                {
                    client.println(handleSetModeRequest(args));
                }
                else
                {
                    client.println("Not Found");
                }
                if(failCnt>0)
                    failCnt--;
            }
            client.stop();
            lastRxTime = millis();
        }
    }

    static String handleSwitchRequest()
    {
        IFDEBUG(Serial.println("Switch request received\n"));
        EmailSender::sendDebugEmail("Switch web request received");
        LightSwitch::getInstance()->handleSwitchToggle();
        return "Switch request received\n";
    }

    //args is just data
    static String handleWakeupLightRequest(String args)
    {
        if (isWakeupSoon)
        {
            EmailSender::sendDebugEmail("Wakeup time already scheduled", "You are tearing me apart. You said one thing, now you say another");
            return "Wakeup time already scheduled";
        }
        else
        {

            IFDEBUG(Serial.println("Wakeup request received: " + args + "\n"));
            EmailSender::sendDebugEmail("Wakeup light request received", "Args: " + args);

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
            EmailSender::sendEmail("Wakeup light request received", "Args: " + args + "<br>Alarm Time: " + asctime(alarmTimeStruct) + "<br>Start Time: " + asctime(localtime(&wakeupStartTime)) + "<br>Time Until Target: " + (timeUntil / (60 * 60)) + ":" + (timeUntil % (60 * 60) / 60) + ":" + (timeUntil % 60));
            return "Wakeup request received: " + args + "\n";
        }
    }

    static String handleSetModeRequest(String args)
    {
        IFDEBUG(Serial.println("Mode selection received: " + args + "\n"));
        args.toLowerCase();
        EmailSender::sendDebugEmail("Mode selection received", "Args: " + args);
        if (args.indexOf("slow") >= 0)
        {
            Animations::AnimationManager::getInstance()->doTransition(Animations::SlowOn::getInstance());
            IFDEBUG(Serial.println("Slow On mode by direct request"));
        }
        else if (args.indexOf("on") >= 0)
        {
            Animations::AnimationManager::getInstance()->doTransition(Animations::On::getInstance());
            IFDEBUG(Serial.println("On mode by direct request"));
        }
        else if (args.indexOf("party") >= 0)
        {
            Animations::AnimationManager::getInstance()->doTransition(Animations::Party::getInstance());
            IFDEBUG(Serial.println("Party mode by direct request"));
        }
        else if (args.indexOf("night") >= 0)
        {
            Animations::AnimationManager::getInstance()->doTransition(Animations::NightLight::getInstance());
            IFDEBUG(Serial.println("Night light mode by direct request"));
        }
        else if (args.indexOf("off") >= 0)
        {
            Animations::AnimationManager::getInstance()->doTransition(Animations::Off::getInstance());
            IFDEBUG(Serial.println("Off mode by direct request"));
        }
        else
        {
            IFDEBUG(Serial.println("Invalid selection"));
        }
        return "Mode selection received: " + args + "\n";
    }
    static AsyncPing ping;
    static void runPingTest();
};
#endif