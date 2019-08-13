#ifndef OFFANIM
#define OFFANIM
#include "animation.hpp"
namespace Animations
{
class Off : public Animation
{
    static Off *instance;
    Off(){
        numFrames = 1;
    }
    
    ValueStruct getCurrentFrame() override
    {
        fill_solid(AddrLeds::getInstance()->vals,NUM_LEDS,CRGB::Black);
        return (ValueStruct){true, CRGB::Black, CRGB::Black, 0, 0};
    }
    Animation *getNextAnimation() override { return this; }

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new Off();
        return instance;
    }
};
} // namespace Animations

Animations::Off *Animations::Off::instance;
#endif