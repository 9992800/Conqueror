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
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/rapidjson.h"
#include "json/document.h"

template<typename T>
std::vector<T> parseData(const char* key)
{
        Data data = UserDefault::getInstance()->getDataForKey(key);
        T* buffer = (T*) data.getBytes();
        ssize_t length = data.getSize() / sizeof(T);
        
        std::vector<T> result;
        for (int i = 0; i < length; i++) {
                result.push_back(buffer[i]);
        }
        
        return result;
}

std::vector<std::string> parseData(const char* key){
        Data data = UserDefault::getInstance()->getDataForKey(key);
        std::string::value_type* buffer = (std::string::value_type*) data.getBytes();
        ssize_t length = data.getSize() / sizeof(std::string::value_type);
        
        std::vector<std::string> result;
        std::istringstream f(buffer, length);
        std::string str;
        while (getline(f, str, '\n')){
                result.push_back(str);
        }
        return result;
}

Scene* ReplayLast::createScene(){
        auto scene = Scene::create();
        auto layer = ReplayLast::create();
        
        scene->addChild(layer);
        return scene;
}

bool ReplayLast::init(){
        int player_num = UserDefault::getInstance()->getIntegerForKey(GAME_HISTORY_PLAYER_NUM);
        
        if (0 == player_num){
                return false;
        }
        
        _gameData = GameData::create(player_num);
        _gameData->retain();
        
        std::vector<int>  map_data = parseData<int>(GAME_HISTORY_MAP_KEY);
        _gameData->_mapData = map_data;
        
        std::vector<int>  cell_data = parseData<int>(GAME_HISTORY_CELL_INFO);
        _gameData->_cel = cell_data;
        
        std::vector<std::string> area_data = parseData(GAME_HISTORY_AREA_INFO);
        
        for (int i = 0; i < AREA_MAX; i++){
                std::string area_str = area_data[i];
                
                rapidjson::Document area_d;
                area_d.Parse<0>(area_str.c_str());
                if (area_d.HasParseError()) {
                        CCLOG("GetParseError %u\n",area_d.GetParseError());
                        return false;
                }
                
                AreaData* area = new AreaData(i);
                
                const rapidjson::Value& basic = area_d["basic"];
                area->_arm      = basic["_arm"].GetInt();
                area->_size     = basic["_size"].GetInt();
                area->_cpos     = basic["_cpos"].GetInt();
                area->_dice     = basic["_dice"].GetInt();
                area->_left     = basic["_left"].GetInt();
                area->_right    = basic["_right"].GetInt();
                area->_top      = basic["_top"].GetInt();
                area->_bottom   = basic["_bottom"].GetInt();
                area->_cx       = basic["_cx"].GetInt();
                area->_cy       = basic["_cy"].GetInt();
                area->_len_min  = basic["_len_min"].GetInt();
                area->_areaId   = basic["_areaId"].GetInt();
                
                
                const rapidjson::Value& _line_cel = area_d["_line_cel"];
                for (int j = 0; j < _line_cel.Size(); j++){
                        int cel = _line_cel[(rapidjson::SizeType)j].GetInt();
                        area->_line_cel[j] = cel;
                }
                const rapidjson::Value& _line_dir = area_d["_line_dir"];
                for (int j = 0; j < _line_dir.Size(); j++){
                        int dir = _line_dir[j].GetInt();
                        area->_line_dir[j] = dir;
                }
                
                const rapidjson::Value& _cell_idxs = area_d["_cell_idxs"];
                for (int j = 0; j < _cell_idxs.Size(); j++){
                        int cel = _cell_idxs[j].GetInt();
                        area->_cell_idxs.insert(cel);
                }
                
                _gameData->_areaData[i] = area;
        }
        
        
        
        auto map = MapCreator::instance()->createMap(_gameData);
        Size map_size = map->getContentSize();
        ScreenCoordinate::getInstance()->configScreen(map_size);
        
        _gameData->reshDataByMapInfo(map);
        
        this->addChild(map, 0, 1);
        
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
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
        this->addChild(menu);
        
        
        return true;
}

ReplayLast::~ReplayLast(){
        _gameData->release();
}


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
        scheduleUpdate();
}

void ReplayLast::update(float delta){
        
}

void ReplayLast::onExit(){
        Layer::onExit();
        unscheduleUpdate();
}
