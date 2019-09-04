#ifndef OFFANIM
#define OFFANIM
#include "animation.hpp"
#include "addrLeds.hpp"
#include "funOnAnimation.hpp"
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
        ((FunOn*)FunOn::getInstance())->onTime=0;
        digitalWrite(LED_BUILTIN, HIGH);
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
#endif