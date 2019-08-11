#ifndef ANIMATION
#define ANIMATION
#include "defs.hpp"
#include "FastLED.h"
class Animation
{
public:
    struct valueStruct
    {
        bool isOff;
        uint16_t topRed;
        uint16_t topGreen;
        uint16_t topBlue;
        uint16_t topWhite;
        uint16_t botRed;
        uint16_t botGreen;
        uint16_t botBlue;
        uint16_t botWhite;

        CRGB addr[NUM_LEDS];
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

    static Animation* getInstance(){
        if(instance ==0)
            instance= new Animation();
        return instance;
    }
private:
    uint16_t frameIdx;
    uint16_t numFrames;
    static Animation* instance;
};
#endif