#include <Arduino.h>
#line 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
#line 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
#define FASTLED_ALLOW_INTERRUPTS 0

#include <GDBStub.h>
#include "hvLeds.hpp"
#include "addrLeds.hpp"
#include "defs.hpp"
#include "animationManager.hpp"
#include "timeManager.hpp"
#include "lightSwitch.hpp"
#include "EEPROM.h"
#include "webServer.hpp"
HvLeds *hvLeds;
AddrLeds *addrLeds;
Animations::AnimationManager *animMan;

#line 16 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void setup();
#line 45 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void loop();
#line 16 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
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
    WebServer::setup();

}

void loop()
{
    Utils::doUpdates();
}


