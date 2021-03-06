//
//  GameData.cpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/11/3.
//
//

#include "GameData.hpp"
#include "APPConstants.hpp"
#include "ScreenCoordinate.hpp"
#include "editor-support/cocostudio/CocoStudio.h"

using namespace cocos2d::ui;
using namespace cocostudio;
using namespace cocostudio::timeline;

GameData::GameData(int n):_curPlayerNum(n),_userId(0),
_gameStatus(GAME_STATUS_INIT),_ban(0),_areaFrom(AREA_UNSELECTED),
_areaTo(AREA_UNSELECTED){
        
        _join           = std::vector<JoinData*>(CEL_MAX, nullptr);
        _areaData       = std::vector<AreaData*>(AREA_MAX, nullptr);
        _player         = std::vector<GamePlayer*>(MAX_PLAYER, nullptr);
        _jun            = std::vector<int>(MAX_PLAYER, 0);
        _cel            = std::vector<int>(CEL_MAX, 0);
        _rcel           = std::vector<int>(CEL_MAX, 0);
        _num            = std::vector<int>(CEL_MAX, 0);
        _chk            = std::vector<int>(AREA_MAX, 0);
        _mapData        = std::vector<int>(CEL_MAX, 0);
        
        SET_SIZE_TOIDX(this->_num, CEL_MAX);
}

GameData::~GameData(){
        for (AreaData* p : _areaData){
                p->release();
        }
        for (GamePlayer* p : _player){
                p->release();
        }
        for (JoinData* p : _join){
                p->release();
        }
        
        _join.clear();
        _areaData.clear();
        _player.clear();
        _jun.clear();
        _cel.clear();
        _rcel.clear();
        _num.clear();
        _chk.clear();
        _mapData.clear();
}

GameData* GameData::createWithData(GameData* data){
        
        GameData *pRet = new(std::nothrow) GameData(data->_curPlayerNum);
        if (pRet && pRet->init(data))
        {
                pRet->autorelease();
                return pRet;
        }
        else
        {
                delete pRet;
                pRet = nullptr;
                return nullptr;
        }
}

bool GameData::init(){
        for (int i = 0; i < CEL_MAX; i++){
                JoinData* join_data = JoinData::create();
                join_data->retain();
                join_data->initdir(i);
                _join[i] = join_data;
        }
        
        SET_SIZE_TOIDX(_jun, MAX_PLAYER);
        for (int i = 0; i < _curPlayerNum; i++){
                int ramdom_p = random(0, _curPlayerNum - 1);
                int tmp = this->_jun[i];
                this->_jun[i] = this->_jun[ramdom_p];
                this->_jun[ramdom_p] = tmp;
        }
        
        for (int i = 0; i < MAX_PLAYER; i++){
                this->_player[i] = GamePlayer::create(i);
                this->_player[i]->retain();
        }
        return true;
}

bool GameData::init(GameData* data){
        this->_userId           = data->_userId;
        this->_curPlayerNum     = data->_curPlayerNum;
        this->_gameStatus       = data->_gameStatus;
        this->_ban              = data->_ban;
        this->_areaFrom         = data->_areaFrom;
        this->_areaTo           = data->_areaTo;
        
        
        this->_jun              = std::vector<int>(data->_jun);
        this->_cel              = std::vector<int>(data->_cel);
        this->_rcel             = std::vector<int>(data->_rcel);
        this->_num              = std::vector<int>(data->_num);
        this->_chk              = std::vector<int>(data->_chk);
        this->_mapData          = std::vector<int>(data->_mapData);
        
        this->_areaData = std::vector<AreaData*>(AREA_MAX);
        for (int i = 0; i < this->_areaData.size(); i++){
                this->_areaData[i] = AreaData::create(data->_areaData[i], this);
                this->_areaData[i]->retain();
        }
        
        this->_player = std::vector<GamePlayer*>(MAX_PLAYER);
        for (int i = 0; i < this->_player.size(); i++){
                this->_player[i] = GamePlayer::create(data->_player[i]);
                this->_player[i]->retain();
        }
        
        this->_join = std::vector<JoinData*>(CEL_MAX);
        for (int i = 0; i < this->_join.size(); i++){
                this->_join[i] = JoinData::create(data->_join[i]);
                this->_join[i]->retain();
        }
        
        return true;
}

void GameData::reshDataByBackGrnd(Node* backGrd){
        
        this->_referedLayer = backGrd;
        
        for (int i = 0; i < AREA_MAX; i++){
                DrawNode* draw_node = DrawNode::create();
                backGrd->addChild(draw_node, 1);
                AreaData* area = this->_areaData[i];
                area->intDrawObject(draw_node);
        }
        
        for(int i = 1; i < AREA_MAX; i++){
                AreaData* area = this->_areaData[i];
                if (area->getOwner() < 0){
                        continue;
                }
                Sprite* dice = area->createSprite();
                backGrd->addChild(dice, AREA_SPRITE_ZORDER, AREA_TAG_ID_INMAP(i));
        }
        
        
        FileUtils::getInstance()->addSearchPath("common_anim");
        std::string filename = "hailang.csb";
        cocostudio::timeline::ActionTimeline* action =
        CSLoader::createTimeline(filename);
        
        
        for (int i = 0; i < CEL_MAX; i++){
                if (this->_cel[i] != 0 ){
                        continue;
                }
                
                bool is_shore = false;
                JoinData * join_data = _join[i];
                for (int k = 0; k < DIR_INAREA; k++){
                        int cell_idx = join_data->getJoinDir(k);
                        if (cell_idx > 0 && this->_cel[cell_idx] > 0){
                                is_shore = true;
                                break;
                        }
                }
                if (!is_shore){
                        continue;
                }
                
                Vec2 pos = ScreenCoordinate::getInstance()->getCellPos2(i);
                Node* node = CSLoader::createNode(filename);
                auto action_cop = action->clone();
                
                node->setPosition(pos);
                
                node->runAction(action_cop);
                action_cop->gotoFrameAndPlay(0, true);
                
                backGrd->addChild(node);
        }
}


void GameData::reshDataByMapInfo(TMXTiledMap* map){
        
        this->_refereMap = map;
                
        for (int i = 0; i < AREA_MAX; i++){
                DrawNode* draw_node = DrawNode::create();
                map->addChild(draw_node, 1);
                AreaData* area = this->_areaData[i];
                area->intDrawObject(draw_node);
        }
        
        for(int i = 1; i < AREA_MAX; i++){
                AreaData* area = this->_areaData[i];
                if (area->getOwner() < 0){
                        continue;
                }
                Sprite* dice = area->createSprite();
                map->addChild(dice, AREA_SPRITE_ZORDER, AREA_TAG_ID_INMAP(i));
        }
}
 
