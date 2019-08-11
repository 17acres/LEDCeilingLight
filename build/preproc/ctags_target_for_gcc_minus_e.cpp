# 1 "n:\\classmate\\CeilingLight\\CeilingLight.ino"
# 1 "n:\\classmate\\CeilingLight\\CeilingLight.ino"
# 2 "n:\\classmate\\CeilingLight\\CeilingLight.ino" 2
# 3 "n:\\classmate\\CeilingLight\\CeilingLight.ino" 2
# 4 "n:\\classmate\\CeilingLight\\CeilingLight.ino" 2
# 5 "n:\\classmate\\CeilingLight\\CeilingLight.ino" 2

HvLeds* hvLeds;
double temperature=72;

CRGB leds[48];

void setup()
{
    FastLED.addLeds<WS2812B, D3, RGB>(leds, 48);


    hvLeds=HvLeds::getInstance();

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
    // digitalWrite(LED_BUILTIN,LOW);
    // hvLeds->setTop(0,0,0,0);
    // hvLeds->setBot(65535,0,0,0);
    // delayUpdate(500);
    // hvLeds->setBot(0,65535,0,0);
    // delayUpdate(500);
    // hvLeds->setBot(0,0,65535,0);
    // delayUpdate(500);
    // hvLeds->setBot(0,0,0,65535);
    // delayUpdate(500);

    // digitalWrite(LED_BUILTIN,HIGH);
    // hvLeds->setBot(0,0,0,0);
    // hvLeds->setTop(65535,0,0,0);
    // delayUpdate(500);
    // hvLeds->setTop(0,65535,0,0);
    // delayUpdate(500);
    // hvLeds->setTop(0,0,65535,0);
    // delayUpdate(500);
    // hvLeds->setTop(0,0,0,65535);
    // delayUpdate(500);
    // doUpdates();
    // hvLeds->setTop(65535,65535,65535,65535);
    // hvLeds->setBot(65535,65535,65535,65535);
    // delayUpdate(500);
    // hvLeds->setTop(5,5,5,5);
    // hvLeds->setBot(5,5,5,5);
    // delayUpdate(500);
    // hvLeds->setTop(1,1,1,1);
    // hvLeds->setBot(1,1,1,1);
    // delayUpdate(500);

    // for(int i=0;i<48;i++){
    //     if(i>0)
    //         leds[i-1]=CRGB::Black;

    //     leds[i] = CRGB::White;
    //     FastLED.show();

    //     delay(50);
    // }

    for(int i=0;i<5000;i+=1){
        hvLeds->setTop(i,i,i,i);
        hvLeds->setBot(i,i,i,i);
        delayUpdate(100);
    }
}

void doUpdates(){
    static unsigned long lastRunTime=0;
    if((millis()-lastRunTime)>10){
        lastRunTime=millis();

        HvLeds::getInstance()->update();
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
