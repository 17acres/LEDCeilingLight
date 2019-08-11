#include "defs.hpp"
#include "FastLED.h"
class Animation
{
public:
    struct valueStruct
    {
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
    virtual void startAnimation() = 0;
    virtual void restartAnimation() = 0;
    virtual int getFrameIdx() { return frameIdx; }
    virtual int getNumFrames() { return numFrames; }
    virtual valueStruct getCurrentFrame() = 0;
    virtual void nextFrame() {frameIdx++;}
    
private:
    int frameIdx;
    int numFrames;
};