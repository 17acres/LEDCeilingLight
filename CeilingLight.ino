#include <GDBStub.h>
#include "hvLeds.hpp"
#include "addrLeds.hpp"
#include "utils.hpp"
#include "animationManager.hpp"
#include "timeManager.hpp"
#include "lightSwitch.hpp"
#include "EEPROM.h"
#include "webServer.hpp"
HvLeds *hvLeds;
AddrLeds *addrLeds;
Animations::AnimationManager *animMan;

void setup()
{
    Serial.begin(115200);//Do this before RX pin is repurposed for dma LEDs (i2s)
    hvLeds = HvLeds::getInstance();
    addrLeds = AddrLeds::getInstance();
    animMan = Animations::AnimationManager::getInstance();
    
    //gdbstub_init();

    EEPROM.begin(sizeof(unsigned int));
    unsigned int numSeconds;
    EEPROM.get(0, numSeconds);
    Serial.print("On Mode Second Count ");
    Serial.println(numSeconds);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    TimeManager::setup();
    WebServer::setup();
    EmailSender::setup();
    EmailSender::sendEmail("Ceiling light started - On Mode Second Count: "+String(numSeconds));
}

void loop()
{
    EmailSender::runSpooler(Utils::doUpdates);
}

