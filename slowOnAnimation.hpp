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
        numFrames = 65536;
    }
    ValueStruct getCurrentFrame() override
    {
        digitalWrite(LED_BUILTIN, HIGH);
        //CHSV top(frameIdx/512,253-frameIdx/129,min(frameIdx/64ul,255ul));
        //CHSV top(0,0,frameIdx/129);
        fill_solid(AddrLeds::getInstance()->vals,NUM_LEDS,CRGB::Black);
        return (ValueStruct){false,0,0,Utils::saturatingMultiply(frameIdx,2),frameIdx};
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