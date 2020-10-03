#ifndef LIGHTSWITCH
#define LIGHTSWITCH
#include "animation/animationManager.hpp"
#include "utils.hpp"
class LightSwitch
{
private:
    static LightSwitch *instance;
    bool lastSwitchState;

public:
    LightSwitch()
    {
        lastSwitchState = !digitalRead(SW_SENSE_PIN);
    }
    static LightSwitch *getInstance()
    {
        if (instance == 0)
            instance = new LightSwitch();
        return instance;
    }
    void handleSwitchToggle()
    {
        Animations::Animation *currentAnimation = Animations::AnimationManager::getInstance()->getCurrentAnimation();
        Animations::Animation *nextAnimation = Animations::Off::getInstance();
        if(currentAnimation==Animations::TransCrossfade::getInstance()){
            currentAnimation=currentAnimation->getNextAnimation();//so a double flick from party mode goes to off
        }
        
        if (currentAnimation == Animations::Off::getInstance())
        {
            nextAnimation = Animations::On::getInstance();
        }
        else if (currentAnimation == Animations::On::getInstance())
        {
            nextAnimation = Animations::Off::getInstance();
        }
        else if (currentAnimation == Animations::TransOnOff::getInstance())
        {
            nextAnimation = Animations::Party::getInstance();
        }
        else if (currentAnimation == Animations::Party::getInstance())
        {
            nextAnimation = Animations::NightLight::getInstance();
        }
        else if (currentAnimation == Animations::TransOffOn::getInstance())
        {
            nextAnimation = Animations::Party::getInstance();
        }
        else if (currentAnimation == Animations::NightLight::getInstance())
        {
            nextAnimation = Animations::Off::getInstance();
        }
        else if (currentAnimation == Animations::SlowOn::getInstance())
        {
            nextAnimation = Animations::On::getInstance();
        }
        Animations::AnimationManager::getInstance()->doTransition(nextAnimation);
    }
    void update()
    {
        bool newSwitchState = !digitalRead(SW_SENSE_PIN);
        if (newSwitchState != lastSwitchState)
        {
            IFDEBUG(Serial.println("Physical switch toggled"));
            EmailSender::sendDebugEmail("Physical switch toggled");
            handleSwitchToggle();
        }
        lastSwitchState = newSwitchState;
    }
};
#endif