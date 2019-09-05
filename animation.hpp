#ifndef ANIMATION
#define ANIMATION
#include "defs.hpp"
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include "utils.hpp"
namespace Animations
{

class Animation
{
public:
    struct ValueStruct
    {
        bool isOff;
        CRGB topColor;
        CRGB botColor;
        uint16_t topWhite;
        uint16_t botWhite;

        String toString()
        {
            return ("isOff: " + String(isOff) + ", Top (" + String(topColor.r) + "," + String(topColor.g) + "," + String(topColor.b) + "," + String(topWhite) + ")" + ", Bot (" + String(botColor.r) + "," + String(botColor.g) + "," + String(botColor.b) + "," + String(botWhite) + ")");
        }
    };
    typedef struct ValueStruct ValueStruct;
    virtual void reset() { frameIdx = 0; }
    virtual void start() { isRunning = true; }
    virtual void restart()
    {
        frameIdx = 0;
        isRunning = true;
    }
    virtual void forceEnd()
    {
        frameIdx = numFrames - 1;
        isRunning = false;
    }
    virtual uint16_t getFrameIdx() { return frameIdx; }
    virtual uint16_t getNumFrames() { return numFrames; }
    virtual ValueStruct getCurrentFrame() = 0;
    virtual void nextFrame()
    {
        if ((!isFinished()) && isRunning)
            frameIdx++;
        if (isFinished())
            isRunning = false;
    }
    virtual Animation *getNextAnimation() = 0;
    virtual bool isFinished() { return frameIdx+1 == numFrames; }

protected:
    uint16_t frameIdx;
    uint16_t numFrames;
    bool isRunning;
};
} // namespace Animations
#endif