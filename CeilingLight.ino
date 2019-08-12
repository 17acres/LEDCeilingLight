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
    Serial.println("TLC59711 test");
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
    animMan->setAnimation(Animations::On::getInstance());
    animMan->startAnimation();
    delayUpdate(1000);
    animMan->setAnimation(Animations::FadeOff::getInstance());
    animMan->startAnimation();
    delayUntilFinished();
}

void doUpdates(){
    static unsigned long lastRunTime=0;
    if((millis()-lastRunTime)>10){
        lastRunTime=millis();

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
    doUpdates();
    while(mills>=1){
        delay(1);
        doUpdates();
        mills--;
    }
}

void delayUntilFinished(){
    while(!animMan->isFinished()){
        delayUpdate(10);
    }
}