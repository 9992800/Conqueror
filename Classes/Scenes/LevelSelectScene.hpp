//
//  LevelSelecteScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/2.
//
//

#ifndef LevelSelecteScene_hpp
#define LevelSelecteScene_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;


class LevelSelect : public cocos2d::Layer
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(LevelSelect);
        
        void update(float delta)override;
        virtual void onExit()override;
        
private:
        void loadLevelShow(Vec2 center, Size visibleSize);
private:
        int _count = 0;
        LoadingBar*     _loadingBar;        
};


#endif /* LevelSelecteScene_hpp */
