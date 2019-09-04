# 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
# 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"


# 4 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 5 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 6 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2

# 8 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2

# 10 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 11 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 12 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
HvLeds *hvLeds;
AddrLeds *addrLeds;
Animations::AnimationManager *animMan;

void setup()
{
    hvLeds = HvLeds::getInstance();
    addrLeds = AddrLeds::getInstance();
    animMan = Animations::AnimationManager::getInstance();
    Serial.begin(115200);
    //gdbstub_init();

    EEPROM.begin(sizeof(unsigned int));
    unsigned int numSeconds;
    EEPROM.get(0, numSeconds);
    Serial.print("On Mode Second Count ");
    Serial.println(numSeconds);

    pinMode(2, 0x01);
    digitalWrite(2, 0x0);
    digitalWrite(2, 0x1);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    TimeManager::setup();
    WebServer::setup();

}

void loop()
{
    Utils::doUpdates();
}
