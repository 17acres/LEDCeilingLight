#ifndef ONANIM
#define ONANIM
#include "../animation.hpp"
#include "../../utils.hpp"
namespace Animations
{
class On : public Animation
{
    static On *instance;
    On()
    {
        numFrames = 1;
    }
    ValueStruct getCurrentFrame() override
    {
        digitalWrite(LED_BUILTIN, LOW);
        fill_solid(AddrLeds::getInstance()->vals, NUM_LEDS, CRGB::Black);
        return (ValueStruct){false, Utils::rgbToRGBW16(CRGB::Black,65535,255), Utils::rgbToRGBW16(CRGB::Black,65535,255)};
    }
    Animation *getNextAnimation() override { return this; }
    String getName() override{return "A brilliant light shall now spew forth with great fury        (On)";}

    uint8_t getId() override{return 2;}

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new On();
        return instance;
    }
};
} // namespace Animations
#endif