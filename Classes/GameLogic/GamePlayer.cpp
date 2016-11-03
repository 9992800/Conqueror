//
//  GamePlayer.cpp
//  DiceWars
//
//  Created by Wansheng Li on 16/10/12.
//
//

#include "GamePlayer.hpp"
GamePlayer::GamePlayer(int id)
:_player_id(id)
,_area_c(0)
,_area_tc(0)
,_dice_c(0)
,_dice_jun(id)
,_stock(0){
        _gid_inmap = id + 1;
}

GamePlayer::~GamePlayer(){
}


GamePlayer::GamePlayer(GamePlayer* obj){
        this->_player_id        = obj->_player_id;
        this->_area_c           = obj->_area_c;
        this->_area_tc          = obj->_area_tc;
        this->_dice_c           = obj->_dice_c;
        this->_dice_jun         = obj->_dice_jun;
        this->_stock            = obj->_stock;
        this->_gid_inmap        = obj->_gid_inmap;
}
