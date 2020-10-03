#ifndef SLOWON
#define SLOWON
#include "../animation.hpp"
#include "on.hpp"
#include "../../drivers/addrLeds.hpp"
#include "../../utils.hpp"
#include "transOffOn.hpp"
namespace Animations
{
    class SlowOn : public Animation
    {
        static SlowOn *instance;
        SlowOn()
        {
            numFrames = 65535;
        }
        ValueStruct getCurrentFrame() override
        {
            if (frameIdx == 0)
                ((TransOffOn *)TransOffOn::getInstance())->onTime = TimeManager::getTime();
            digitalWrite(LED_BUILTIN, HIGH);
            //CHSV top(frameIdx/512,253-frameIdx/129,min(frameIdx/64ul,255ul));
            //CHSV top(0,0,frameIdx/129);
            fill_solid(AddrLeds::getInstance()->vals, NUM_LEDS, CRGB::Black);
            return (ValueStruct){false, Utils::rgbToRGBW16(CRGB::Black, Utils::saturatingMultiply(frameIdx, 2), 255), Utils::rgbToRGBW16(CRGB::Black, frameIdx, 255)};
        }
        Animation *getNextAnimation() override { return On::getInstance(); }
        void nextFrame() override
        {
            static bool skippedFrame = false;
            if ((!isFinished()) && isRunning)
            {
                //Double length of animation by only increasing frame index every other frame
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
        String getName() override { return "Everything the light touches is.... in danger. Slowly       (Slow On)"; }

        uint8_t getId() override { return 3; }

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