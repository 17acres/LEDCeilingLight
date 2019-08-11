#ifndef HVLEDS
#define HVLEDS
#include "Adafruit_TLC59711.h"
#include "utils.hpp"
#include <SPI.h>

class HvLeds{
private:
    static HvLeds* instance;
    Adafruit_TLC59711* tlc;
    HvLeds();
public:
    static HvLeds* getInstance(){
        if(instance ==0)
            instance= new HvLeds();
        return instance;
    }
    void update();
    void enterPowerSave();
    void exitPowerSave();
    void setTop(CRGB color, uint16_t white);
    void setBot(CRGB color, uint16_t white);
};
#endif