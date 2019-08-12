#ifndef FADEOFF
#define FADEOFF
#include "animation.hpp"
#include "offAnimation.hpp"
#include "FastLED.h"
#include "addrLeds.hpp"
namespace Animations
{
class FadeOff : public Animation
{
    static FadeOff *instance;
    FadeOff(){
        numFrames = 255;
    }
    valueStruct getCurrentFrame() override
    {
        fill_solid(AddrLeds::vals,HALF_LEDS,CRGB::Black);
        if(frameIdx<25)
            AddrLeds::vals[Utils::saturatingSubtract(HALF_LEDS,(frameIdx))]=CRGB::White;

        fill_rainbow(AddrLeds::getInstance()->vals,Utils::saturatingSubtract(HALF_LEDS,(frameIdx)),frameIdx);
        Utils::mirrorAboutShortAxis(AddrLeds::getInstance()->vals);      
        return (valueStruct){true, CRGB::Black, CRGB::Black, 65535-Utils::saturatingMultiply(frameIdx,400), 65535-Utils::saturatingMultiply(frameIdx,256)};
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

Animations::FadeOff *Animations::FadeOff::instance;
#endif