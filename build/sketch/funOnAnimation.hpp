#ifndef FADEON
#define FADEON
#include "animation.hpp"
#include "onAnimation.hpp"
#include "FastLED.h"
#include "addrLeds.hpp"
namespace Animations
{
class FunOn : public Animation
{
    static FunOn *instance;
    FunOn()
    {
        numFrames = 58;
    }
    ValueStruct getCurrentFrame() override
    {
        ValueStruct ret;
        ret.isOff = false;
        ret.topWhite=ret.botWhite=pow(1.214785,frameIdx);
        ret.topColor=ret.botColor=CHSV(frameIdx,255-qmul8(frameIdx,5),ret.topWhite/255ul);
        if (frameIdx < 30)
        {
            uint16_t rainbowEnd = frameIdx+frameIdx/2-(frameIdx*frameIdx)/42;
            fill_solid(AddrLeds::vals, HALF_LEDS, CRGB::Black);
            fill_rainbow_sv(AddrLeds::vals, rainbowEnd, -rainbowEnd * 10, 10, 255, 255);
            AddrLeds::vals[rainbowEnd] = CHSV(0, 0, 255);
        }
        else
        {
            int32_t subtract=((30l - (int32_t)frameIdx) * (30l - (int32_t)frameIdx) * (30l - (int32_t)frameIdx))/400l;
            int16_t head = 23 + subtract;
            for (int16_t i = 0; i < HALF_LEDS; i++)
            {
                if ((int16_t)i >= head)
                    AddrLeds::vals[i] = CHSV(0, 0, max(255 - ((i - head) * (i - head)) / 2, 0));
            }
        }
        Utils::mirrorAboutShortAxis(AddrLeds::getInstance()->vals);
        //delay(500);
        return ret;
    }
    Animation *getNextAnimation() override { return On::getInstance(); }

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new FunOn();
        return instance;
    }
};
} // namespace Animations

Animations::FunOn *Animations::FunOn::instance;
#endif