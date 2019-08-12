#include <Arduino.h>
#line 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
#line 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
#include <GDBStub.h>
#include "hvLeds.hpp"
#include "addrLeds.hpp"
#include "defs.hpp"
#include "animationManager.hpp"

HvLeds* hvLeds;
AddrLeds* addrLeds;
Animations::AnimationManager* animMan;
double temperature=72;

#line 12 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void setup();
#line 35 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void loop();
#line 45 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void doUpdates();
#line 58 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void updateTemp();
#line 68 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void delayUpdate(unsigned long mills);
#line 76 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void delayUntilFinished();
#line 12 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
void setup()
{

    hvLeds=HvLeds::getInstance();
    addrLeds=AddrLeds::getInstance();
    animMan=Animations::AnimationManager::getInstance();

    Serial.begin(115200);
    gdbstub_init();
    Serial.println("TLC59711 test");
    pinMode(LED_BUILTIN, OUTPUT);

    while (millis() < 5000)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
    }

    Serial.println(Utils::saturatingMultiply(32768,2));
}

void loop()
{
    // animMan->setAnimation(Animations::On::getInstance());
    // animMan->startAnimation();
    // delayUpdate(1000);
    animMan->setAnimation(Animations::FadeOff::getInstance());
    animMan->restartAnimation();
    delayUntilFinished();
}

void doUpdates(){
    static unsigned long lastRunTime=0;
    if((millis()-lastRunTime)>10){
        lastRunTime=millis();
        digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
        hvLeds->update();
        addrLeds->update();
        updateTemp();

        animMan->update();
    }
}

void updateTemp(){
    static double intTemp;
    static const double alpha=.005;

    double raw=analogRead(TEMP_PIN);
    intTemp=raw*-.11818+182.6364;
    temperature = alpha * intTemp + (1-alpha) * temperature;
    Serial.println(temperature);
}

void delayUpdate(unsigned long mills){
    unsigned long targetTime=millis()+mills;
    while(millis()<targetTime){
        yield();
        doUpdates();
    }
}

void delayUntilFinished(){
    while(!animMan->isFinished()){
        doUpdates();
        yield();
    }
}
