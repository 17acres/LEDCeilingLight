#ifndef UTILS
#define UTILS
#include "FastLED.h"
#include "pgmspace.h"

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
};

#endif