#ifndef CROSSFADE
#define CROSSFADE
#include "../animation.hpp"
#include "../../drivers/addrLeds.hpp"
#include "../../utils.hpp"

namespace Animations
{
    class TransCrossfade : public Animation
    {
        CRGB sourceRgb[NUM_LEDS];
        CRGB targetRgb[NUM_LEDS];
        static TransCrossfade *instance;
        TransCrossfade()
        {
            numFrames = 100; //default
        }
        void restart() override
        {
            numFrames = targetAnimation->getFadeInDuration();
            targetAnimation->restart();
            Animation::restart();
        }

        void nextFrame() override
        {
            sourceAnimation->nextFrame();

            if (sourceAnimation->isFinished())
            {
                sourceAnimation = sourceAnimation->getNextAnimation();
                sourceAnimation->restart();
            }

            targetAnimation->nextFrame();

            if (targetAnimation->isFinished())
            {
                targetAnimation = targetAnimation->getNextAnimation();
                targetAnimation->restart();
            }

            Animation::nextFrame();
        }
        ValueStruct getCurrentFrame() override
        {
            fract16 amountSource16 = sin16(((uint16_t)frameIdx) * 32768 / numFrames + 16384) + 32768;
            fract8 amountSource = amountSource16 >> 8;
            //Serial.println(amountSource);
            ValueStruct sourceValues = sourceAnimation->getCurrentFrame();
            memcpy(sourceRgb, AddrLeds::getInstance()->vals, sizeof(sourceRgb));
            ValueStruct ret = targetAnimation->getCurrentFrame();
            // Serial.println("Target: " + ret.toString());
            // Serial.println("Source: " + sourceValues.toString());

            ret.isOff = false;

            ret.botColor = Utils::blendScaled(ret.botColor, sourceValues.botColor, amountSource);
            ret.topColor = Utils::blendScaled(ret.topColor, sourceValues.topColor, amountSource);

            nblend(AddrLeds::getInstance()->vals, sourceRgb, NUM_LEDS, amountSource);

            //Serial.println("Mixed: " + ret.toString());
            return ret;
        }
        Animation *getNextAnimation() override { return targetAnimation; }
        String getName() override { return "Crossfading"; }

    public:
        Animation *sourceAnimation;
        Animation *targetAnimation;
        static Animation *getInstance()
        {
            if (instance == 0)
                instance = new TransCrossfade();
            return instance;
        }
    };
} // namespace Animations
#endif