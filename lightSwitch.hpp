#ifndef LIGHTSWITCH
#define LIGHTSWITCH
#include "defs.hpp"
#include "animationManager.hpp"
#include "utils.hpp"
class LightSwitch
{
private:
    static LightSwitch *instance;
    bool lastSwitchState;
public:
    LightSwitch(){
        lastSwitchState=!digitalRead(SW_SENSE_PIN);
    }
    static LightSwitch *getInstance()
    {
        if (instance == 0)
            instance = new LightSwitch();
        return instance;
    }
    void handleSwitchToggle()
    {
        if (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::Off::getInstance())
        {
            Animations::AnimationManager::getInstance()->setAnimation(Animations::FunOn::getInstance());
            Animations::AnimationManager::getInstance()->startAnimation();
        }
        else if (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::On::getInstance())
        {
            Animations::AnimationManager::getInstance()->setAnimation(Animations::FadeOff::getInstance());
            Animations::AnimationManager::getInstance()->startAnimation();
        }
        else if (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::FadeOff::getInstance())
        {
            Animations::AnimationManager::getInstance()->setNextAnimation(Animations::Fun::getInstance());
        }
        else if (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::Fun::getInstance())
        {
            Animations::AnimationManager::getInstance()->setAnimation(Animations::NightLight::getInstance());
            Animations::AnimationManager::getInstance()->startAnimation();
        }
        else if (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::FunOn::getInstance())
        {
            Animations::AnimationManager::getInstance()->setAnimation(Animations::SlowOn::getInstance());
            Animations::AnimationManager::getInstance()->startAnimation();
        }
        else if (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::NightLight::getInstance())
        {
            Animations::AnimationManager::getInstance()->setAnimation(Animations::Off::getInstance());
            Animations::AnimationManager::getInstance()->startAnimation();
        }
        else if (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::SlowOn::getInstance())
        {
            Animations::AnimationManager::getInstance()->setAnimation(Animations::On::getInstance());
            Animations::AnimationManager::getInstance()->startAnimation();
        }
    }
    void update()
    {
        bool newSwitchState = !digitalRead(SW_SENSE_PIN);
        if (newSwitchState != lastSwitchState)
        {
            Serial.println("Physical switch toggled");
            EmailSender::sendEmail("Physical switch toggled");
            handleSwitchToggle();         
        }
        lastSwitchState = newSwitchState;
    }
};
#endif