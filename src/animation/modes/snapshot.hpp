#ifndef SNAPSHOTANIM
#define SNAPSHOTANIM
#include "../animation.hpp"
#include "../../drivers/addrLeds.hpp"

namespace Animations
{
    class Snapshot : public Animation
    {
        static Snapshot *instance;
        CRGB sourceRgb[NUM_LEDS];
        ValueStruct sourceVals;
        Snapshot()
        {
            numFrames = 1;
        }

        // void restart() override
        // {

        //     Animation::restart();
        // }

        ValueStruct getCurrentFrame() override
        {
            memcpy(AddrLeds::getInstance()->vals, sourceRgb, sizeof(sourceRgb)); // write fixed values to array as expected by crossfade
            return sourceVals;
        }
        Animation *getNextAnimation() override { return this; }
        String getName() override { return "Snapshot"; }

    public:
        Animation *sourceAnimation;
        static Animation *getInstance()
        {
            if (instance == 0)
                instance = new Snapshot();
            return instance;
        }
        void takeSnapshot()
        {
            sourceVals = sourceAnimation->getCurrentFrame();
            memcpy(sourceRgb, AddrLeds::getInstance()->vals, sizeof(sourceRgb)); // get fixed copy of current transition position
        }
    };
} // namespace Animations
#endif