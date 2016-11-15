//
//  ReplayLastScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/14.
//
//
#include "GameData.hpp"
#include "ReplayLastScene.hpp"
#include "MapCreator.hpp"
#include "ScreenCoordinate.hpp"
#include "base/CCAsyncTaskPool.h"


Scene* ReplayLast::createScene(GameData* data){
        auto scene = Scene::create();
        auto layer = ReplayLast::create(data);
        
        scene->addChild(layer);
        return scene;
}


#pragma mark - init things
bool ReplayLast::init(){
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto map = MapCreator::instance()->createMap(_gameData);
        Size map_size = map->getContentSize();
        ScreenCoordinate::getInstance()->configScreen(map_size);
        
        _gameData->reshDataByMapInfo(map);
        
        this->addChild(map, 0, 1);
        
        _lowestPostion_y = visibleSize.height + origin.y - map_size.height - 6;//TODO::
        
        Director::getInstance()->setDepthTest(true);
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesMoved = CC_CALLBACK_2(ReplayLast::onTouchesMoved, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        
        auto return_back = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                                 CC_CALLBACK_1(ReplayLast::menuExit, this));
        return_back->setPosition(Vec2(origin.x + return_back->getContentSize().width + 10,
                                      origin.y + visibleSize.height - return_back->getContentSize().height - 10));
        
        auto  start_show = MenuItemImage::create("start.png", "start.png",
                                                 CC_CALLBACK_1(ReplayLast::menuStartShow, this));
        start_show->setPosition(origin + visibleSize / 2);
        
        auto menu = Menu::create(start_show, return_back, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 100);
        
        return true;
}

ReplayLast::~ReplayLast(){
        _gameData->release();
}


#pragma mark - UI action things
void ReplayLast::onTouchesMoved(const std::vector<Touch*>& touches, Event* event){
        auto touch = touches[0];
        auto diff = touch->getDelta();
        
        diff.x = 0;
        auto map = this->getChildByTag(1);
        auto currentPos = map->getPosition();
        auto origin = Director::getInstance()->getVisibleOrigin();
        
        if (origin.y < (currentPos.y + diff.y)){
                diff.y = origin.y - currentPos.y;
        }
        
        if ((currentPos.y + diff.y) < _lowestPostion_y){
                diff.y = _lowestPostion_y - currentPos.y;
        }
        
        map->setPosition(currentPos + diff);
}

void ReplayLast::menuExit(Ref* pSender){
        Director::getInstance()->popScene();
}

void ReplayLast::menuStartShow(Ref* pSender){
        _hisFrom = parseData<int>(GAME_HISTORY_FROM_KEY);
        _hisTo   = parseData<int>(GAME_HISTORY_TO_KEY);
        _hisRes  = parseData<int>(GAME_HISTORY_RES_KEY);
        _timeCounter = 0;
        _dataIdx = 0;
        scheduleUpdate();
}


#pragma mark - update and enter or exit things
void ReplayLast::onEnter(){
        Layer::onEnter(); 
}


void ReplayLast::update(float delta){
        
}

void ReplayLast::onExit(){
        Layer::onExit();
        unscheduleUpdate();
}


#pragma mark - history show
void ReplayLast::playHistory(float delta){
        
        
        if (_dataIdx >= _hisFrom.size()){
                //TODO::Show Dialog and Pause;
                Director::getInstance()->pause();
        }
        
        if (_timeCounter % 4 == 0){
                int from = _hisFrom[_dataIdx];
                _gameData->_areaData[from]->drawAsSelected();
                
        }
        else if (_timeCounter % 4 == 1){
                int to = _hisTo[_dataIdx];
                _gameData->_areaData[to]->drawAsSelected();
                
        }else if (_timeCounter % 4 == 2){
                int res = _hisRes[_dataIdx];
                int from = _hisFrom[_dataIdx];
                int to = _hisTo[_dataIdx];
                
                AreaData* area_to       = _gameData->_areaData[to];
                AreaData* area_from     = _gameData->_areaData[from];
                
                auto map = _gameData->_refereMap;
                if (ATTACK_RES_WIN == res){
                        int new_owner = _gameData->_areaData[from]->getOwner();
                        TMXLayer * layer = map->getLayer(LAYER_NAME_IN_TILE_MAP);
                        for (int j = 0; j < CEL_MAX; j++){
                                if (to != _gameData->_cel[j]){
                                        continue;
                                }
                                int col = j / XMAX;
                                int row = j % XMAX;
                                
                                layer->setTileGID(GamePlayer::getGid(new_owner), Vec2(row, col));
                        }
                        area_to->setOwner(new_owner);
                        area_to->setDice(area_from->getDice() - 1);
                        area_to->updatePawn(map);
                        
                        area_from->initDice();
                        area_from->updatePawn(map);
                        
                }else if (ATTACK_RES_DEFEATED == res){
                        area_from->initDice();
                        area_from->updatePawn(map);
                        
                }else if (ATTACK_RES_GOTSUPPLY == res){
                        
                }else if (ATTACK_RES_NONE == res){
                        
                }
                
        }else if (_timeCounter % 4 == 3){
                int from = _hisFrom[_dataIdx];
                int to = _hisTo[_dataIdx];
                _gameData->_areaData[from]->drawAsUnselected();
                _gameData->_areaData[to]->drawAsUnselected();
                _dataIdx ++;
        }
        
        ++_timeCounter;
}
