#ifndef FADEOFF
#define FADEOFF
#include "animation.hpp"
#include "offAnimation.hpp"
#include "FastLED.h"
#include "addrLeds.hpp"
namespace Animations
{
class FadeOff : public Animation
{
    static FadeOff *instance;
    FadeOff(){
        numFrames = 512;
    }
    ValueStruct getCurrentFrame() override
    {
        fill_solid(AddrLeds::vals,HALF_LEDS,CRGB::Black);
        // if(frameIdx<25)
        //     AddrLeds::vals[Utils::saturatingSubtract(HALF_LEDS,(frameIdx))]=CRGB::White;

        // fill_rainbow(AddrLeds::getInstance()->vals,Utils::saturatingSubtract(HALF_LEDS,(frameIdx)),frameIdx);
        // Utils::mirrorAboutShortAxis(AddrLeds::getInstance()->vals);  

        ValueStruct ret;
        ret.isOff=false;
        if(frameIdx<129){
            ret.topWhite=65535-Utils::saturatingMultiply(frameIdx,512);
            ret.botWhite=ret.topWhite;

            ret.topColor=CHSV(frameIdx,qmul8(frameIdx,2),qmul8(frameIdx,2));
            ret.botColor=ret.topColor;
        }else{
            ret.topWhite=ret.botWhite=0;
            ret.topColor=ret.botColor=CHSV(129,255,255);
        }
        Serial.println("Frame "+String(frameIdx)+": "+ret.toString());
        return ret;
        
    }
    Animation *getNextAnimation() override { return Off::getInstance(); }

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new FadeOff();
        return instance;
    }
};
} // namespace Animations

Animations::FadeOff *Animations::FadeOff::instance;
#endif