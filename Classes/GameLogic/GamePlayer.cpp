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
