#ifndef LIGHTSWITCH
#define LIGHTSWITCH
#include "defs.hpp"
#include "animationManager.hpp"
#include "utils.hpp"
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
        if (Animations::AnimationManager::getInstance()->getCurrentAnimation()->isFinished())//Won't update last state until animation is finished
        {
            if (newSwitchState != lastSwitchState)
            {
                Serial.println("Switch toggled");
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
                    Utils::delayUntilFinished();
                    Animations::AnimationManager::getInstance()->setAnimation(Animations::FunOn::getInstance());
                    Animations::AnimationManager::getInstance()->startAnimation();
                }
                else if (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::FunOn::getInstance())
                {
                    Utils::delayUntilFinished();
                    Animations::AnimationManager::getInstance()->setAnimation(Animations::FadeOff::getInstance());
                    Animations::AnimationManager::getInstance()->startAnimation();
                }
            }
            lastSwitchState = newSwitchState;
        }
    }
};
LightSwitch *LightSwitch::instance;

#endif