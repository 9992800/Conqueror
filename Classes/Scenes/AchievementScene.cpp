//
//  AchievementScene.cpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/12/27.
//
//

#include "AchievementScene.hpp"
Scene* Achievement::createScene(){
        auto scene = Scene::create();
        auto layer = Achievement::create();
        scene->addChild(layer);
        return scene;
}

bool Achievement::init() {
        return true;
} 
