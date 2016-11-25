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
        
        if (obj["players"].evaluate_as_boolean()){
                picojson::array array = obj["players"].get<picojson::array>();
                for (picojson::array::iterator it = array.begin(); it != array.end(); it++){
                        _curPlayerFbIds.insert(it->get<std::string>());
                }
        }
}
