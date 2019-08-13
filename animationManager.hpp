#ifndef ANIMATIONMANAGER
#define ANIMATIONMANAGER
#include "animation.hpp"
#include "offAnimation.hpp"
#include "onAnimation.hpp"
#include "fadeOffAnimation.hpp"
#include "fadeOnAnimation.hpp"
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
            HvLeds::getInstance()->setPowerSave(vals.isOff);
            HvLeds::getInstance()->setTop(vals.topColor, vals.topWhite);
            HvLeds::getInstance()->setBot(vals.botColor, vals.botWhite);
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
    };
} // namespace Animations:
Animations::AnimationManager *Animations::AnimationManager::instance;

#endif