#ifndef FUN
#define FUN
#include "animation.hpp"
#include "onAnimation.hpp"
#include "addrLeds.hpp"
#include "utils.hpp"
#include "funOnAnimation.hpp"

namespace Animations
{
class Fun : public Animation
{
    static Fun *instance;
    Fun()
    {
        numFrames = 255*2;
    }
    ValueStruct getCurrentFrame() override
    {
        ((FunOn*)FunOn::getInstance())->onTime=0;
        digitalWrite(LED_BUILTIN, LOW);
        ValueStruct ret;
        ret.colorScaleFactor=1;
        ret.isOff = false;
        ret.topWhite=ret.botWhite=0;        
        Utils::fill_rainbow_sv(AddrLeds::vals,NUM_LEDS,frameIdx/2,255/NUM_LEDS,255,255);
        ret.topColor=CHSV(frameIdx/2,255,128);
        ret.botColor=0;
        return ret;
    }
    Animation *getNextAnimation() override { return this; }
    String getName() override{return "Fuuuuuuuuuuuunnnnnnnnnnn Mooooooooooode       (Fun Mode)";}

    uint8_t getId() override{return 1;}

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new Fun();
        return instance;
    }
};
} // namespace Animations
#endif