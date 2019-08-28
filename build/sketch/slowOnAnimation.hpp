#ifndef SLOWON
#define SLOWON
#include "animation.hpp"
#include "onAnimation.hpp"
#include "FastLED.h"
#include "addrLeds.hpp"
#include "utils.hpp"

namespace Animations
{
class SlowOn : public Animation
{
    static SlowOn *instance;
    SlowOn()
    {
        numFrames = 32768;
    }
    ValueStruct getCurrentFrame() override
    {
        //CHSV top(frameIdx/512,253-frameIdx/129,min(frameIdx/64ul,255ul));
        CHSV top(0,0,frameIdx/129);
        fill_solid(AddrLeds::getInstance()->vals,NUM_LEDS,CRGB::Black);
        return (ValueStruct){false,top*2,top,Utils::saturatingMultiply(frameIdx,4),frameIdx*2};
    }
    Animation *getNextAnimation() override { return On::getInstance(); }

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new SlowOn();
        return instance;
    }
};
} // namespace Animations
#endif