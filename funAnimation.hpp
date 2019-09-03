#ifndef FUN
#define FUN
#include "animation.hpp"
#include "onAnimation.hpp"
#include "FastLED.h"
#include "addrLeds.hpp"
#include "utils.hpp"

namespace Animations
{
class Fun : public Animation
{
    static Fun *instance;
    Fun()
    {
        numFrames = 255*2;
    }
    ValueStruct getCurrentFrame() override
    {
        digitalWrite(LED_BUILTIN, LOW);
        ValueStruct ret;
        ret.isOff = false;
        ret.topWhite=ret.botWhite=0;        
        Utils::fill_rainbow_sv(AddrLeds::vals,NUM_LEDS,frameIdx/2,255/NUM_LEDS,255,255);
        ret.topColor=CHSV(frameIdx/2,255,64);
        ret.botColor=CHSV(frameIdx/2+128,255,64);
        return ret;
    }
    Animation *getNextAnimation() override { return this; }

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new Fun();
        return instance;
    }
};
} // namespace Animations
#endif