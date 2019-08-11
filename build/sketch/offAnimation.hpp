#ifndef OFFANIM
#define OFFANIM
#include "animation.hpp"
namespace Animations
{
class Off : public Animation
{
    static Off *instance;
    static CRGB arr[NUM_LEDS];
    int numFrames = 1;
    valueStruct getCurrentFrame()
    {
        return (valueStruct){true, CRGB::Black, CRGB::Black, &arr, 0, 0};
    }
    Animation *getNextAnimation() { return this; }

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new Off();
        return instance;
    }
};
}; // namespace Animations

Animations::Off *Animations::Off::instance;
#endif