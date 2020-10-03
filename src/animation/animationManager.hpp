#ifndef ANIMATIONMANAGER
#define ANIMATIONMANAGER
#include "animation.hpp"
#include "modes/off.hpp"
#include "modes/on.hpp"
#include "modes/transOffOn.hpp"
#include "modes/transOnOff.hpp"
#include "modes/nightLight.hpp"
#include "modes/slowOn.hpp"
#include "modes/party.hpp"
#include "modes/transCrossfade.hpp"
#include "../drivers/hvLeds.hpp"
#include "../drivers/addrLeds.hpp"
#include "../emailSender.hpp"
#include "../utils.hpp"
#include "ESP_EEPROM.h"
#include "Arduino.h"
namespace Animations
{
    class AnimationManager
    {
    private:
        static AnimationManager *instance;
        AnimationManager()
        {
            currentAnimation = Animations::Off::getInstance();
            nextAnimation = Animations::Off::getInstance();

            restoreEepromState();
            restartAnimation();
            update();
        }

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
                EmailSender::sendDebugEmail("Animation mode changed to: " + currentAnimation->getName());
                unsigned int animId;
                EEPROM.get(4, animId); //Only backup current status to EEPROM if it is a state which we care about
                if (currentAnimation->getId() != animId)
                {
                    EEPROM.put(4, currentAnimation->getId());
                    EEPROM.commit();
                }
            }
        }
        void startAnimation() { currentAnimation->start(); }
        void stopAnimation() { currentAnimation->forceEnd(); }
        void restartAnimation() { currentAnimation->restart(); }
        Animation::ValueStruct getCurrentAnimationState() { return currentAnimation->getCurrentFrame(); }
        void update()
        {
            Animation::ValueStruct vals = getCurrentAnimationState();
            HvLeds::getInstance()->setColorScale(vals.colorScaleFactor);

            if (isTemperatureOK(vals))
            { //if not, LEDs are turned off inside isTemperatureOK
                HvLeds::getInstance()->setPowerSave(vals.isOff);
                HvLeds::getInstance()->setTop(vals.topColor, vals.topWhite);
                HvLeds::getInstance()->setBot(vals.botColor, vals.botWhite);
            }

            if (currentAnimation->isFinished())
            {
                if (currentAnimation != nextAnimation)
                {
                    EmailSender::sendDebugEmail("Animation mode changed to: " + nextAnimation->getName());
                    unsigned int animId;
                    EEPROM.get(4, animId); //Only backup current status to EEPROM if it is a state which we care about
                    if (nextAnimation->getId() != animId)
                    {
                        EEPROM.put(4, nextAnimation->getId());
                        EEPROM.commit();
                    }
                }

                currentAnimation = nextAnimation;
                if (currentAnimation != TransCrossfade::getInstance()) //crossfade will already start new animation
                    currentAnimation->restart();
            }
            else
            {
                currentAnimation->nextFrame();
            }
        }
        bool isFinished() { return currentAnimation->isFinished(); }
        Animation *getCurrentAnimation() { return currentAnimation; }

        void setNextAnimation(Animation *anim)
        { //Overwrite next animation requested by by current animation
            nextAnimation = anim;
        }

        void doTransition(Animation *targetAnimation)
        {
            Serial.println("doing transition from"+getCurrentAnimation()->getName()+" to "+targetAnimation->getName());
            if (getCurrentAnimation() != targetAnimation)
            {
                Animation *transitionAnimation = TransCrossfade::getInstance();
                if(getCurrentAnimation()!=transitionAnimation)
                    ((TransCrossfade *)transitionAnimation)->sourceAnimation = getCurrentAnimation();

                
                ((TransCrossfade *)transitionAnimation)->targetAnimation = targetAnimation;

                if (currentAnimation == Off::getInstance())
                {
                    if (targetAnimation == On::getInstance())
                        transitionAnimation == TransOffOn::getInstance();
                }
                else if (currentAnimation == On::getInstance())
                {
                    if (targetAnimation == Off::getInstance())
                        transitionAnimation == TransOnOff::getInstance();
                }
                setAnimation(transitionAnimation);
                restartAnimation();
            }
        }

        void restoreEepromState()
        {
            unsigned int savedAnimationStatus;
            EEPROM.get(4, savedAnimationStatus);
            if (savedAnimationStatus == Animations::SlowOn::getInstance()->getId())
            {
                setAnimation(Animations::SlowOn::getInstance());
            }
            else if (savedAnimationStatus == Animations::Party::getInstance()->getId())
            {
                setAnimation(Animations::Party::getInstance());
            }
            else if (savedAnimationStatus == Animations::On::getInstance()->getId())
            {
                setAnimation(Animations::On::getInstance());
            }
        }

        bool isTemperatureOK(Animation::ValueStruct vals)
        {
            static bool wasHot = false;
            static bool wasWarm = false;
            if (Utils::temperature > 130)
            {
                HvLeds::getInstance()->setPowerSave(true);
                HvLeds::getInstance()->setTop(CRGB::Black, 0);
                HvLeds::getInstance()->setBot(CRGB::Black, 0);
                fill_solid(AddrLeds::vals, NUM_LEDS, CRGB::Black);
                if (!wasHot)
                {
                    EmailSender::sendEmail("I am basically on fire... RIP me... Temp is: " + String(Utils::temperature) + "F");
                    wasHot = true;
                }
            }
            else if (Utils::temperature > 120)
            {
                HvLeds::getInstance()->setPowerSave(vals.isOff);
                HvLeds::getInstance()->setTop(vals.topColor / (uint8_t)2, vals.topWhite / 2);
                HvLeds::getInstance()->setBot(vals.topColor / (uint8_t)2, vals.botWhite / 2);
                fill_solid(AddrLeds::vals, NUM_LEDS, CRGB::Black);
                if (!wasWarm)
                {
                    EmailSender::sendEmail("I'm too toasty... Oof... Temp is: " + String(Utils::temperature) + "F");
                    wasWarm = true;
                }
            }
            else
            {
                return true;
                wasHot = wasWarm = false;
            }

            return false;
        }
    };
} // namespace Animations

#endif