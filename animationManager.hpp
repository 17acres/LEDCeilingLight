#ifndef ANIMATIONMANAGER
#define ANIMATIONMANAGER
#include "animation.hpp"
#include "offAnimation.hpp"
#include "onAnimation.hpp"
#include "fadeOffAnimation.hpp"
#include "funOnAnimation.hpp"
#include "hvLeds.hpp"
#include "addrLeds.hpp"
namespace Animations
{
class AnimationManager
{
private:
    static AnimationManager *instance;
    AnimationManager() { currentAnimation = Animations::Off::getInstance(); }

    Animation *currentAnimation;

public:
    static AnimationManager *getInstance()
    {
        if (instance == 0)
            instance = new AnimationManager();
        return instance;
    }

    void setAnimation(Animation *animation)
    {
        if (animation != currentAnimation)
        {
            currentAnimation = animation;
            currentAnimation->reset();
        }
    }
    void startAnimation() { currentAnimation->start(); }
    void stopAnimation() { currentAnimation->forceEnd(); }
    void restartAnimation() { currentAnimation->restart(); }
    Animation::ValueStruct getCurrentAnimationState() { return currentAnimation->getCurrentFrame(); }
    void update()
    {
        Animation::ValueStruct vals = getCurrentAnimationState();
        if (temperature > 130)
        {
            HvLeds::getInstance()->setPowerSave(true);
            HvLeds::getInstance()->setTop(CRGB::Black, 0);
            HvLeds::getInstance()->setBot(CRGB::Black, 0);
            fill_solid(AddrLeds::vals, NUM_LEDS, CRGB::Black);
        }
        else if (temperature > 120)
        {
            if (vals.topColor > (CRGB)CRGB::Black)
            {
                HvLeds::getInstance()->setPowerSave(vals.isOff);
                HvLeds::getInstance()->setTop(CRGB::Black, vals.topWhite);
                HvLeds::getInstance()->setBot(CRGB::Black, vals.botWhite);
                fill_solid(AddrLeds::vals, NUM_LEDS, CRGB::Black);
            }
            else
            {
                HvLeds::getInstance()->setPowerSave(vals.isOff);
                HvLeds::getInstance()->setTop(CRGB::Black, vals.topWhite / 2);
                HvLeds::getInstance()->setBot(CRGB::Black, vals.botWhite / 2);
                fill_solid(AddrLeds::vals, NUM_LEDS, CRGB::Black);
            }
        }
        else
        {
            HvLeds::getInstance()->setPowerSave(vals.isOff);
            HvLeds::getInstance()->setTop(vals.topColor, vals.topWhite);
            HvLeds::getInstance()->setBot(vals.botColor, vals.botWhite);
        }

        if (currentAnimation->isFinished())
        {
            currentAnimation = currentAnimation->getNextAnimation();
            currentAnimation->restart();
        }
        else
        {
            currentAnimation->nextFrame();
        }
    }
    bool isFinished() { return currentAnimation->isFinished(); }
    Animation *getCurrentAnimation() { return currentAnimation; }
};
} // namespace Animations
Animations::AnimationManager *Animations::AnimationManager::instance;

#endif