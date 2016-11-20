//
//  OnlineGameData.cpp
//  Conqueror
//
//  Created by wsli on 16/11/21.
//
//
#include "AppMacros.hpp"
#include "OnlineGameData.hpp"
OnlineGameData::OnlineGameData():_playerNum(3){
}

OnlineGameData::~OnlineGameData(){
}
bool OnlineGameData::init(){
        int _num[CEL_MAX] = {0};
        for (int i = 0; i <  CEL_MAX; i++){
                int radom = random(0, CEL_MAX - 1);
                
                int tmp = _num[i];
                _num[i] = _num[radom];
                _num[radom] = tmp;
        }

        return true;
}


std::string OnlineGameData::getMapData(){
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        _battleData.Accept(writer);

        return  _battleData.GetString();
}


