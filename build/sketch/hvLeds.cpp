#include "defs.hpp"
#include "hvLeds.hpp"

HvLeds* HvLeds::instance=0;

HvLeds::HvLeds(){
    tlc=new Adafruit_TLC59711(1,TLC_CLK_PIN,TLC_DATA_PIN);
    tlc->begin();
    tlc->write();
    digitalWrite(BUF_OE_PIN,LOW);
    pinMode(BUF_OE_PIN,OUTPUT);
}

void HvLeds::update(){
    tlc->write();
}

void HvLeds::enterPowerSave(){
    setTop(CRGB::Black,0);
    setBot(CRGB::Black,0);
    digitalWrite(BUF_OE_PIN,HIGH);
}

void HvLeds::exitPowerSave(){
    digitalWrite(BUF_OE_PIN,LOW);
}

void HvLeds::setTop(CRGB color, uint16_t white){
    RGBW16 corrected=Utils::colorGammaCorrectRGBW(color,white);
    tlc->setPWM(TOP_R_CHAN,corrected.r);
    tlc->setPWM(TOP_G_CHAN,corrected.g);
    tlc->setPWM(TOP_B_CHAN,corrected.b);
    tlc->setPWM(TOP_W_CHAN,corrected.w);
}

void HvLeds::setBot(CRGB color, uint16_t white){
    RGBW16 corrected=Utils::colorGammaCorrectRGBW(color,white);
    tlc->setPWM(BOT_R_CHAN,corrected.r);
    tlc->setPWM(BOT_G_CHAN,corrected.g);
    tlc->setPWM(BOT_B_CHAN,corrected.b);
    tlc->setPWM(BOT_W_CHAN,corrected.w);
}
