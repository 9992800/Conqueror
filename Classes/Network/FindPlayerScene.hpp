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
#include "AppMacros.hpp"
#include "WebSocktUtil.hpp"

USING_NS_CC;


class FindPlayer : public cocos2d::Layer
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(FindPlayer);
        virtual ~FindPlayer();
        
        virtual void onEnter() override;
        virtual void onExit()override;
protected:
        void afterAnimation();
        void onMessage(std::string);
        
private:
        Sprite*         _waitingTips;
        WebSocktUtil*   _network;
};

#endif /* FindPlayerScene_hpp */