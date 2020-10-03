#ifndef ONANIM
#define ONANIM
#include "../animation.hpp"
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

        return (ValueStruct){false, 0, 0, 65535, 65535, 1};
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