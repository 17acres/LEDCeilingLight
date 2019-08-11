#ifndef ONANIM
#define ONANIM
#include "animation.hpp"
namespace Animations
{
class On : public Animation
{
    static On *instance;
    static CRGB arr[NUM_LEDS];
    int numFrames = 1;
    valueStruct getCurrentFrame()
    {
        return (valueStruct){true, CRGB::Black, CRGB::Black, &arr, 65535, 65535};
    }
    Animation *getNextAnimation() { return this; }

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new On();
        return instance;
    }
};
} // namespace Animations

Animations::On *Animations::On::instance;
CRGB Animations::On::arr[NUM_LEDS];
#endif