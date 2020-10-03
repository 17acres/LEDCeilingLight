#ifndef PARTY
#define PARTY
#include "../animation.hpp"
#include "on.hpp"
#include "../../drivers/addrLeds.hpp"
#include "../../utils.hpp"
#include "transOffOn.hpp"

namespace Animations
{
class Party : public Animation
{
    static Party *instance;
    Party()
    {
        numFrames = 255*2;
    }
    ValueStruct getCurrentFrame() override
    {
        ((TransOffOn*)TransOffOn::getInstance())->onTime=0;
        digitalWrite(LED_BUILTIN, LOW);
        ValueStruct ret;
        ret.isOff = false;
        Utils::fill_rainbow_sv(AddrLeds::vals,NUM_LEDS,frameIdx/2,255/NUM_LEDS,255,255);
        ret.topColor=Utils::rgbToRGBW16(CHSV(frameIdx/2,255,128),0,255);
        ret.botColor=Utils::rgbToRGBW16(CRGB::Black,0,255);
        return ret;
    }
    Animation *getNextAnimation() override { return this; }
    String getName() override{return "Paaaaaaaaaaaaaaarty Mooooooooooode       (Party Mode)";}

    uint8_t getId() override{return 1;}

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new Party();
        return instance;
    }
};
} // namespace Animations
#endif