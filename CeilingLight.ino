#include <GDBStub.h>
#include "hvLeds.hpp"
#include "FastLED.h"
#include "defs.hpp"

HvLeds* hvLeds;
double temperature=72;

CRGB leds[48];

void setup()
{
    FastLED.addLeds<WS2812B, ADDR_DATA, RGB>(leds, 48);


    hvLeds=HvLeds::getInstance();

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
    
}

void doUpdates(){
    static unsigned long lastRunTime=0;
    if((millis()-lastRunTime)>10){
        lastRunTime=millis();

        HvLeds::getInstance()->update();
        updateTemp();
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
