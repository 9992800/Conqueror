//
//  BattleFieldBean.cpp
//  Conqueror
//
//  Created by wsli on 16/11/21.
//
//
#include "BattleFieldBean.hpp"
bool BattleFieldBean::init(){
        return true;
}

BattleFieldBean::BattleFieldBean(rapidjson::Value::ConstValueIterator vptr){
        _cells = std::vector<int>(CEL_MAX, 0);
        _battleId       = ((*vptr)["id"].GetString());
        _battleCreator  = ((*vptr)["owner"].GetString());
        _playerNum      = ((*vptr)["playerNum"].GetInt());
        _status         = ((*vptr)["status"].GetInt());
        
        int i = 0;
        for (rapidjson::Value::ConstValueIterator it = (*vptr)["dataInfo"].Begin();
             it != (*vptr)["dataInfo"].End(); ++it){
                _cells[i++] = (it->GetInt());
        }
}
