#ifndef FADEOFF
#define FADEOFF
#include "animation.hpp"
#include "offAnimation.hpp"
#include "FastLED.h"
#include "addrLeds.hpp"
#include "utils.hpp"

namespace Animations
{
class FadeOff : public Animation
{
    static FadeOff *instance;
    const uint16_t transitionPoint=170;
    FadeOff()
    {
        numFrames = (transitionPoint+HALF_LEDS);
    }
    ValueStruct getCurrentFrame() override
    {

        // if(frameIdx<25)
        //     AddrLeds::vals[Utils::saturatingSubtract(HALF_LEDS,(frameIdx))]=CRGB::White;

        // fill_rainbow(AddrLeds::getInstance()->vals,Utils::saturatingSubtract(HALF_LEDS,(frameIdx)),frameIdx);
        //
        digitalWrite(LED_BUILTIN, LOW);
        ValueStruct ret;
        ret.isOff = false;
        if (frameIdx < 129)
        {
            ret.topWhite = sin16(frameIdx * 128 + 16384) * 2;
            ret.botWhite = ret.topWhite;

            ret.topColor = CHSV(64 - frameIdx / 4, qmul8(frameIdx, 2), frameIdx);
            ret.botColor = ret.topColor;
            Utils::fill_rainbow_sv(AddrLeds::vals, HALF_LEDS, 64, 10, quadwave8(frameIdx), quadwave8(frameIdx));
            AddrLeds::vals[HALF_LEDS - 1] = CHSV(0, 0, quadwave8(frameIdx));
        }
        else
        {
            //ret.topColor = ret.botColor = CHSV(64 - frameIdx / 4, 255, quadwave8((frameIdx*17)/16+24));
            ret.topColor = ret.botColor = CHSV(64 - frameIdx / 4, 255, max(385-frameIdx*2,0));
            ret.topWhite = ret.botWhite = 0;
            if (frameIdx < transitionPoint)
            {                
                Utils::fill_rainbow_sv(AddrLeds::vals, HALF_LEDS, 64, 10, 255, 255);
                AddrLeds::vals[HALF_LEDS - 1] = CHSV(0, 0, 255);
            }
            else
            {
                uint16_t rainbowEnd = (transitionPoint+HALF_LEDS-1)-frameIdx;
                fill_solid(AddrLeds::vals, HALF_LEDS, CRGB::Black);
                Utils::fill_rainbow_sv(AddrLeds::vals, rainbowEnd, 64, 10, 255, 255);

                AddrLeds::vals[rainbowEnd] = CHSV(0, 0, 255);
            }
        }
        Utils::mirrorAboutShortAxis(AddrLeds::getInstance()->vals);
        return ret;
    }
    Animation *getNextAnimation() override { return Off::getInstance(); }

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new FadeOff();
        return instance;
    }
};
} // namespace Animations
#endif