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
#include "ScreenCoordinate.hpp"

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


#pragma mark - constructor
Scene* GameScene::createScene(int gameLevel)
{
        auto scene = Scene::create();
        auto layer = GameScene::create(gameLevel);
        
        scene->addChild(layer); 
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

#pragma mark - initilization
void GameScene::initMapLayer(){        
        
        _theGameLogic = DiceGame::create();
        
        GameData* data = _theGameLogic->initGameData(_playerNumber);
        
        auto map = MapCreator::instance()->createMap(data);
        
        data->reshDataByMapInfo(map);
        
//        _clonedGameData = _gameData.clone();
        
        ScreenCoordinate::getInstance()->configScreen(map->getContentSize());
        
        this->addChild(map, ZORDER_MAP_GROUND, key_map_tag);
}

void GameScene::initControlLayer(){
        _controlLayer = Layer::create();
        //TODO:: add controll buttons
        this->addChild(_controlLayer, ZORDER_CRTL_LAYERS, key_ctrl_layer_tag);
}

void GameScene::initAnimationLayer(){
        _animationLayer = Layer::create();
        
        //TODO::add fighting animaiton objects
        this->addChild(_animationLayer, ZORDER_ANIM_LAYER, key_anim_layer_tag);
        _animationLayer->setVisible(false);
}

