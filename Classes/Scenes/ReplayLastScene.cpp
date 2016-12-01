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
#include "AppMacros.hpp"

enum{
        HISTORY_STATUS_INIT = 1,
        HISTORY_STATUS_PLAYING,
        HISTORY_STATUS_FINISHED
};

Scene* ReplayLast::createScene(HistoryReplayData data){
        auto scene = Scene::create();
        auto layer = ReplayLast::create(data);
        
        scene->addChild(layer);
        return scene;
}


#pragma mark - init things
bool ReplayLast::init(){
        
        if ( !Layer::init() ) {
                return false;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        

        auto back_layer = LayerColor::create(TILE_COLOR_BACKGRUND);//TILE_COLOR_BACKGRUND  //Color4B::WHITE
        back_layer->setContentSize(visibleSize+Size(10,10));
        ScreenCoordinate::getInstance()->configScreen(visibleSize);
        _gameData->reshDataByBackGrnd(back_layer);
        this->addChild(back_layer, 0, 1);
        _lowestPostion_y = visibleSize.height + origin.y - visibleSize.height - 6;//TODO::
     
        
        
        Director::getInstance()->setDepthTest(true);
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesMoved = CC_CALLBACK_2(ReplayLast::onTouchesMoved, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        
        auto return_back = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                                 CC_CALLBACK_1(ReplayLast::menuExit, this));
        return_back->setPosition(Vec2(origin.x + return_back->getContentSize().width + 10,
                                      origin.y + visibleSize.height - return_back->getContentSize().height - 10));
        
        _startBtn = MenuItemImage::create("start.png", "start.png",
                                                 CC_CALLBACK_1(ReplayLast::menuStartShow, this));
        _startBtn->setPosition(origin + visibleSize / 2);
        
        auto menu = Menu::create(_startBtn, return_back, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 100);
        
        return true;
}

ReplayLast::ReplayLast(HistoryReplayData data):_gameData(data.gameData),
_timeCounter(0),_dataIdx(0),
_hisFrom(data.from),
_hisTo(data.to),
_hisRes(data.res),
_hisSupply(data.sup),
_curStatus(HISTORY_STATUS_INIT){
        _clonedGameData = GameData::createWithData(_gameData);
        _clonedGameData->retain();
        
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("anim/zssl.plist", "anim/zssl.png");
}

ReplayLast::~ReplayLast(){
        _gameData->release();
        _clonedGameData->release();
        
        SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("anim/zssl.plist");
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


void ReplayLast::resetHistoryData(){
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto layer = _gameData->_referedLayer;
        layer->removeFromParentAndCleanup(true);
        _gameData->release();
        
        _gameData = GameData::createWithData(_clonedGameData);
        _gameData->retain();
        
        layer = LayerColor::create(TILE_COLOR_BACKGRUND, visibleSize.width + 10, visibleSize.height + 20);
        ScreenCoordinate::getInstance()->configScreen(visibleSize);
        _gameData->reshDataByBackGrnd(layer);
        
        this->addChild(layer, 0, 1);
}
void ReplayLast::menuStartShow(Ref* pSender){
        if (HISTORY_STATUS_PLAYING == _curStatus){
                return;
        }else if (HISTORY_STATUS_FINISHED == _curStatus){
                this->resetHistoryData();
        }
        
        _curStatus  = HISTORY_STATUS_PLAYING;
        _startBtn->setVisible(false);
        _timeCounter = 0;
        _dataIdx = 0;
        scheduleUpdate();
}


#pragma mark - update and enter or exit things
void ReplayLast::onEnter(){
        Layer::onEnter(); 
}


void ReplayLast::update(float delta){
        if (_curStatus == HISTORY_STATUS_PLAYING)
                this->playHistory(delta);
}

void ReplayLast::onExit(){
        Layer::onExit();
        unscheduleUpdate();
}


#pragma mark - history show
void ReplayLast::playHistory(float delta){
        
        
        if (_dataIdx >= _hisFrom.size()){
                _curStatus  = HISTORY_STATUS_FINISHED;
                _startBtn->setVisible(true);
                unscheduleUpdate();
                return;
        }
        
        int res = _hisRes[_dataIdx];
        int from = _hisFrom[_dataIdx];
        int to = _hisTo[_dataIdx];
        
        auto map = _gameData->_referedLayer;
 
        if (ATTACK_RES_GOTSUPPLY == res){
                
                for (std::vector<Vec2>::iterator it = _hisSupply.begin() + from;
                     it != _hisSupply.begin() + to; it++){
                        Vec2 sup = *it;
                        _gameData->_areaData[(int)sup.x]->setDice((int)sup.y);
                        _gameData->_areaData[(int)sup.x]->updatePawn(map);
                }
                ++_dataIdx;
                return;
                
        }else if (ATTACK_RES_NONE == res){
                printf("---should not come here---");
                return;
        }
        
        if (_timeCounter % 40 == 0){
                int from = _hisFrom[_dataIdx];
                _gameData->_areaData[from]->drawAsSelected();
                
        }
        else if (_timeCounter % 40 == 10){
                int to = _hisTo[_dataIdx];
                _gameData->_areaData[to]->drawAsSelected();
                
        }else if (_timeCounter % 40 == 20){
                
                AreaData* area_to       = _gameData->_areaData[to];
                AreaData* area_from     = _gameData->_areaData[from];
                
                if (ATTACK_RES_WIN == res){
                        int new_owner = _gameData->_areaData[from]->getOwner();
                        area_to->changeOwner(new_owner);               
                        area_to->setDice(area_from->getDice() - 1);
                        area_to->updatePawn(map);
                        
                        area_from->initDice();
                        area_from->updatePawn(map);
                        
                }else if (ATTACK_RES_DEFEATED == res){
                        area_from->initDice();
                        area_from->updatePawn(map);
                        
                }
                
        }else if (_timeCounter % 40 == 30){
                int from = _hisFrom[_dataIdx];
                int to = _hisTo[_dataIdx];
                _gameData->_areaData[from]->drawAsUnselected();
                _gameData->_areaData[to]->drawAsUnselected();
                _dataIdx ++;
        }
        
        ++_timeCounter;
}
