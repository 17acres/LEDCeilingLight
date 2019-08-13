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
        numFrames = 24 + 24 + 12;
    }
    ValueStruct getCurrentFrame() override
    {
        ValueStruct ret;
        ret.isOff = false;
        if (frameIdx < 24)
        {
            ret.topWhite = ret.botWhite = ret.topColor = ret.botColor = 0;

            uint16_t rainbowEnd = frameIdx;
            fill_solid(AddrLeds::vals, HALF_LEDS, CRGB::Black);
            fill_rainbow_sv(AddrLeds::vals, rainbowEnd, 64, 10, 255, 255);
            AddrLeds::vals[rainbowEnd] = CHSV(0, 0, 255);
        }
        else if (frameIdx < 36)
        {
            ret.topWhite = ret.botWhite = ret.topColor = ret.botColor = 0;

            fill_rainbow_sv(AddrLeds::vals, HALF_LEDS, 64, 10, 255, 255);
            int16_t head = 70-2*(int16_t)frameIdx;
            for (uint16_t i = 0; i < HALF_LEDS; i++)
            {
                if (i>=head)
                    AddrLeds::vals[i] = CHSV(0, 0,255-(i-head)*11);
            }
        }else if(frameIdx<48){
            ret.topWhite = ret.botWhite = ret.topColor = ret.botColor = 0;

            fill_solid(AddrLeds::vals, HALF_LEDS, CRGB::Black);
            int16_t head = 70-2*(int16_t)frameIdx;
            for (int16_t i = 0; i < HALF_LEDS; i++)
            {
                if ((int16_t)i>=head)
                    AddrLeds::vals[i] = CHSV(0, 0,max(255-(i-head)*11,0));
            }
            
        }else{
            ret.topWhite = ret.botWhite = ret.topColor = ret.botColor = 0;
            fill_solid(AddrLeds::vals, HALF_LEDS, CRGB::Black);
        }
        Utils::mirrorAboutShortAxis(AddrLeds::getInstance()->vals);
        delay(1000);
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