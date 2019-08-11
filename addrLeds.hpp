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
        vals = new CRGB[1][48];
        FastLED.addLeds<WS2812B, ADDR_DATA, RGB>(*vals, 48).setCorrection(TypicalSMD5050);
    }

public:
    static AddrLeds *getInstance()
    {
        if (instance == 0)
            instance = new AddrLeds();
        return instance;
    }
    CRGB (*vals)[48];
    void update() { FastLED.show(); }
};

AddrLeds *AddrLeds::instance;
#endif