#ifndef ANIMATION
#define ANIMATION
#include "../utils.hpp"
#include "../defs.hpp"

namespace Animations
{

    class Animation
    {
    public:
        Animation()
        {
            frameIdx = 0;
            isRunning = false;
        }
        struct ValueStruct
        {
            bool isOff;
            RGBW16 topColor;
            RGBW16 botColor;
            String toString()
            {
                return ("isOff: " + String(isOff) + ", Top (" + String(topColor.r) + "," + String(topColor.g) + "," + String(topColor.b) + "," + String(topColor.w) + ")" + ", Bot (" + String(botColor.r) + "," + String(botColor.g) + "," + String(botColor.b) + "," + String(botColor.w) + ")");
            }
        };
        typedef struct ValueStruct ValueStruct;
        virtual void restart()
        {
            frameIdx = 0;
            isRunning = true;
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
        virtual bool isFinished() { return frameIdx + 1 == numFrames; }
        virtual String getName() { return "A rose by any other name is terrible because someone didn't bother to name this mode..."; }

        virtual uint8_t getId() { return 0; }
        virtual uint16_t getFadeInDuration() { return FRAMERATE / 2; } // frames
    protected:
        uint16_t frameIdx;
        uint16_t numFrames;
        bool isRunning;
    };
} // namespace Animations
#endif