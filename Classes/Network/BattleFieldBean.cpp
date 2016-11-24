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

BattleFieldBean::BattleFieldBean(picojson::object& obj){ 

        _battleId       = obj["id"].get<std::string>();
        _battleCreator  = obj["owner"].get<std::string>();
        _playerNum      = (int)obj["playerNum"].get<double>();
        _status         = (int)obj["status"].get<double>();
}
