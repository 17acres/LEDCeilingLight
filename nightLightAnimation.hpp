#ifndef NIGHTLIGHT
#define NIGHTLIGHT
#include "animation.hpp"
#include "utils.hpp"
#include "addrLeds.hpp"
namespace Animations
{
class NightLight : public Animation
{
    static NightLight *instance;
    NightLight(){
        numFrames = 1;
    }
    ValueStruct getCurrentFrame() override
    {
        CHSV colorHSV=CHSV(millis()/255,255,30);
        CRGB top;
        CRGB bot;
        hsv2rgb_spectrum(colorHSV,top);
        colorHSV.h+=127;
        hsv2rgb_spectrum(colorHSV,bot);
        fill_solid(AddrLeds::getInstance()->vals,NUM_LEDS,CRGB::Black);
        return (ValueStruct){false,top,bot, 0, 0};
    }
    Animation *getNextAnimation() override { return this; }

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new NightLight();
        return instance;
    }
};
} // namespace Animations
#endif