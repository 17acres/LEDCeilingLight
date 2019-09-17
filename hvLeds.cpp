#include "defs.hpp"
#include "hvLeds.hpp"

HvLeds *HvLeds::instance = 0;

HvLeds::HvLeds()
{
    tlc = new Adafruit_TLC59711(1, TLC_CLK_PIN, TLC_DATA_PIN, 10000000);
    tlc->begin();
    tlc->write();
    digitalWrite(BUF_OE_PIN, LOW);
    pinMode(BUF_OE_PIN, OUTPUT);
}

void HvLeds::update()
{
    tlc->write();
}

void HvLeds::setPowerSave(bool state)
{
    isPowerSave = state;
    digitalWrite(BUF_OE_PIN, state);
    if (state)
    {
        HvLeds::setTop(CRGB::Black, 0);
        HvLeds::setBot(CRGB::Black, 0);
    }
}
void HvLeds::setColorScale(double colorScale)
{
    colorScaleFactor = colorScale;
}
void HvLeds::setTop(CRGB color, uint16_t white)
{
    if (!isPowerSave)
    {
        RGBW16 corrected;
        if (color == (CRGB)CRGB::White)
        {
            corrected = Utils::pureWhiteCorrectRGBW(color, white);
        }
        else
        {
            corrected = Utils::colorGammaCorrectRGBW(color, white,colorScaleFactor);
        }
        tlc->setPWM(TOP_R_CHAN, corrected.r);
        tlc->setPWM(TOP_G_CHAN, corrected.g);
        tlc->setPWM(TOP_B_CHAN, corrected.b);
        tlc->setPWM(TOP_W_CHAN, corrected.w);
    }
}

void HvLeds::setBot(CRGB color, uint16_t white)
{
    if (!isPowerSave)
    {
        RGBW16 corrected;
        if (color == (CRGB)CRGB::White)
        {
            corrected = Utils::pureWhiteCorrectRGBW(color, white);
        }
        else
        {
            corrected = Utils::colorGammaCorrectRGBW(color, white,colorScaleFactor);
        }
        tlc->setPWM(BOT_R_CHAN, corrected.r);
        tlc->setPWM(BOT_G_CHAN, corrected.g);
        tlc->setPWM(BOT_B_CHAN, corrected.b);
        tlc->setPWM(BOT_W_CHAN, corrected.w);
    }
}
