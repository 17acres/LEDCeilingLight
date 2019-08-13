#ifndef ADDRLEDS
#define ADDRLEDS
#include "FastLED.h"
#include "defs.hpp"
#include "color.h"
class AddrLeds
{
private:
    static AddrLeds *instance;
    AddrLeds()
    {
        FastLED.addLeds<WS2812B, ADDR_DATA, BRG>(vals, 48);//.setCorrection(TypicalSMD5050);
    }

public:
    static AddrLeds *getInstance()
    {
        if (instance == 0)
            instance = new AddrLeds();
        return instance;
    }
    static CRGB vals[48];
    void update() { FastLED.show(); }
};

AddrLeds *AddrLeds::instance;
CRGB AddrLeds::vals[48];
#endif