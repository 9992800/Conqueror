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
        _battleId       = ((*vptr)["id"].GetString());
        _battleCreator  = ((*vptr)["owner"].GetString());
        _playerNum      = ((*vptr)["playerNum"].GetInt());
        _status         = ((*vptr)["status"].GetInt());
}
