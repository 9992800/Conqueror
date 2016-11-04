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
        ZORDER_BACK_GROUND = 0,
        ZORDER_MAP_GROUND,
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
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;

        auto back_ground = Sprite::create("starting_back.png");
        back_ground->setPosition(center);
        this->addChild(back_ground, ZORDER_BACK_GROUND);
        
        _theGameLogic = DiceGame::create();
        
        GameData* data = _theGameLogic->initGameData(_playerNumber);
        
        auto map = MapCreator::instance()->createMap(data);
        
        ScreenCoordinate::getInstance()->configScreen(map->getContentSize());
        
        data->reshDataByMapInfo(map);
        
//        _clonedGameData = _gameData.clone();
        
        
        
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

