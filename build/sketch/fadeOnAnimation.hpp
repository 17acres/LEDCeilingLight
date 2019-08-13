#ifndef FADEON
#define FADEON
#include "animation.hpp"
#include "onAnimation.hpp"
#include "FastLED.h"
#include "addrLeds.hpp"
namespace Animations
{
class FadeOn : public Animation
{
    static FadeOn *instance;
    FadeOn()
    {
        numFrames = 129;
    }
    ValueStruct getCurrentFrame() override
    {
        ValueStruct ret;
        ret.isOff = false;
        ret.topColor = ret.botColor = CHSV(0, 0, qmul8(frameIdx, 2));

        ret.topWhite = Utils::saturatingMultiply(frameIdx, 512);
        ret.botWhite = ret.topWhite;
        fill_solid(AddrLeds::vals, NUM_LEDS, CRGB::Black);
        return ret;
    }
    Animation *getNextAnimation() override { return On::getInstance(); }

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new FadeOn();
        return instance;
    }
};
} // namespace Animations

Animations::FadeOn *Animations::FadeOn::instance;
#endif