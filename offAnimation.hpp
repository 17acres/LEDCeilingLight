#ifndef OFFANIM
#define OFFANIM
#include "animation.hpp"
namespace Animations{
    class Off: public Animation{
        int numFrames=1;
        valueStruct getCurrentFrame(){
            return (valueStruct){true,0,0,0,0,0,0,0,0,CRGB[NUM_LEDS]};
        }
        Animation* getNextAnimation(){return instance};
    };
};
#endif