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
USING_NS_CC;

class BattleFieldBean: public Ref{
public:
        CREATE_FUNC2(BattleFieldBean, rapidjson::Value::ConstValueIterator, vptr);
        BattleFieldBean(rapidjson::Value::ConstValueIterator vptr);
        virtual ~BattleFieldBean(){}
        bool init();
private:
        std::vector<int> _cells;
        std::string     _battleId;
        std::string     _battleCreator;
        int             _playerNum;
        int             _status;
};

#endif /* BattleFieldBean_hpp */
