//
//  BattleFieldScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//
#include "AppMacros.hpp"
#include "BattleFieldScene.hpp"

Scene* BattleField::createScene(int idx){
        auto scene = Scene::create();
        auto layer = BattleField::create(idx);
        scene->addChild(layer);
        
        return scene;
}

bool BattleField::init(){
        
        this->initMap();
        
        return true;
}

BattleField::~BattleField(){
}

void BattleField::initMap(){
        std::string map_name = "maps/online_map";
        map_name.append(tostr(_mapIdx));
        map_name.append(".tmx");
        auto map = TMXTiledMap::create(map_name);
        
        auto layer = map->getLayer("online_map");
        uint32_t* tiles = layer->getTiles();
        for (int i = 0; i < CEL_MAX; i++){
                printf("\t -%d-", (int)tiles[i]);
        }
        this->addChild(map);
}
