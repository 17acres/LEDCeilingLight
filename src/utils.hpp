#ifndef UTILS
#define UTILS
#define FASTLED_ESP8266_DMA //https://github.com/coryking/FastLED NOT REGULAR LIBRARY
#include "FastLED.h"
#define FL_ALIGN_PROGMEM __attribute__((aligned(4))) //might help

#include "pgmspace.h"
#include "defs.hpp"
#include "emailSender.hpp"
struct RGBW16
{
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint16_t w;
};
typedef struct RGBW16 RGBW16;
inline RGBW16 operator/(const RGBW16 &p1, uint8_t d)
{
    return (RGBW16){p1.r / d, p1.g / d, p1.b / d, p1.w / d};
}

struct RGBPairScaled
{
    CRGB colorTop;
    CRGB colorBot;
    uint8_t colorScaleFactor;
};
typedef struct RGBW16 RGBW16;

class Utils
{
    static const uint16_t PROGMEM gammaNoDriver[65536];
    //static const uint16_t PROGMEM gammaWithDriver[65536];

public:
    //colorScaleFactor is from 1/256 to 256/256

    static RGBW16 rgbToRGBW16(CRGB color, uint16_t white, uint8_t colorScaleFactor)
    {
        RGBW16 ret = {
            ((uint16_t)color.r) * colorScaleFactor + color.r,
            ((uint16_t)color.g) * colorScaleFactor + color.g,
            ((uint16_t)color.b) * colorScaleFactor + color.b,
            white};
        return ret;
    }
    static RGBW16 colorGammaCorrectRGBW(RGBW16 in)
    {
        RGBW16 colorCorrected = {
            (((uint32_t)in.r) * 257) >> 8,
            (((uint32_t)in.g) * 145) >> 8,
            (((uint32_t)in.b) * 242) >> 8,
            in.w};

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

    // static RGBW16 pureWhiteCorrectRGBW(RGBW16 in)
    // {
    //     RGBW16 colorCorrected = {
    //         ((uint32_t)in.r) * 257,
    //         ((uint32_t)in.r) * 233,
    //         ((uint32_t)in.r) * 226,
    //         in.w};
    //     // return (RGBW16){
    //     //     pgm_read_word(&gammaWithDriver[colorCorrected.r]),
    //     //     pgm_read_word(&gammaWithDriver[colorCorrected.g]),
    //     //     pgm_read_word(&gammaWithDriver[colorCorrected.b]),
    //     //     pgm_read_word(&gammaWithDriver[colorCorrected.w])};
    //     return (RGBW16){
    //         colorCorrected.r,
    //         colorCorrected.g,
    //         colorCorrected.b,
    //         pgm_read_word(&gammaNoDriver[colorCorrected.w])};
    // }

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

    static void fill_spectrum_sv(struct CRGB *pFirstLED, int numToFill,
                                 uint8_t initialhue,
                                 uint8_t deltahue, uint8_t sat, uint8_t val)
    {
        CHSV hsv;
        hsv.hue = initialhue;
        hsv.val = val;
        hsv.sat = sat;
        for (int i = 0; i < numToFill; i++)
        {
            hsv2rgb_spectrum(hsv, pFirstLED[i]);
            hsv.hue += deltahue;
        }
    }

    static RGBW16 blendScaled(RGBW16 color1, RGBW16 color2, fract8 amountOfColor2)
    {
        RGBW16 result;

        result.r = (((uint32_t)color1.r) * (256 - amountOfColor2) + ((uint32_t)color2.r) * (amountOfColor2 + 1)) >> 8;
        result.g = (((uint32_t)color1.g) * (256 - amountOfColor2) + ((uint32_t)color2.g) * (amountOfColor2 + 1)) >> 8;
        result.b = (((uint32_t)color1.b) * (256 - amountOfColor2) + ((uint32_t)color2.b) * (amountOfColor2 + 1)) >> 8;
        result.w = (((uint32_t)color1.w) * (256 - amountOfColor2) + ((uint32_t)color2.w) * (amountOfColor2 + 1)) >> 8;
        return result;
    }
    static double temperature;
    static void updateTemp();
    static void doUpdates();
    static void delayUpdate(unsigned long mills);
    static void delayUntilFinished();
    static bool wifiPresent;
};
#endif