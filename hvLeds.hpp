#ifndef HVLEDS
#define HVLEDS
#include "Adafruit_TLC59711.h"
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
    void setTop(uint16_t r, uint16_t g, uint16_t b, uint16_t w);
    void setBot(uint16_t r, uint16_t g, uint16_t b, uint16_t w);
};
#endif