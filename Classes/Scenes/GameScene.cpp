//
//  DiceGameScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//

#include "GameScene.hpp"
Scene* GameScene::createScene()
{
        auto scene = Scene::create();
        auto layer = GameScene::create();
        scene->addChild(layer);
        
        return scene;
}

bool GameScene::init()
{
        if (!Layer::init()){
                return false;
        }
        
        return true;
}

