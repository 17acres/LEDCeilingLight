#ifndef UTILS
#define UTILS
#include "FastLED.h"
#include "pgmspace.h"
#include "defs.hpp"

struct RGBW16
{
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint16_t w;
};
typedef struct RGBW16 RGBW16;

class Utils
{
    static const uint16_t PROGMEM gammaNoDriver[65536];
    static const uint16_t PROGMEM gammaWithDriver[65536];

public:
    static RGBW16 colorGammaCorrectRGBW(CRGB in, uint16_t w)
    {
        RGBW16 colorCorrected = {
            ((uint16_t)(in.r)) * ((uint16_t)(0xFF)),
            ((uint16_t)(in.g)) * ((uint16_t)(0x90)),
            ((uint16_t)(in.b)) * ((uint16_t)(0xF0)),
            w};
        // return (RGBW16){
        //     pgm_read_word(&gammaWithDriver[colorCorrected.r]),
        //     pgm_read_word(&gammaWithDriver[colorCorrected.g]),
        //     pgm_read_word(&gammaWithDriver[colorCorrected.b]),
        //     pgm_read_word(&gammaWithDriver[colorCorrected.w])};
        return (RGBW16){
            colorCorrected.r,
            colorCorrected.g,
            colorCorrected.b,
            pgm_read_word(&gammaNoDriver[colorCorrected.w])};
    }

    static RGBW16 colorGammaCorrectRGBW8(CRGB in, uint8_t w)
    {
        colorGammaCorrectRGBW(in, ((uint16_t)w) << 8);
    }

    //Just use qmul8, qadd8, qsub8

    static uint16_t saturatingMultiply(uint16_t x, uint16_t y)
    {
        uint32_t res = (uint32_t)x * (uint32_t)y;

        uint32_t hi = res >> 16;
        uint32_t lo = res;

        return lo | -!!hi;
    }

    static void mirrorAboutLongAxis(CRGB data[48])
    {
        for (uint8_t i = HALF_LEDS; i < NUM_LEDS; i++)
        {
            data[i] = data[NUM_LEDS - 1 - i];
        }
    }
    static void mirrorAboutShortAxis(CRGB data[48])
    {
        for (uint8_t i = HALF_LEDS; i < NUM_LEDS; i++)
        {
            data[i] = data[i - HALF_LEDS];
        }
    }

    static uint16_t sinfade16(uint16_t theta)
    {
        return sin16(theta) + 32767;
    }

    static void fill_rainbow_sv(struct CRGB *pFirstLED, int numToFill,
                                uint8_t initialhue,
                                uint8_t deltahue, uint8_t sat, uint8_t val)
    {
        CHSV hsv;
        hsv.hue = initialhue;
        hsv.val = val;
        hsv.sat = sat;
        for (int i = 0; i < numToFill; i++)
        {
            pFirstLED[i] = hsv;
            hsv.hue += deltahue;
        }
    }
    static double temperature;
    static void updateTemp();
    static void doUpdates();
    static void delayUpdate(unsigned long mills);
    static void delayUntilFinished();
};
#endif