#ifndef WEBSERVER
#define WEBSERVER
#include <ESP8266WebServer.h>
#include "animation/animationManager.hpp"
#include "defs.hpp"
#include "lightSwitch.hpp"
#include "emailSender.hpp"

#include <AsyncPing.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "../noGit/auth.hpp"

extern "C"
{
#include <lwip/icmp.h> // needed for icmp packet definitions
}
//https://techtutorialsx.com/2017/03/26/esp8266-webserver-accessing-the-body-of-a-http-request/
//https://beebotte.com/tutorials/led_control

class WebServer
{
private:
    static ESP8266WebServer server;
    //static BearSSL::WiFiClientSecure wiFiClient;
    static WiFiClient wiFiClient;
    //static BearSSL::X509List cert;
    static bool isWakeupSoon;
    static time_t wakeupStartTime;
    static bool hackFlag;
    static const char validMQTTIdChars[63];
    static char mqttId[17];
    //static const char beebotteCACert[2168];

public:
    static PubSubClient mqttClient;
    static void setup()
    {
        server.on("/lightOn", handleSwitchRequestHttp);
        server.on("/wakeupLight", handleWakeupLightRequestHttp);
        server.on("/setMode", handleSetModeRequestHttp);
        server.begin();
        //cert = BearSSL::X509List(beebotteCACert);

        //wiFiClient.setTrustAnchors(&cert);
        wiFiClient.setTimeout(1000);//timeout for low-level connections not like MQTT high-level stuff
        mqttClient.setServer("mqtt.beebotte.com", 1883); //8883 for secure
        // char errStr[120];
        // errStr[0]=0;
        // wiFiClient.getLastSSLError(errStr,120);
        // IFDEBUG(Serial.println(errStr));

        mqttClient.setCallback(mqttHandler);
        mqttClient.setBufferSize(512);
        mqttClient.setSocketTimeout(1);
        IFDEBUG(Serial.println("Web Server Initialized"));
    }
    static void update()
    {
        server.handleClient();
        checkMQTT();
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
        if (((millis() - lastRunTime) > 5 * 60 * 1000) && Utils::wifiPresent) //every 5 minutes
        {
            runPingTest();
            lastRunTime = millis();
        }
    }

    static void mqttHandler(char *topic, byte *payload, unsigned int length)
    {
        static long lastRxTime = 0;
        static int failCnt = 0;
        String topicStr = String(topic);
        if (topicStr.indexOf("control") != -1)
        {
            IFDEBUG(Serial.println((char *)payload));
            StaticJsonDocument<128> root;
            DeserializationError err = deserializeJson(root, payload);

            // Test if parsing succeeds.
            if (err.code() != DeserializationError::Ok)
            {
                IFDEBUG(Serial.println("parseObject() failed"));
                return;
            }
            String requestStr = root["data"];

            if (millis() < (lastRxTime + 1000))
            {
                failCnt++;
                if (failCnt > 10)
                {
                    EmailSender::sendEmail("Being attacked??????????", false);
                    mqttClient.disconnect();
                    hackFlag = true;
                }
            }
            else
            {
                String path = requestStr.substring(0, min(requestStr.indexOf('?'), (int)requestStr.length()));
                String args = requestStr.substring(max(requestStr.indexOf('=') + 1, 0), (int)requestStr.length());
                IFDEBUG(Serial.println(path));
                IFDEBUG(Serial.println(args));
                if (path == "/lightOn")
                {
                    handleSwitchRequest();
                }
                else if (path == "/wakeupLight")
                {
                    handleWakeupLightRequest(args);
                }
                else if (path == "/setMode")
                {
                    handleSetModeRequest(args);
                }
                lastRxTime = millis();
            }
        }
    }

    static const char *generateID()
    {
        randomSeed(analogRead(0));
        int i = 0;
        for (i = 0; i < sizeof(mqttId) - 1; i++)
        {
            mqttId[i] = validMQTTIdChars[random(sizeof(validMQTTIdChars))];
        }
        mqttId[sizeof(mqttId) - 1] = '\0';

        return mqttId;
    }

    static boolean reconnectMQTT()
    {
        if (mqttClient.connect(generateID(), AUTH_MQTT_TOKEN, ""))
        {
            char topic[64];
            sprintf(topic, "%s/%s", "ceilinglight", "control");
            mqttClient.subscribe(topic);

            IFDEBUG(Serial.println("Connected to Beebotte MQTT"));
        }
        return mqttClient.connected();
    }

    static void checkMQTT()
    {
        static long lastReconnectAttempt = 0;
        static long reconnectDelayTime = 5000;
        if (!mqttClient.connected() && !hackFlag)
        {
            long now = millis();
            if (now - lastReconnectAttempt > reconnectDelayTime)
            {
                lastReconnectAttempt = now;
                // Attempt to reconnect
                IFDEBUG(Serial.println("Reconnecting MQTT"));
                if (reconnectMQTT())
                {
                    IFDEBUG(Serial.println("MQTT Reconnected"));
                    lastReconnectAttempt = 0;
                    reconnectDelayTime = 5000;
                }
                else
                {
                    IFDEBUG(Serial.println("MQTT Not Reconnected"));
                    if(reconnectDelayTime<1000*60*60)//12 hrs max retry time
                        reconnectDelayTime=reconnectDelayTime*4; //exponential backoff
                }
            }
        }
        else
        {
            mqttClient.loop();
        }
    }

    static String handleSwitchRequest()
    {
        IFDEBUG(Serial.println("Switch request received\n"));
        EmailSender::sendDebugEmail("Switch web request received", true);
        LightSwitch::getInstance()->handleSwitchToggle();
        return "Switch request received\n";
    }

    //args is just data
    static String handleWakeupLightRequest(String args)
    {
        if (isWakeupSoon)
        {
            EmailSender::sendDebugEmail("Wakeup time already scheduled", "You are tearing me apart. You said one thing, now you say another", true);
            return "Wakeup time already scheduled";
        }
        else
        {

            IFDEBUG(Serial.println("Wakeup request received: " + args + "\n"));
            EmailSender::sendDebugEmail("Wakeup light request received", "Args: " + args, true);

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
            EmailSender::sendDebugEmail("Wakeup light request received", "Args: " + args + "<br>Alarm Time: " + asctime(alarmTimeStruct) + "<br>Start Time: " + asctime(localtime(&wakeupStartTime)) + "<br>Time Until Target: " + (timeUntil / (60 * 60)) + ":" + (timeUntil % (60 * 60) / 60) + ":" + (timeUntil % 60), true);
            return "Wakeup request received: " + args + "\n";
        }
    }

    static String handleSetModeRequest(String args)
    {
        IFDEBUG(Serial.println("Mode selection received: " + args + "\n"));
        args.toLowerCase();
        EmailSender::sendDebugEmail("Mode selection received", "Args: " + args, true);
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