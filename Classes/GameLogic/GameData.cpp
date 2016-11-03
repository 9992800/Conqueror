//
//  GameData.cpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/11/3.
//
//

#include "GameData.hpp"

GameData::GameData(int n):_curPlayerNum(n),_userId(0),
_gameStatus(GAME_STATUS_INIT),_ban(0),_areaFrom(AREA_UNSELECTED),
_areaTo(AREA_UNSELECTED){
        
        _join           = std::vector<JoinData*>(CEL_MAX);
        _areaData       = std::vector<AreaData*>(AREA_MAX);
        _player         = std::vector<GamePlayer*>(MAX_PLAYER);
        _jun            = std::vector<int>(MAX_PLAYER);
        _cel            = std::vector<int>(CEL_MAX);
        _rcel           = std::vector<int>(CEL_MAX);
        _num            = std::vector<int>(CEL_MAX);
        _chk            = std::vector<int>(AREA_MAX);
        _mapData        = std::vector<int>(CEL_MAX);
        
        for (int i = 0; i < CEL_MAX; i++){
                JoinData* join_data = new JoinData();
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
                this->_player[i] = new GamePlayer(i);
        }
}

GameData::~GameData(){
        for (AreaData* p : _areaData){
                delete p;
        }
        for (GamePlayer* p : _player){
                delete p;
        }
        for (JoinData* p : _join){
                delete p;
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

GameData GameData::clone(){
        GameData coped_obj(this->_curPlayerNum);
        coped_obj._gameStatus  = this->_gameStatus;
        
        coped_obj._gameStatus  = this->_gameStatus;
        coped_obj._ban         = this->_ban;
        coped_obj._areaFrom    = this->_areaFrom;
        coped_obj._areaTo      = this->_areaTo;
        
        
        coped_obj._jun         = std::vector<int>(this->_jun);
        coped_obj._cel         = std::vector<int>(this->_cel);
        coped_obj._rcel        = std::vector<int>(this->_rcel);
        coped_obj._num         = std::vector<int>(this->_num);
        coped_obj._chk         = std::vector<int>(this->_chk);
        coped_obj._mapData     = std::vector<int>(this->_mapData);
        
        coped_obj._areaData = std::vector<AreaData*>(AREA_MAX);
        for (int i = 0; i < this->_areaData.size(); i++){
                coped_obj._areaData[i] = new AreaData(this->_areaData[i]);
        }
        
        coped_obj._player = std::vector<GamePlayer*>(MAX_PLAYER);
        for (int i = 0; i < this->_player.size(); i++){
                coped_obj._player[i] = new GamePlayer(this->_player[i]);
        }
        
        coped_obj._join = std::vector<JoinData*>(CEL_MAX);
        for (int i = 0; i < this->_join.size(); i++){
                coped_obj._join[i] = new JoinData(this->_join[i]);
        }
        
        return coped_obj;
}

void GameData::reshDataByMapInfo(TMXTiledMap* map){
                
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
