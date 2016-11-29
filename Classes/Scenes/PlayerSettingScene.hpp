//
//  PlayerSettingScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/29.
//
//

#ifndef PlayerSettingScene_hpp
#define PlayerSettingScene_hpp

#include "cocos2d.h"
USING_NS_CC;
class PlayerSetting : public cocos2d::Layer{
public:
        static Scene* createScene();
        virtual bool init();
        CREATE_FUNC(PlayerSetting);
        ~PlayerSetting();
};

#endif /* PlayerSettingScene_hpp */
