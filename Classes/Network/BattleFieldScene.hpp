//
//  BattleFieldScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//

#ifndef BattleFieldScene_hpp
#define BattleFieldScene_hpp

#include "cocos2d.h"
#include "OnlineGameData.hpp"
#include "AppMacros.hpp"

USING_NS_CC;

class BattleField: public Layer{
        
public:
        static Scene* createScene(OnlineGameData*);
        virtual bool init() override;
        CREATE_FUNC2(BattleField, OnlineGameData*, data);
        BattleField(OnlineGameData* data):_onlineMapData(data){
                
        }
        virtual ~BattleField();
        
private:
        float           _lowestPostion_y;
        OnlineGameData* _onlineMapData;
};

#endif /* BattleFieldScene_hpp */
