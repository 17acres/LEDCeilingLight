# 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
# 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
# 2 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 3 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 4 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2

# 6 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2

HvLeds* hvLeds;
AddrLeds* addrLeds;
Animations::AnimationManager* animMan;
double temperature=72;

void setup()
{

    hvLeds=HvLeds::getInstance();
    addrLeds=AddrLeds::getInstance();
    animMan=Animations::AnimationManager::getInstance();

    Serial.begin(115200);
    gdbstub_init();
    Serial.println("TLC59711 test");
    pinMode(2, 0x01);

    while (millis() < 5000)
    {
        digitalWrite(2, 0x1);
        delay(500);
        digitalWrite(2, 0x0);
        delay(500);
    }


}

void loop()
{
    animMan->setAnimation(Animations::On::getInstance());
    animMan->startAnimation();
    delayUpdate(1000);
    animMan->setAnimation(Animations::Off::getInstance());
    animMan->startAnimation();
    delayUpdate(1000);
    animMan->setAnimation(Animations::FadeOff::getInstance());
    animMan->startAnimation();
    delayUntilFinished();
}

void doUpdates(){
    static unsigned long lastRunTime=0;
    if((millis()-lastRunTime)>10){
        lastRunTime=millis();
        animMan->update();

        hvLeds->update();
        addrLeds->update();
        updateTemp();
    }
}

void updateTemp(){
    static double intTemp;
    static const double alpha=.005;

    double raw=analogRead(A0);
    intTemp=raw*-.11818+182.6364;
    temperature = alpha * intTemp + (1-alpha) * temperature;
    Serial.println(temperature);
}

void delayUpdate(unsigned long mills){
    doUpdates();
    while(mills>=1){
        delay(1);
        doUpdates();
        mills--;
    }
}

void delayUntilFinished(){
    while(!animMan->isFinished()){
        delayUpdate(10);
    }
}
