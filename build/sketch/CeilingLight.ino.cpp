#include <Arduino.h>
#line 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
#line 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
#include <GDBStub.h>
#include "hvLeds.hpp"
#include "addrLeds.hpp"
#include "defs.hpp"
#include "animationManager.hpp"
#include "timeManager.hpp"
HvLeds *hvLeds;
AddrLeds *addrLeds;
Animations::AnimationManager *animMan;

#line 11 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void setup();
#line 38 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void loop();
#line 11 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void setup()
{

    hvLeds = HvLeds::getInstance();
    addrLeds = AddrLeds::getInstance();
    animMan = Animations::AnimationManager::getInstance();
    Serial.begin(115200);
    //gdbstub_init();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    //TimeManager::setup();
    // while (millis() < 5000)
    // {
    //     digitalWrite(LED_BUILTIN, HIGH);
    //     delay(500);
    //     digitalWrite(LED_BUILTIN, LOW);
    //     delay(500);
    // }
}

void loop()
{
    Utils::doUpdates();
}

