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
private:
        GameData* _gameData;
};

#endif /* ReplayLastScene_hpp */
