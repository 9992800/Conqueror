//
//  DiceGameScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//
#include "GameData.hpp"
#include "MapCreator.hpp"
#include "GameScene.hpp"

enum{
        ZORDER_MAP_GROUND = 0,
        ZORDER_CRTL_LAYERS,
        ZORDER_ANIM_LAYER,
        ZORDER_DIALOG_LAYER
};
enum{
        key_map_tag             = 1,
        key_ctrl_layer_tag,
        key_anim_layer_tag,
        key_dialog_layer_tag
};

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
        this->initMapLayer();
        
        this->initControlLayer();
        
        this->initAnimationLayer();
        
        return true;
}

void GameScene::initMapLayer(){        
        
        _theGameLogic = DiceGame::create();
        
        _gameData = _theGameLogic->initGameData(_playerNumber);
        
        _clonedGameData = _gameData.clone();
        
        auto map = MapCreator::instance()->createMap(_gameData);
        
        this->addChild(map, ZORDER_MAP_GROUND, key_map_tag);
}

void GameScene::initControlLayer(){
        
}

void GameScene::initAnimationLayer(){
        
}
