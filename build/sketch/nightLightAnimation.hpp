#ifndef NIGHTLIGHT
#define NIGHTLIGHT
#include "animation.hpp"
#include "utils.hpp"
#include "addrLeds.hpp"
#include "funOnAnimation.hpp"
namespace Animations
{
class NightLight : public Animation
{
    static NightLight *instance;
    NightLight()
    {
        numFrames = 2048;
    }
    ValueStruct getCurrentFrame() override
    {
        ((FunOn *)FunOn::getInstance())->onTime = 0;
        digitalWrite(LED_BUILTIN, HIGH);
        fract8 blendPos = ((float)(frameIdx % 8)) * 256.0 / 8.0;
        CHSV colorHSV = CHSV(frameIdx / 8, 255, 255);
        CRGB topLeft;
        CRGB topRight;
        CRGB botLeft;
        CRGB botRight;
        hsv2rgb_rainbow(colorHSV, topLeft);
        colorHSV.h += 1;
        hsv2rgb_rainbow(colorHSV, topRight);
        colorHSV.h += 127;
        hsv2rgb_rainbow(colorHSV, botLeft);
        colorHSV.h += 1;
        hsv2rgb_rainbow(colorHSV, botRight);
        fill_solid(AddrLeds::getInstance()->vals, NUM_LEDS, CRGB::Black);
        // delay(100);
        // Serial.print(blend(topLeft, topRight, blendPos).r);
        // Serial.print(',');
        // Serial.print(blend(topLeft, topRight, blendPos).g);
        // Serial.print(',');
        // Serial.println(blend(topLeft, topRight, blendPos).b);
        return (ValueStruct){false, blend(topLeft, topRight, blendPos), blend(botLeft, botRight, blendPos), 0, 0,.05};
    }
    Animation *getNextAnimation() override { return this; }

public:
    static Animation *getInstance()
    {
        if (instance == 0)
            instance = new NightLight();
        return instance;
    }
};
} // namespace Animations
#endif