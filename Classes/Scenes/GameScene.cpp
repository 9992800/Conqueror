//
//  DiceGameScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//

#include "GameScene.hpp"
Scene* GameScene::createScene(int gameLevel)
{
        auto scene = Scene::create();
        auto layer = GameScene::create();
        scene->addChild(layer);
        layer->setLevel(gameLevel);
        return scene;
}

bool GameScene::init()
{
        if (!Layer::init()){
                return false;
        }
        
        
        return true;
}

