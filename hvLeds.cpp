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
    setTop(0,0,0,0);
    setBot(0,0,0,0);
    digitalWrite(BUF_OE_PIN,HIGH);
}

void HvLeds::exitPowerSave(){
    digitalWrite(BUF_OE_PIN,LOW);
}

void HvLeds::setTop(uint16_t r, uint16_t g, uint16_t b, uint16_t w){
    tlc->setPWM(TOP_R_CHAN,r);
    tlc->setPWM(TOP_G_CHAN,g);
    tlc->setPWM(TOP_B_CHAN,b);
    tlc->setPWM(TOP_W_CHAN,w);
}

void HvLeds::setBot(uint16_t r, uint16_t g, uint16_t b, uint16_t w){
    tlc->setPWM(BOT_R_CHAN,r);
    tlc->setPWM(BOT_G_CHAN,g);
    tlc->setPWM(BOT_B_CHAN,b);
    tlc->setPWM(BOT_W_CHAN,w);
}
