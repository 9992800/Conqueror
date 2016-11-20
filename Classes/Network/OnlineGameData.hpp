//
//  OnlineGameData.hpp
//  Conqueror
//
//  Created by wsli on 16/11/21.
//
//

#ifndef OnlineGameData_hpp
#define OnlineGameData_hpp

#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/rapidjson.h"

USING_NS_CC;
class OnlineGameData : public Ref{
public:
        CREATE_FUNC(OnlineGameData);
        bool init();
        OnlineGameData();
        ~OnlineGameData();
        
        std::string getMapData();
private:
        int _playerNum;
        rapidjson::Document _battleData;
};
#endif /* OnlineGameData_hpp */
