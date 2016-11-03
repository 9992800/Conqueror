//
//  GameAI.cpp
//  DiceWars
//
//  Created by Wansheng Li on 2016/10/23.
//
//

#include "GameAI.hpp" 
#include "DiceGame.hpp"

static GameAI* s_SharedAI;
GameAI* GameAI::getInstance(){
        if (!s_SharedAI)
        {
                s_SharedAI = new (std::nothrow) GameAI();
                CCASSERT(s_SharedAI, "FATAL: Not enough memory");
                s_SharedAI->init();
        }
        
        return s_SharedAI;
}

GameAI::GameAI(){
        _list_from = std::vector<int>(AI_ATTACK_THINKING_SIZE);
        _list_to   = std::vector<int>(AI_ATTACK_THINKING_SIZE);
}

bool GameAI::init(){
        return true;
}

int GameAI::com_thinking(){
        
        SET_SIZE_TOZERO2(_list_to, _list_from, AI_ATTACK_THINKING_SIZE);
        
        DiceGame* game = DiceGame::getInstance();
        
        for (int i = 0; i < MAX_PLAYER; i++){
                game->_player[i]->resetValue();
        }
        
        int total_dice = 0;
        for (int i = 1; i < AREA_MAX; i++){
                AreaData* area = game->_areaData[i];
                if (area->isEmpty()){
                        continue;
                }
                int owner = area->getOwner();
                GamePlayer* player = game->_player[owner];
                player->increaseAreaC();
                player->addDiceC(area->getDice());
                total_dice += area->getDice();
        }
        
        for (int i = 0; i < MAX_PLAYER - 1; i++) {
                GamePlayer* player_i = game->_player[i];
                for (int j = i + 1; j < MAX_PLAYER; j++){
                        GamePlayer* player_j = game->_player[j];
                        if (player_i->getDiceC() < player_j->getDiceC()){
                                int tmp = player_i->getDiceJun();
                                player_i->setDiceJun(player_j->getDiceJun());
                                player_j->setDiceJun(tmp);
                        }
                }
        }
        
        int target_uid = -1;
        for (int i = 0; i < MAX_PLAYER; i++){
                GamePlayer* player = game->_player[i];
                if (player->getDiceC() > total_dice *2 /5){
                        target_uid = i;
                }
        }
        
        int target_cont = 0;
        int current_player_id = game->_jun[game->_ban];
        for (int i = 1; i < AREA_MAX; i++){
                
                AreaData* area_i = game->_areaData[i];
                if (area_i->isEmpty()
                    || current_player_id != area_i->getOwner()
                    ||area_i->getDice() <= 1){
                        continue;
                }
                
                for (int j = 1; j < AREA_MAX; j++){
                        
                        AreaData* area_j = game->_areaData[j];
                        if (area_j->isEmpty()
                            || area_j->getOwner() == current_player_id
                            || !area_i->isJoinedWithArea(j)){
                                continue;
                        }
                        
                        if (target_uid >= 0
                            && !(area_i->getOwner() != target_uid && area_j->getOwner() != target_uid)
                            && area_j->getDice() <= area_i->getDice()){
                                
                                if (area_j->getDice() == area_i->getDice()){
                                        int owner_j = area_j->getOwner();
                                        bool attack = false;
                                        if (game->_player[current_player_id]->getDiceJun() == 0){
                                                attack = true;
                                        }
                                        if (game->_player[owner_j]->getDiceJun() == 0){
                                                attack = true;
                                        }
                                        if (random(0, 9) > 1){
                                                attack = true;
                                        }
                                        if (attack){
                                                _list_from[target_cont] = i;
                                                _list_to[target_cont] = j;
                                                ++target_cont;
                                        }
                                }else{
                                        _list_from[target_cont] = i;
                                        _list_to[target_cont] = j;
                                        ++target_cont;
                                }
                                
                        }else if (area_j->getDice() <= area_i->getDice()){
                                if (area_j->getDice() == area_i->getDice()){
                                        int owner_j = area_j->getOwner();
                                        bool attack = false;
                                        
                                        if (game->_player[current_player_id]->getDiceJun() == 0){
                                                attack = true;
                                        }
                                        if (game->_player[owner_j]->getDiceJun() == 0){
                                                attack = true;
                                        }
                                        if (random(0, 9) > 1){
                                                attack = true;
                                        }
                                        if (attack){
                                                _list_from[target_cont] = i;
                                                _list_to[target_cont] = j;
                                                ++target_cont;
                                        }
                                }else{
                                        _list_from[target_cont] = i;
                                        _list_to[target_cont] = j;
                                        ++target_cont;
                                }
                        }
                }
        }
        
        if (0 == target_cont){
                return 0;
        }
        
        int random_target = random(0, target_cont - 1);
        game->_area_from = _list_from[random_target];
        game->_area_to   = _list_to[random_target];
        
        return target_cont;
}

