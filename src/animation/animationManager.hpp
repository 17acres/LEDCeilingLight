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
#include "modes/snapshot.hpp"
#include "../drivers/hvLeds.hpp"
#include "../drivers/addrLeds.hpp"
#include "../emailSender.hpp"
#include "../utils.hpp"
#include "ESP_EEPROM.h"

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
                currentAnimation->restart();
                setNextAnimation(currentAnimation->getNextAnimation());
                EmailSender::sendDebugEmail("Animation mode changed to: " + currentAnimation->getName(),true);
                unsigned int animId;
                EEPROM.get(4, animId); //Only backup current status to EEPROM if it is a state which we care about
                if (currentAnimation->getId() != animId)
                {
                    EEPROM.put(4, currentAnimation->getId());
                    EEPROM.commit();
                }
            }
        }
        void restartAnimation() { currentAnimation->restart(); }
        Animation::ValueStruct getCurrentAnimationState() { return currentAnimation->getCurrentFrame(); }
        void update()
        {
            Animation::ValueStruct vals = getCurrentAnimationState();

            if (isTemperatureOK(vals))
            { //if not, LEDs are turned off inside isTemperatureOK
                HvLeds::getInstance()->setPowerSave(vals.isOff);
                HvLeds::getInstance()->setTop(vals.topColor);
                HvLeds::getInstance()->setBot(vals.botColor);
            }

            if (currentAnimation->isFinished())
            {
                if (currentAnimation != nextAnimation)
                {
                    EmailSender::sendDebugEmail("Animation mode changed to: " + nextAnimation->getName(),true);
                    unsigned int animId;
                    EEPROM.get(4, animId); //Only backup current status to EEPROM if it is a state which we care about
                    if (nextAnimation->getId() != animId)
                    {
                        EEPROM.put(4, nextAnimation->getId());
                        EEPROM.commit();
                    }
                }

                if (currentAnimation == TransCrossfade::getInstance())
                { //crossfade will have already started new animation
                    currentAnimation = nextAnimation;
                }
                else
                {
                    currentAnimation = nextAnimation;
                    currentAnimation->restart();
                }
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
            if (currentAnimation != targetAnimation)
            {
                TransCrossfade *crossfadeAnimation = (TransCrossfade *)TransCrossfade::getInstance();

                if (currentAnimation == TransCrossfade::getInstance())
                {
                    ((Snapshot *)Snapshot::getInstance())->sourceAnimation = currentAnimation;
                    Snapshot::getInstance()->restart(); //update snapshot with current values
                    currentAnimation = Snapshot::getInstance();
                }

                crossfadeAnimation->sourceAnimation = currentAnimation;
                crossfadeAnimation->targetAnimation = targetAnimation;

                Animation *transitionAnimation = crossfadeAnimation;

                if (currentAnimation == Off::getInstance())
                {
                    if (targetAnimation == On::getInstance())
                        transitionAnimation = TransOffOn::getInstance();
                }
                else if (currentAnimation == On::getInstance())
                {
                    if (targetAnimation == Off::getInstance())
                        transitionAnimation = TransOnOff::getInstance();
                }
                IFDEBUG(Serial.println("doing transition from " + getCurrentAnimation()->getName() + " to " + targetAnimation->getName() + " using " + transitionAnimation->getName()));
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
                HvLeds::getInstance()->setTop(Utils::rgbToRGBW16(CRGB::Black, 0,255));
                HvLeds::getInstance()->setBot(Utils::rgbToRGBW16(CRGB::Black, 0,255));
                fill_solid(AddrLeds::vals, NUM_LEDS, CRGB::Black);
                if (!wasHot)
                {
                    EmailSender::sendEmail("I am basically on fire... RIP me... Temp is: " + String(Utils::temperature) + "F",false);
                    wasHot = true;
                }
            }
            else if (Utils::temperature > 120)
            {
                HvLeds::getInstance()->setPowerSave(vals.isOff);
                HvLeds::getInstance()->setTop(vals.topColor / 2);
                HvLeds::getInstance()->setBot(vals.topColor / 2);
                fill_solid(AddrLeds::vals, NUM_LEDS, CRGB::Black);
                if (!wasWarm)
                {
                    EmailSender::sendEmail("I'm too toasty... Oof... Temp is: " + String(Utils::temperature) + "F",false);
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