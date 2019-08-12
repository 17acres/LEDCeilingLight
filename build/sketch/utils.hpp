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
            ((uint16_t)(in.r)) * ((uint16_t)(CRGB(TypicalSMD5050).r)),
            ((uint16_t)(in.g)) * ((uint16_t)(CRGB(TypicalSMD5050).g)),
            ((uint16_t)(in.b)) * ((uint16_t)(CRGB(TypicalSMD5050).b)),
            w};
        return (RGBW16){
            pgm_read_word(&gammaWithDriver[colorCorrected.r]),
            pgm_read_word(&gammaWithDriver[colorCorrected.g]),
            pgm_read_word(&gammaWithDriver[colorCorrected.b]),
            pgm_read_word(&gammaWithDriver[colorCorrected.w])};
    }

    static RGBW16 colorGammaCorrectRGBW8(CRGB in, uint8_t w)
    {
        colorGammaCorrectRGBW(in, ((uint16_t)w) << 8);
    }

    //https://stackoverflow.com/questions/33481295/saturating-subtract-add-for-unsigned-bytes
    static uint16_t saturatingSubtract(uint16_t minuend, uint16_t subtrahend)
    {
        uint16_t res = minuend - subtrahend;
        res &= -(res <= minuend);
        return res;
    }

    static uint16_t saturatingMultiply(uint16_t x, uint16_t y)
    {
        uint32_t res = (uint32_t)x * (uint32_t)y;

        uint32_t hi = res >> 16;
        uint32_t lo = res;

        return lo | -!!hi;
    }

    static void mirrorAboutLongAxis(CRGB data[48]){
        for(uint8_t i=HALF_LEDS;i<NUM_LEDS;i++){
            data[i]=data[NUM_LEDS-1-i];
        }
    }
    static void mirrorAboutShortAxis(CRGB data[48]){
        for(uint8_t i=HALF_LEDS;i<NUM_LEDS;i++){
            data[i]=data[i-HALF_LEDS];
        }
    }
};

#endif