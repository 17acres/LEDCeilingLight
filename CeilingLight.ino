#include <GDBStub.h>
#include "hvLeds.hpp"
#include "addrLeds.hpp"
#include "defs.hpp"
#include "animationManager.hpp"

HvLeds* hvLeds;
AddrLeds* addrLeds;
Animations::AnimationManager* animMan;
double temperature=72;

void setup()
{

    hvLeds=HvLeds::getInstance();
    addrLeds=AddrLeds::getInstance();
    animMan=Animations::AnimationManager::getInstance();

    Serial.begin(115200);
    gdbstub_init();
    pinMode(LED_BUILTIN, OUTPUT);

    while (millis() < 5000)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
    }
}

void loop()
{
    animMan->setAnimation(Animations::FunOn::getInstance());
    animMan->restartAnimation();
    delayUntilFinished();
    delayUpdate(2000);
    animMan->setAnimation(Animations::FadeOff::getInstance());
    animMan->restartAnimation();
    delayUntilFinished();
    delayUpdate(2000);
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
    if(millis()%100==0)
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