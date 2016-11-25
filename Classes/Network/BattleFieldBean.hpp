//
//  BattleFieldBean.hpp
//  Conqueror
//
//  Created by wsli on 16/11/21.
//
//

#ifndef BattleFieldBean_hpp
#define BattleFieldBean_hpp

#include "cocos2d.h"
#include "AppMacros.hpp"
#include "picojson.h"

USING_NS_CC;

class BattleFieldBean: public Ref{
public:
        CREATE_FUNC2(BattleFieldBean, picojson::object&, obj);
        BattleFieldBean(picojson::object& obj);
        virtual ~BattleFieldBean(){}
        bool init();
        
        inline std::string getCreator(){
                return _battleCreator;
        }
        
        inline std::set<std::string> getCurrentPlayers(){
                return this->_curPlayerFbIds;
        }
        
private:
        std::string     _battleId;
        std::string     _battleCreator;
        int             _playerNum;
        int             _status;
        std::set<std::string> _curPlayerFbIds;
};

#endif /* BattleFieldBean_hpp */
