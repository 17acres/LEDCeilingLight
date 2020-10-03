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
        void reset() override
        {
            numFrames = targetAnimation->getFadeInDuration();
            Animation::reset();
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
            fract8 amountSource = 255*(frameIdx/100)%2;
            ValueStruct sourceValues = sourceAnimation->getCurrentFrame();
            memcpy(AddrLeds::getInstance()->vals, sourceRgb, sizeof(sourceRgb));
            ValueStruct ret = targetAnimation->getCurrentFrame();

            ret.isOff = false;
            blend(ret.botColor, sourceValues.botColor, amountSource);
            blend(ret.topColor, sourceValues.topColor, amountSource);
            blend(AddrLeds::getInstance()->vals, sourceRgb, AddrLeds::getInstance()->vals, NUM_LEDS, amountSource);

            ret.topWhite=(uint16_t)((((uint32_t)sourceValues.topWhite)*amountSource+((uint32_t)ret.topWhite)*(255-amountSource))/255);
            ret.botWhite=(uint16_t)((((uint32_t)sourceValues.botWhite)*amountSource+((uint32_t)ret.botWhite)*(255-amountSource))/255);
            ret.colorScaleFactor=((sourceValues.colorScaleFactor)*(double)amountSource+ret.colorScaleFactor*(255.0-(double)amountSource))/255.0;

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