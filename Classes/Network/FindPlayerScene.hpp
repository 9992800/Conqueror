//
//  FindPlayerScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//

#ifndef FindPlayerScene_hpp
#define FindPlayerScene_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
USING_NS_CC;


class FindPlayer : public cocos2d::Layer
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(FindPlayer);
        
        virtual void onEnter() override;
        virtual void onExit()override;
protected:
        void afterAnimation();
        
private:
        Sprite*         _waitingTips;
};

#endif /* FindPlayerScene_hpp */
