#ifndef OFFANIM
#define OFFANIM
#include "../animation.hpp"
#include "../../utils.hpp"
#include "../../defs.hpp"
#include "../../drivers/addrLeds.hpp"
#include "transOffOn.hpp"
namespace Animations
{
    class Off : public Animation
    {
        static Off *instance;
        Off()
        {
            numFrames = 1;
        }

        ValueStruct getCurrentFrame() override
        {
            ((TransOffOn *)TransOffOn::getInstance())->onTime = 0;
            digitalWrite(LED_BUILTIN, HIGH);
            fill_solid(AddrLeds::getInstance()->vals, NUM_LEDS, CRGB::Black);
            return (ValueStruct){true, Utils::rgbToRGBW16(CRGB::Black, 0, 0), Utils::rgbToRGBW16(CRGB::Black, 0, 0)};
        }
        Animation *getNextAnimation() override { return this; }
        String getName() override { return "Darkness shall come upon the land       (Off)"; }
        virtual uint16_t getFadeInDuration() override { return FRAMERATE * 2; }

    public:
        static Animation *getInstance()
        {
            if (instance == 0)
                instance = new Off();
            return instance;
        }
    };
} // namespace Animations
#endif