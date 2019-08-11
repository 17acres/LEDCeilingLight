#ifndef ADDRLEDS
#define ADDRLEDS
#include "FastLED.h"
#include "color.h"
class AddrLeds
{
private:
    static AddrLeds *instance;
    AddrLeds()
    {
        FastLED.addLeds<WS2812B, ADDR_DATA, RGB>(vals, 48).setCorrection(TypicalSMD5050);
    }

public:
    static AddrLeds *getInstance()
    {
        if (instance == 0)
            instance = new AddrLeds();
        return instance;
    }
    CRGB vals[48];
    void update(){FastLED.show();}
};
#endif