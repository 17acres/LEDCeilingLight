# 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
# 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
# 2 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 3 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 4 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2

# 6 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 7 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
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
    pinMode(2, 0x01);
    digitalWrite(2, 0x0);
    digitalWrite(2, 0x1);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    //TimeManager::setup();
    // while (millis() < 5000)
    // {
    //     digitalWrite(LED_BUILTIN, HIGH);
    //     delay(500);
    //     digitalWrite(LED_BUILTIN, LOW);
    //     delay(500);
    // }
}

void loop()
{
    Utils::doUpdates();
}
