#ifndef FADEOFF
#define FADEOFF
#include "animation.hpp"
#include "offAnimation.hpp"
namespace Animations
{
class FadeOff : public Animation
{
    static FadeOff *instance;
    static CRGB arr[NUM_LEDS];
    int numFrames = 255;
    valueStruct getCurrentFrame()
    {
        return (valueStruct){true, CRGB::Black, CRGB::Black, &arr, max(0,65535-frameIdx*300), 65535-frameIdx*256};
    }
    Animation *getNextAnimation() { return Off::getInstance(); }

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
CRGB Animations::FadeOff::arr[NUM_LEDS];
#endif