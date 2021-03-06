#ifndef ADDRLEDS
#define ADDRLEDS
#include "../utils.hpp"
#include <color.h>
class AddrLeds
{
private:
    static AddrLeds *instance;
    AddrLeds()
    {
        FastLED.addLeds<WS2812B, ADDR_DATA, BRG>(vals, 48).setCorrection(TypicalSMD5050);
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
#endif