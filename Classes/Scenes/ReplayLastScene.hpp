//
//  ReplayLastScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/14.
//
//

#ifndef ReplayLastScene_hpp
#define ReplayLastScene_hpp
#include "AppMacros.hpp"
#include "cocos2d.h"
USING_NS_CC;

class ReplayLast : public cocos2d::Layer{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(ReplayLast);
        virtual ~ReplayLast();
        
        void update(float delta)override;
        virtual void onExit()override;
        
private:
        void onTouchesMoved(const std::vector<Touch*>& touches, Event* event)override;
        
        void menuExit(Ref* pSender);
        void menuStartShow(Ref* pSender);
private:
        float           _lowestPostion_y;
        GameData*       _gameData;
};

#endif /* ReplayLastScene_hpp */
