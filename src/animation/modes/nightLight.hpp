#ifndef NIGHTLIGHT
#define NIGHTLIGHT
#include "../animation.hpp"
#include "../../utils.hpp"
#include "../../defs.hpp"
#include "../../drivers/addrLeds.hpp"
#include "transOffOn.hpp"
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
            ((TransOffOn *)TransOffOn::getInstance())->onTime = 0;
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
            return (ValueStruct){false, Utils::rgbToRGBW16(blend(topLeft, topRight, blendPos), 0, 5), Utils::rgbToRGBW16(blend(botLeft, botRight, blendPos), 0, 5)};
        }
        Animation *getNextAnimation() override { return this; }
        String getName() override { return "Nacht Licht ist am schlestesten       (Night Light)"; }
        virtual uint16_t getFadeInDuration() override { return FRAMERATE * 2; }

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