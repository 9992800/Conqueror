//
//  BattleFieldScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//
#include "AppMacros.hpp"
#include "MapCreator.hpp"
#include "ScreenCoordinate.hpp"
#include "BattleFieldScene.hpp"

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

Scene* BattleField::createScene(OnlineGameData* data){
        auto scene = Scene::create();
        auto layer = BattleField::create(data);
        scene->addChild(layer);
        
        return scene;
}

bool BattleField::init(){
        if ( !Layer::init() ) {
                return false;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        auto map = MapCreator::instance()->createMap(_onlineMapData->getIntMapData());
        Size map_size = map->getContentSize();
        ScreenCoordinate::getInstance()->configScreen(map_size);
        
        _onlineMapData->reshDataByMapInfo(map);
        
        this->addChild(map, ZORDER_MAP_GROUND, key_map_tag);
        
        _lowestPostion_y = visibleSize.height + origin.y - map_size.height - 6;//TODO::

        return true;
}

BattleField::~BattleField(){
        _onlineMapData->release();
}
