//
//  LevelSelecteScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/2.
//
//

#include "LevelSelectScene.hpp"

Scene* LevelSelect::createScene()
{
        auto scene = Scene::create();
        auto layer = LevelSelect::create();
        scene->addChild(layer);
        
        return scene;
}

bool LevelSelect::init()
{
        if (!Layer::init()){
                return false;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto back_ground = Sprite::create("level_select_back.png");
        back_ground->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        this->addChild(back_ground, 0);
        
        return true;
}

void LevelSelect::update(float delta){
        
        if (_count < 100)
                _count++;
        else{
                _count = 0;
        }
        
        _loadingBar->setPercent(_count);
}

void LevelSelect::onExit(){
        Layer::onExit();
        unscheduleUpdate();
}
