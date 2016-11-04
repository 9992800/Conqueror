//
//  DiceGame.cpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//

#include "DiceGame.hpp"

#pragma mark - game init
bool DiceGame::init(){
        return true;
}

GameData* DiceGame::initGameData(int num){
        _data =  GameData::create(num);
        
        this->makeNewMapData();
        
        for (int i = 0; i < MAX_PLAYER; i++){
                this->set_area_tc(i);
        }
        
        for (int i = 0; i < CEL_MAX; i++){
                int area_id = _data->_cel[i];
                AreaData* area = _data->_areaData[area_id];
                int player_uid = area->getOwner();
                GamePlayer* player = _data->_player[player_uid];
                if (area_id > 0)
                        _data->_mapData[i] = player->getGid();
                else
                        _data->_mapData[i] = 0;
        }
        _data->retain();
        
        _clonedGameData = GameData::createWithData(_data);
        _clonedGameData->retain();

        return _data;
}


GameData* DiceGame::resetInitData(){
        _data->release();
        _data = GameData::createWithData(_clonedGameData);
        _data->retain();
        return _data;
}

void DiceGame::makeNewMapData(){
        
        SET_SIZE_TOIDX(_data->_num, CEL_MAX);
        for (int i = 0; i <  CEL_MAX; i++){
                int radom = random(0, CEL_MAX - 1);
                
                int tmp = _data->_num[i];
                _data->_num[i] = _data->_num[radom];
                _data->_num[radom] = tmp;
        }
        
        SET_SIZE_TOZERO2(_data->_cel, _data->_rcel, CEL_MAX);
        int radom = random(0, CEL_MAX -1);
        _data->_rcel[radom] = 1;
        /*create original area data*/
        for (int i = 1; i < AREA_MAX; ++i){
                int valaible_cel = 9999;
                int selected_cell;
                for (int j = 0; j < CEL_MAX; j++){
                        
                        if (_data->_cel[j] <= 0
                            && _data->_num[j] <= valaible_cel
                            && _data->_rcel[j] != 0){
                                
                                valaible_cel = _data->_num[j];
                                selected_cell = j;
                        }
                }
                
                if (9999 == valaible_cel){
                        break;
                }
                
                int next_cel = this->percolate(selected_cell, 8, i);
                if (0 ==  next_cel){
                        break;
                }
        };
        /*make all cells around created area been in used*/
        for (int i = 0; i < CEL_MAX; i++){
                if (_data->_cel[i] > 0){
                        continue;
                }
                
                int areaIdx = 0;
                bool areaNotUsed = false;
                for (int j = 0; j < DIR_INAREA; j++) {
                        int joined_cell = _data->_join[i]->getJoinDir(j);
                        if (joined_cell < 0){
                                continue;
                        }
                        
                        if (_data->_cel[joined_cell] == 0){
                                areaNotUsed = true;
                        }else{
                                areaIdx = _data->_cel[joined_cell];
                        }
                }
                
                if (!areaNotUsed){
                        _data->_cel[i] = areaIdx;
                }
        }
        
        for (int i = 0 ; i < AREA_MAX; i++){
                
                if (nullptr != _data->_areaData[i]){
                        delete _data->_areaData[i];
                }
                _data->_areaData[i] = new AreaData(i);
        }
        
        for (int i = 0; i < CEL_MAX; i++){
                int area_id = _data->_cel[i];
                if (area_id > 0){
                        _data->_areaData[area_id]->increaseSize(i);
                }
        }
        
        for (int i = 1; i < AREA_MAX; i++){
                _data->_areaData[i]->removeAreaTooSmall(i);
        }
        
        for (int i = 0; i < CEL_MAX; i++){
                int areaId = _data->_cel[i];
                if (_data->_areaData[areaId]->isEmpty() ){
                        _data->_cel[i] = 0;
                }
        }
        
        for (int j = 0; j < CEL_MAX; j++){
                int area_id = _data->_cel[j];
                if (j % AREA_MAX == 0){
                        printf("]\r\n[");
                }
                printf(" %d ", area_id);
        }
        
        int cell_idx = 0;
        for (int i = 0; i < YMAX; i++){
                for (int j = 0; j < XMAX; j++){
                        int area_id = _data->_cel[cell_idx];
                        if (area_id > 0){
                                AreaData* area = _data->_areaData[area_id];
                                area->initBound(i, j);
                        }
                        
                        ++cell_idx;
                }
        }
        
        
        for (int i = 1; i < AREA_MAX; i++){
                AreaData* area = _data->_areaData[i];
                area->initCenter();
        }
        
        
        cell_idx = 0;
        for (int i = 0; i < YMAX; i++){
                
                for (int j = 0; j < XMAX; j++){
                        
                        int area_id = _data->_cel[cell_idx];
                        
                        if (area_id > 0){
                                AreaData* area = _data->_areaData[area_id];
                                area->calcLenAndPos(i, j, cell_idx, _data);
                        }
                        
                        cell_idx++;
                }
        }
        
        
        int tmpAreaId[AREA_MAX] = {0};
        int player_uid = 0;
        
        while(true){
                int available_area = 0;
                for (int i = 1; i < AREA_MAX; i++){
                        AreaData* area = _data->_areaData[i];
                        if (area->isNeedOwner()){
                                tmpAreaId[available_area] = i;
                                ++available_area;
                        }
                        
                }
                
                if (0 == available_area){
                        break;
                }
                
                int random_area = tmpAreaId[random<int>(0, available_area - 1)];
                _data->_areaData[random_area]->setOwner(player_uid);
                
                ++player_uid;
                if (player_uid >= _data->_curPlayerNum){
                        player_uid = 0;
                }
        }
        
        SET_SIZE_TOZERO(_data->_chk, AREA_MAX);
        
        for (int i = 0; i < CEL_MAX; i++){
                int area_id = _data->_cel[i];
                
                if (area_id == 0 || _data->_chk[area_id]){
                        continue;
                }
                
                for (int j = 0; j < DIR_INAREA; j++){
                        if (_data->_chk[area_id]){
                                break;
                        }
                        
                        int joined_cell = _data->_join[i]->getJoinDir(j);
                        if (joined_cell >= 0
                            && _data->_cel[joined_cell] != area_id){
                                
                                this->setAreaLine(i, j);
                                _data->_chk[area_id] = 1;
                        }
                }
        }
        
        int total_dice = 0;
        for (int i = 1; i < AREA_MAX; i++){
                if (_data->_areaData[i]->initDice()){
                        total_dice++;
                }
        }
        
        total_dice = total_dice * (PUT_DICE - 1);
        player_uid = 0;
        for (int i = 0; i < total_dice; i++){
                
                int tempArea[AREA_MAX] = {0};
                int available_area = 0;
                
                
                for (int j = 1; j < AREA_MAX; j++){
                        if (_data->_areaData[j]->needDice(player_uid)){
                                tempArea[available_area] = j;
                                ++available_area;
                        }
                }
                
                if (0 == available_area){
                        break;
                }
                
                int random_area = tempArea[random(0, available_area - 1)];
                _data->_areaData[random_area]->increaseDice();
                
                ++player_uid;
                if (player_uid >= _data->_curPlayerNum){
                        player_uid = 0;
                }
        }
}


int DiceGame::percolate(int pt, int cmax, int an){
        if (cmax < 3){
                cmax = 3;
        }
        
        int cell_in_area = pt;
        int next_f[CEL_MAX] = {0};
        
        int cell_num_in_area = 0;
        while (cell_num_in_area < cmax){
                _data->_cel[cell_in_area] = an;
                ++cell_num_in_area;
                
                for (int j = 0; j < DIR_INAREA; j++){
                        int joined_cell = _data->_join[cell_in_area]->getJoinDir(j);
                        if (joined_cell >= 0){
                                next_f[joined_cell] = 1;
                        }
                }
                
                
                int cell_value = 9999;
                for (int j = 0; j < CEL_MAX; j++){
                        if (next_f[j] != 0 &&
                            _data->_cel[j] <= 0 &&
                            _data->_num[j] < cell_value){
                                
                                cell_value = _data->_num[j];
                                cell_in_area = j;
                        }
                }
                
                if (9999 == cell_value){
                        break;
                }
        }
        
        
        for (int i = 0; i < CEL_MAX; i++){
                
                if (next_f[i] != 0 && _data->_cel[i] <= 0){
                        
                        _data->_cel[i] = an;
                        ++cell_num_in_area;
                        
                        for (int j = 0; j < DIR_INAREA; j++){
                                int joined_cell = _data->_join[i]->getJoinDir(j);
                                if (joined_cell >= 0){
                                        _data->_rcel[joined_cell] = 1;
                                }
                        }
                }
        }
        
        return cell_in_area;
}


void DiceGame::setAreaLine(int cell, int dir){
        
        int cur_area = _data->_cel[cell];
        AreaData* area = _data->_areaData[cur_area];
        
        area->initAreaLine(cell, dir, _data);
}


int DiceGame::set_area_tc(int pid){
        
        GamePlayer* player = _data->_player[pid];
        
        SET_SIZE_TOIDX(_data->_chk, AREA_MAX);
        
        bool found = false;
        do {
                found = false;
                
                for (int i = 1; i < AREA_MAX; i++){
                        
                        AreaData* area = _data->_areaData[i];
                        if (area->isEmpty() || area->getOwner() != pid){
                                continue;
                        }
                        
                        for (int j = 0; j < AREA_MAX; j++){
                                
                                AreaData* area_another = _data->_areaData[j];
                                if (area_another->isEmpty()
                                    || area_another->getOwner() != pid
                                    || !area->isJoinedWithArea(j)
                                    || _data->_chk[j] == _data->_chk[i]){
                                        continue;
                                }
                                
                                if (_data->_chk[i] > _data->_chk[j]){
                                        _data->_chk[i] = _data->_chk[j];
                                }else{
                                        _data->_chk[j] = _data->_chk[i];
                                }
                                found = true;
                                break;
                        }
                        
                        if (found){
                                break;
                        }
                }
                
        } while (found);
        
        int tcArr[AREA_MAX] = {0};
        for (int i = 1;  i < AREA_MAX; i++) {
                if (!_data->_areaData[i]->isEmpty()
                    && _data->_areaData[i]->getOwner() == pid){
                        ++tcArr[_data->_chk[i]];
                }
        }
        
        
        int tc = 0;
        for (int i = 0; i < AREA_MAX; i++){
                if (tcArr[i] > tc){
                        tc = tcArr[i];
                }
        }
        
        player->setAreaTc(tc);
        printf("\r\n\r\n---player:(%d) tc=%d---", pid, tc);
        
        return tc;
}

#pragma mark - game logic process

int DiceGame::startBattle(){
        
        AreaData* area_from = _data->_areaData[_data->_areaFrom];
        area_from->drawAsSelected();
        
        AreaData* area_to   = _data->_areaData[_data->_areaTo];
        area_to->drawAsSelected();
        
        int from_sum = 0, to_sum = 0;
        
        area_from->clearFightValue();
        printf("\r\n---from---");
        for (int i = 0; i < area_from->getDice(); i++){
                int random_value = random(1, 6);
                area_from->recordFightValue(random_value);
                from_sum += random_value;
                printf("\t---%d---", random_value);
        }
        
        printf("\r\n---to---");
        area_to->clearFightValue();
        for (int i = 0; i < area_to->getDice(); i++){
                int random_value = random(1, 6);
                area_to->recordFightValue(random_value);
                to_sum += random_value;
                printf("\t---%d---", random_value);
        }
        
        if (from_sum > to_sum){
                return ATTACK_RES_WIN;
        }else{
                return ATTACK_RES_DEFEATED;
        }
}

int DiceGame::startPlayerAttack(int cell_id){
        
        int area_id = _data->_cel[cell_id];
        
        AreaData* area = _data->_areaData[area_id];
        int owner_uid = area->getOwner();
        
        
        if (AREA_UNSELECTED == _data->_areaFrom){
                
                if (area->getDice() <= 1){
                        return ATTACK_RES_NONE;
                }
                
                if (owner_uid == _data->_userId){
                        _data->_areaFrom = area_id;
                        area->drawAsSelected();
                }else{
                        return ATTACK_RES_NONE;
                }
                
        }else{
                if (area_id == _data->_areaFrom){
                        _data->_areaFrom = AREA_UNSELECTED;
                        area->drawAsUnselected();
                }else {
                        
                        if (area->isJoinedWithArea(_data->_areaFrom)
                            && owner_uid != _data->_userId){
                                _data->_areaTo = area_id;
                                area->drawAsSelected();
                                return this->startBattle();
                                
                        }else{
                                return ATTACK_RES_NONE;
                        }
                }
        }
        return ATTACK_RES_NONE;
}


int DiceGame::startRobootAttack(){
        if (_data->_jun[_data->_ban] == _data->_userId){
                return ATTACK_RES_NONE;
        }
        int target = GameAI::getInstance()->com_thinking(_data);
        if (0 == target){
                return ATTACK_RES_GOTSUPPLY;
        }else{
                return this->startBattle();
        }
        
}

std::map<int, int> DiceGame::cleanUpBattleField(int res){
        AreaData* area_from = _data->_areaData[_data->_areaFrom];
        AreaData* area_to   = _data->_areaData[_data->_areaTo];
        
        area_from->drawAsUnselected();
        area_to->drawAsUnselected();
        
        area_from->clearFightValue();
        area_to->clearFightValue();
        
        if (ATTACK_RES_WIN == res){
                
                this->occupyArea(area_from->getOwner(), _data->_areaTo);
                
                area_to->setDice(area_from->getDice() - 1);
                area_to->updatePawn(_data->_refereMap);
        }
        
        area_from->initDice();
        area_from->updatePawn(_data->_refereMap);
        
        std::map<int, int> ok_area = std::map<int, int>();
        for (int i = 0; i < MAX_PLAYER; i++){
                int tc = this->set_area_tc(i);
                ok_area.insert(std::pair<int, int>(i, tc));
        }
        
        _data->_areaTo    = AREA_UNSELECTED;
        _data->_areaFrom  = AREA_UNSELECTED;
        
        return ok_area;
}


void DiceGame::clearManulAction(){
        if (AREA_UNSELECTED == _data->_areaFrom){
                return;
        }
        
        AreaData* area = _data->_areaData[_data->_areaFrom];
        area->drawAsUnselected();
        _data->_areaFrom = AREA_UNSELECTED;
}


void DiceGame::starSupplyDice(CallFunc* callback){
        int player_id = _data->_jun[_data->_ban];
        GamePlayer* player = _data->_player[player_id];
        player->setStock();
        
        
        std::set<int> affected_aread;
        while (player->getStock() > 0){
                
                int list[AREA_MAX] = {0};
                int count = 0;
                for (int i = 0; i < AREA_MAX; i++){
                        AreaData* area = _data->_areaData[i];
                        if (area->isEmpty()
                            ||area->getOwner() != player_id
                            ||area->getDice() >= MAX_DICE_PER_AREA){
                                continue;
                        }
                        
                        list[count++] = i;
                }
                
                if (count == 0){
                        break;
                }
                
                int random_area = random(0, count - 1);
                int selected_area = list[random_area];
                _data->_areaData[selected_area]->increaseDice();
                player->decreaseStock();
                affected_aread.insert(selected_area);
        }
        
        for(std::set<int>::iterator it = affected_aread.begin(); it != affected_aread.end(); ++it){
                AreaData* area = _data->_areaData[*it];
                
                area->updatePawn(_data->_refereMap);
                area->drawSupply(_data->_refereMap);
        }
        callback->execute();
}


void DiceGame::occupyArea(int newOwner, int area){
        
        TMXLayer * layer = _data->_refereMap->getLayer(LAYER_NAME_IN_TILE_MAP);
        GamePlayer* player = _data->_player[newOwner];
        
        for (int j = 0; j < CEL_MAX; j++){
                if (area != _data->_cel[j]){
                        continue;
                }
                
                int col = j / XMAX;
                int row = j % XMAX;
                
                layer->setTileGID(player->getGid(), Vec2(row, col));
        }
        
        _data->_areaData[area]->setOwner(newOwner);
}
