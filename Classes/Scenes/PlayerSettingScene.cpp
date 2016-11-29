//
//  PlayerSettingScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/29.
//
//

#include "PlayerSettingScene.hpp"
PlayerSetting::~PlayerSetting(){
        
}
bool PlayerSetting::init(){
        auto back_ground = Sprite::create("level_select_back.png");
        this->addChild(back_ground);
        return true;
}

Scene* PlayerSetting::createScene(){
        auto scene = Scene::create();
        auto layer = PlayerSetting::create();
        scene->addChild(layer);
        
        return scene;
}
