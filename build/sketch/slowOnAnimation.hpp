#ifndef SLOWON
#define SLOWON
#include "animation.hpp"
#include "onAnimation.hpp"
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include "addrLeds.hpp"
#include "utils.hpp"
#include "funOnAnimation.hpp"
namespace Animations
{
class SlowOn : public Animation
{
    static SlowOn *instance;
    SlowOn()
    {
        numFrames = 65536;
    }
    ValueStruct getCurrentFrame() override
    {
        if (frameIdx == 0)
            ((FunOn *)FunOn::getInstance())->onTime = TimeManager::getTime();
        digitalWrite(LED_BUILTIN, HIGH);
        //CHSV top(frameIdx/512,253-frameIdx/129,min(frameIdx/64ul,255ul));
        //CHSV top(0,0,frameIdx/129);
        fill_solid(AddrLeds::getInstance()->vals, NUM_LEDS, CRGB::Black);
        return (ValueStruct){false, 0, 0, Utils::saturatingMultiply(frameIdx, 2), frameIdx,1};
    }
    Animation *getNextAnimation() override { return On::getInstance(); }
    void nextFrame() override
    {
        static bool skippedFrame = false;
        if ((!isFinished()) && isRunning)
        {
            if (frameIdx < 65530 && frameIdx > 1)
            {
                if ((frameIdx % 2 == 0) && !skippedFrame)
                {
                    skippedFrame = true;
                }
                else
                {
                    frameIdx++;
                    skippedFrame = false;
                }
            }
            else
            {
                frameIdx++;
            }
        }
        if (isFinished())
            isRunning = false;
    }

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new SlowOn();
        return instance;
    }
};
} // namespace Animations
#endif