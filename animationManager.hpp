#ifndef ANIMATIONMANAGER
#define ANIMATIONMANAGER
#include "animation.hpp"
#include "offAnimation.hpp"
#include "hvLeds.hpp"
#include "addrLeds.hpp"

class AnimationManager{
    private:
    static AnimationManager* instance;
    AnimationManager(){currentAnimation=Animations::Off::getInstance();}

    Animation* currentAnimation;

    public:
    static AnimationManager* getInstance(){
        if(instance ==0)
            instance= new AnimationManager();
        return instance;
    }

    void setAnimation(Animation* animation){currentAnimation=animation;}
    void startAnimation(){currentAnimation->start();}
    void stopAnimation(){currentAnimation->forceEnd();}
    void restartAnimation(){currentAnimation->restart();}
    Animation::valueStruct getCurrentAnimationState(){return currentAnimation->getCurrentFrame;}
    void update(){
        Animation::valueStruct vals=getCurrentAnimationState();
        HvLeds::getInstance()->setTop(vals.topRed,vals.topGreen,vals.topBlue,vals.topWhite);
        HvLeds::getInstance()->setBot(vals.botRed,vals.botGreen,vals.botBlue,vals.botWhite);
        AddrLeds::getInstance()->vals=vals.addr;
        if(currentAnimation->getFrameIdx()+1==currentAnimation->getNumFrames()){
            currentAnimation=currentAnimation->getNextAnimation();
            currentAnimation->restart();
        }else{
            currentAnimation->nextFrame();
        }
    }

};
#endif