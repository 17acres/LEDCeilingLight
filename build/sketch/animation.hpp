#ifndef ANIMATION
#define ANIMATION
#include "defs.hpp"
#include "FastLED.h"
#include "utils.hpp"
class Animation
{
public:
    struct valueStruct
    {
        bool isOff;
        CRGB topColor;
        CRGB botColor;
        CRGB (*addr)[NUM_LEDS];

        uint16_t topWhite;
        uint16_t botWhite;
    };
    typedef struct valueStruct valueStruct;
    virtual void start() {frameIdx=0;}
    virtual void restart() {frameIdx=0;}
    virtual void forceEnd() {frameIdx=numFrames-1;}
    virtual uint16_t getFrameIdx() { return frameIdx; }
    virtual uint16_t getNumFrames() { return numFrames; }
    virtual valueStruct getCurrentFrame() = 0;
    virtual void nextFrame() { if((frameIdx-1)<numFrames) frameIdx++; }
    virtual Animation* getNextAnimation()=0;
private:
    uint16_t frameIdx;
    uint16_t numFrames;
};
#endif