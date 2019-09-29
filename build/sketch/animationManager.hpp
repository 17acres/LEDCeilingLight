#ifndef ANIMATIONMANAGER
#define ANIMATIONMANAGER
#include "animation.hpp"
#include "offAnimation.hpp"
#include "onAnimation.hpp"
#include "fadeOffAnimation.hpp"
#include "funOnAnimation.hpp"
#include "nightLightAnimation.hpp"
#include "slowOnAnimation.hpp"
#include "funAnimation.hpp"
#include "hvLeds.hpp"
#include "addrLeds.hpp"
#include "emailSender.hpp"
namespace Animations
{
class AnimationManager
{
private:
    static AnimationManager *instance;
    AnimationManager() { currentAnimation = Animations::Off::getInstance(); nextAnimation= Animations::Off::getInstance();}

    Animation *currentAnimation;
    Animation *nextAnimation;

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
            setNextAnimation(currentAnimation->getNextAnimation());
            EmailSender::sendDebugEmail("Animation mode changed to: "+currentAnimation->getName());
        }
    }
    void startAnimation() { currentAnimation->start(); }
    void stopAnimation() { currentAnimation->forceEnd(); }
    void restartAnimation() { currentAnimation->restart(); }
    Animation::ValueStruct getCurrentAnimationState() { return currentAnimation->getCurrentFrame(); }
    void update()
    {
        static bool wasHot=false;
        static bool wasWarm=false;
        Animation::ValueStruct vals = getCurrentAnimationState();
        HvLeds::getInstance()->setColorScale(vals.colorScaleFactor);
        if (Utils::temperature > 130)
        {
            HvLeds::getInstance()->setPowerSave(true);
            HvLeds::getInstance()->setTop(CRGB::Black, 0);
            HvLeds::getInstance()->setBot(CRGB::Black, 0);
            fill_solid(AddrLeds::vals, NUM_LEDS, CRGB::Black);
            if(!wasHot){
                EmailSender::sendEmail("I am basically on fire... RIP me... Temp is: "+String(Utils::temperature)+"F");
                wasHot=true;
            }
            
        }
        else if (Utils::temperature > 120)
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
            if(!wasWarm){
                EmailSender::sendEmail("I'm too toasty... Oof... Temp is: "+String(Utils::temperature)+"F");
                wasWarm=true;
            }
            
        }
        else
        {
            wasHot=wasWarm=false;
            HvLeds::getInstance()->setPowerSave(vals.isOff);
            HvLeds::getInstance()->setTop(vals.topColor, vals.topWhite);
            HvLeds::getInstance()->setBot(vals.botColor, vals.botWhite);
        }

        if (currentAnimation->isFinished())
        {
            if(currentAnimation!=nextAnimation)
                EmailSender::sendDebugEmail("Animation mode changed to: "+nextAnimation->getName());
            currentAnimation = nextAnimation;
            currentAnimation->restart();
        }
        else
        {
            currentAnimation->nextFrame();
        }
    }
    bool isFinished() { return currentAnimation->isFinished(); }
    Animation *getCurrentAnimation() { return currentAnimation; }

    void setNextAnimation(Animation *anim){//Overwrite next animation requested by by current animation
        nextAnimation=anim;
    }
};
} // namespace Animations

#endif