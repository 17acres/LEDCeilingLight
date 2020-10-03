#ifndef HVLEDS
#define HVLEDS
#include "Adafruit_TLC59711_Dan.hpp"
#include "../utils.hpp"
#include <SPI.h>

class HvLeds{
private:
    static HvLeds* instance;
    Adafruit_TLC59711* tlc;
    HvLeds();
    bool isPowerSave;
    uint8_t colorScaleFactor;
public:
    static HvLeds* getInstance(){
        if(instance ==0)
            instance= new HvLeds();
        return instance;
    }
    void update();
    void setPowerSave(bool state);
    void setTop(RGBW16 color);
    void setBot(RGBW16 color);
    CRGB getTopRGB();
};
#endif