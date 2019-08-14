#ifndef LIGHTSWITCH
#define LIGHTSWITCH
#include "defs.hpp"
#include "animationManager.hpp"
class LightSwitch
{
private:
    static LightSwitch *instance;

public:
    static LightSwitch *getInstance()
    {
        if (instance == 0)
            instance = new LightSwitch();
        return instance;
    }

    void update()
    {
        static bool lastSwitchState;
        bool newSwitchState = digitalRead(SW_SENSE_PIN);
        if (newSwitchState != lastSwitchState)
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
                delayUntilFinished();
                Animations::AnimationManager::getInstance()->setAnimation(Animations::FunOn::getInstance());
                Animations::AnimationManager::getInstance()->startAnimation();
            }
            else if (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::FunOn::getInstance())
            {
                delayUntilFinished();
                Animations::AnimationManager::getInstance()->setAnimation(Animations::FadeOff::getInstance());
                Animations::AnimationManager::getInstance()->startAnimation();
            }
        }
        lastSwitchState = newSwitchState;
    }
};
LightSwitch *LightSwitch::instance;

#endif