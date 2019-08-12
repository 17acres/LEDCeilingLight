#ifndef ONANIM
#define ONANIM
#include "animation.hpp"
namespace Animations
{
class On : public Animation
{
    static On *instance;
    On(){
        numFrames = 1;
    }
    valueStruct getCurrentFrame() override
    {
        return (valueStruct){true, CRGB::Black, CRGB::Black, 65535, 65535};
    }
    Animation *getNextAnimation() override { return this; }

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
#endif