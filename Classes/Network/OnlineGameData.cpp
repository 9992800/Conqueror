//
//  OnlineGameData.cpp
//  Conqueror
//
//  Created by wsli on 16/11/21.
//
//
#include "JoinData.hpp"
#include "AppMacros.hpp"
#include "OnlineGameData.hpp"
OnlineGameData::OnlineGameData():_playerNum(3), _refereMap(nullptr){
        _mapIntData = std::vector<int>(CEL_MAX, 0);
}

OnlineGameData::~OnlineGameData(){
}
bool OnlineGameData::init(){
        
        std::vector<JoinData*> _join = std::vector<JoinData*>(CEL_MAX, nullptr);;
        for (int i = 0; i < CEL_MAX; i++){
                JoinData* join_data = new JoinData();
                join_data->initdir(i);
                _join[i] = join_data;
        }
        
        int _num[CEL_MAX] = {0};
        SET_SIZE_TOIDX(_num, CEL_MAX);
        for (int i = 0; i <  CEL_MAX; i++){
                int radom = random(0, CEL_MAX - 1);
                
                int tmp = _num[i];
                _num[i] = _num[radom];
                _num[radom] = tmp;
        }
        
        int _cel[CEL_MAX]   = {0};
        int _rcel[CEL_MAX]  = {0};
        
        int r = random(0, CEL_MAX - 1);
        _rcel[r] = 1;
        for (int i = 1; i < AREA_MAX; i++){
                int valaible_cel  = 9999;
                int selected_cell;
                for (int j = 0; j < CEL_MAX; j++) {
                        if (_cel[j] <= 0
                            && _num[j] <= valaible_cel
                            &&_rcel[j] != 0){
                                valaible_cel = _num[j];
                                selected_cell = j;
                        }
                }
                
                if (9999 == valaible_cel){
                        break;
                }
                
                
                int next_f[CEL_MAX];
                SET_SIZE_TOZERO(next_f, CEL_MAX);
                int cell_num_in_area = 0;
                int cell_in_area = selected_cell;
                do{
                        _cel[cell_in_area] = i;
                        ++cell_num_in_area;
                        
                        for (int k = 0; k < DIR_INAREA; k++){
                                int joined_cell = _join[cell_in_area]->getJoinDir(k);
                                if (joined_cell >= 0){
                                        next_f[joined_cell] = 1;
                                }
                        }
                        
                        int cell_value = 9999;
                        for (int k = 0; k < CEL_MAX; k++){
                                if (next_f[k] != 0
                                    && _cel[k] <= 0
                                    && _num[k] <= cell_value){
                                        cell_value = _num[k];
                                        cell_in_area = k;
                                }
                        }
                        if (9999 == cell_value){
                                break;
                        }
                }while(cell_num_in_area < 8);
                
                for (int j = 0; j < CEL_MAX; j++){
                        
                        if (next_f[j] != 0 && _cel[j] <= 0){
                                
                                _cel[j] = i;
                                ++cell_num_in_area;
                                
                                for (int k = 0; k < DIR_INAREA; k++){
                                        int joined_cell = _join[j]->getJoinDir(k);
                                        if (joined_cell >= 0){
                                                _rcel[joined_cell] = 1;
                                        }
                                }
                        }
                }
                
                if (0 ==  cell_num_in_area){
                        break;
                }
        }
        
        for (int i = 0; i < CEL_MAX; i++){
                if (_cel[i] > 0){
                        continue;
                }
                
                int areaIdx = 0;
                bool areaNotUsed = false;
                for (int j = 0; j < DIR_INAREA; j++) {
                        int joined_cell = _join[i]->getJoinDir(j);
                        if (joined_cell < 0){
                                continue;
                        }
                        
                        if (_cel[joined_cell] == 0){
                                areaNotUsed = true;
                        }else{
                                areaIdx = _cel[joined_cell];
                        }
                }
                
                if (!areaNotUsed){
                        _cel[i] = areaIdx;
                }
        }
        
        int _area_size[AREA_MAX] = {0};
        for (int i = 0; i < CEL_MAX; i++){
                if (_cel[i] > 0)
                        _area_size[_cel[i]]++;
        }
        
        for (int i = 1; i < AREA_MAX; i++){
                if (_area_size[i] <= 5){
                        _area_size[i] = 0;
                }
        }
        
        for (int i = 0; i < CEL_MAX; i++){
                if (i % AREA_MAX == 0){
                        printf("\r\n");
                }
                printf("%d,",_cel[i]);
        }
        
        _battleData.SetObject();
        rapidjson::Document::AllocatorType& allocator = _battleData.GetAllocator();
        rapidjson::Value array(rapidjson::kArrayType);
        for (int i = 0; i < CEL_MAX; i++){
                if (_area_size[_cel[i]] == 0){
                        _cel[i] = 0;
                }
                array.PushBack(_cel[i], allocator);
        }
        _battleData.AddMember("_cells", array, allocator);
        
        return true;
}


std::string OnlineGameData::getMapData(){
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        _battleData.Accept(writer);

        return  buffer.GetString();
}


void OnlineGameData::reshDataByMapInfo(TMXTiledMap* map){
        this->_refereMap = map;
}
