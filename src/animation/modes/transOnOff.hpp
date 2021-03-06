#ifndef FADEOFF
#define FADEOFF
#include "../animation.hpp"
#include "off.hpp"
#include "../../drivers/addrLeds.hpp"
#include "../../utils.hpp"
#include "../../timeManager.hpp"
#include "ESP_EEPROM.h"

namespace Animations
{
    class TransOnOff : public Animation
    {
        static TransOnOff *instance;
        const uint16_t transitionPoint = 170;
        TransOnOff()
        {
            numFrames = (transitionPoint + HALF_LEDS + 1);
        }
        ValueStruct getCurrentFrame() override
        {
            ValueStruct ret;
            if (frameIdx == transitionPoint + HALF_LEDS)
            {
                digitalWrite(LED_BUILTIN, HIGH);
                ret.isOff = true;
                ret.topColor = ret.botColor = Utils::rgbToRGBW16(CRGB::Black, 0, 255);
                fill_solid(AddrLeds::vals, HALF_LEDS, CRGB::Black);
            }
            else
            {
                if (frameIdx == 0)
                {
                    time_t currentTime = TimeManager::getTime();
                    time_t onTime = ((TransOffOn *)TransOffOn::getInstance())->onTime;
                    unsigned int deltaSeconds = currentTime - onTime;
                    if ((deltaSeconds > 30u) && (deltaSeconds < 108000u))
                    {
                        unsigned int numSeconds;
                        EEPROM.get(0, numSeconds);
                        IFDEBUG(Serial.print("On Mode Second Count: "));
                        IFDEBUG(Serial.print(numSeconds));
                        IFDEBUG(Serial.print(", seconds added: "));
                        IFDEBUG(Serial.println(deltaSeconds));

                        numSeconds += deltaSeconds;
                        EEPROM.put(0, numSeconds);
                        EEPROM.commit();
                    }
                }
                // if(frameIdx<25)
                //     AddrLeds::vals[Utils::saturatingSubtract(HALF_LEDS,(frameIdx))]=CRGB::White;

                // fill_rainbow(AddrLeds::getInstance()->vals,Utils::saturatingSubtract(HALF_LEDS,(frameIdx)),frameIdx);
                //
                digitalWrite(LED_BUILTIN, LOW);
                ret.isOff = false;
                if (frameIdx < 129)
                {
                    ret.botColor = ret.topColor = Utils::rgbToRGBW16(CHSV(64 - frameIdx / 4, qmul8(frameIdx, 2), frameIdx), sin16(frameIdx * 128 + 16384) * 2, 255);
                    Utils::fill_rainbow_sv(AddrLeds::vals, HALF_LEDS, 64, 10, quadwave8(frameIdx), quadwave8(frameIdx));
                    AddrLeds::vals[HALF_LEDS - 1] = CHSV(0, 0, quadwave8(frameIdx));
                }
                else
                {
                    //ret.topColor = ret.botColor = CHSV(64 - frameIdx / 4, 255, quadwave8((frameIdx*17)/16+24));
                    ret.botColor = ret.topColor = Utils::rgbToRGBW16(CHSV(64 - frameIdx / 4, 255, max(385 - frameIdx * 2, 0)), 0, 255);
                    if (frameIdx < transitionPoint)
                    {
                        Utils::fill_rainbow_sv(AddrLeds::vals, HALF_LEDS, 64, 10, 255, 255);
                        AddrLeds::vals[HALF_LEDS - 1] = CHSV(0, 0, 255);
                    }
                    else
                    {
                        uint16_t rainbowEnd = (transitionPoint + HALF_LEDS - 1) - frameIdx;
                        fill_solid(AddrLeds::vals, HALF_LEDS, CRGB::Black);
                        Utils::fill_rainbow_sv(AddrLeds::vals, rainbowEnd, 64, 10, 255, 255);

                        AddrLeds::vals[rainbowEnd] = CHSV(0, 0, 255);
                    }
                }
            }
            Utils::mirrorAboutShortAxis(AddrLeds::getInstance()->vals);
            return ret;
        }
        Animation *getNextAnimation() override { return Off::getInstance(); }
        String getName() override { return "Then the light shall end and darkness will return       (Fade Off)"; }

    public:
        static Animation *getInstance()
        {
            if (instance == 0)
                instance = new TransOnOff();
            return instance;
        }
    };
} // namespace Animations
#endif