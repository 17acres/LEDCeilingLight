#include <Arduino.h>
#line 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
#line 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
#include <GDBStub.h>
#include "hvLeds.hpp"
#include "addrLeds.hpp"
#include "defs.hpp"
#include "animationManager.hpp"
#include "timeManager.hpp"
HvLeds* hvLeds;
AddrLeds* addrLeds;
Animations::AnimationManager* animMan;

#line 11 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void setup();
#line 31 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void loop();
#line 11 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void setup()
{

    hvLeds=HvLeds::getInstance();
    addrLeds=AddrLeds::getInstance();
    animMan=Animations::AnimationManager::getInstance();
    Serial.begin(115200);
    TimeManager::setup();
    gdbstub_init();
    pinMode(LED_BUILTIN, OUTPUT);
    while ( WiFi.status() != WL_CONNECTED ) {delay(500); Serial.print(".");}
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
    // hvLeds->setTop(CHSV(millis()/100,millis()/100,millis()/100),32768);
    // hvLeds->setBot(CHSV(millis()/100,millis()/100,millis()/100),32768);
    Utils::doUpdates();
    // animMan->setAnimation(Animations::FunOn::getInstance());
    // animMan->restartAnimation();
    // delayUntilFinished();
    // delayUpdate(2000);
    // animMan->setAnimation(Animations::FadeOff::getInstance());
    // animMan->restartAnimation();
    // delayUntilFinished();
    // delayUpdate(2000);
}

