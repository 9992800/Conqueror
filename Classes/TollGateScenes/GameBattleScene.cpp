//
//  GameBattleScene.cpp
//  Conqueror
//
//  Created by wsli on 17/2/8.
//
//

#include "GameBattleScene.hpp"
Scene* GameBattle::createScene()
{
        auto scene = Scene::create();
        auto layer = GameBattle::create();
        scene->addChild(layer);
        
        return scene;
}

GameBattle::~GameBattle(){
}

bool GameBattle::init(){
        auto visible_size = Director::getInstance()->getVisibleSize();
        auto the_map = TMXTiledMap::create("static_map/fencengditu.tmx");
        auto map_size = the_map->getContentSize();
        the_map->setScale(visible_size.width / map_size.width);
        this->addChild(the_map);
        return true;
}

#pragma mark - update enter and exit of scenes.
void GameBattle::onEnter(){
}

void GameBattle::onExit(){
}

void GameBattle::update(float delta){
}
