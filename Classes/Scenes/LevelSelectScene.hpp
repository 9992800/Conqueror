//
//  LevelSelecteScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/2.
//
//
#ifndef LevelSelecteScene_hpp
#define LevelSelecteScene_hpp

#include "AppMacros.hpp"
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
        
        void onTouchesMoved(const std::vector<Touch*>& touches, Event* event)override;
        
protected:
        void menuSelectLevel(Ref* btn, int num);
        void menuBuyLevel(Ref* btn, int num);
        
private:
        void loadLevelShow(Vec2 center, Size visibleSize);
        void setSelectLevelBackPos();
        void loadLevelSelectedBackGround();
private:
        int             _count;
        int             _lastLevel;
        LoadingBar*     _loadingBar;
        LayerColor*     _selectedBackGround;
        LayerColor*     _levelShowBackGround;
};


#endif /* LevelSelecteScene_hpp */
