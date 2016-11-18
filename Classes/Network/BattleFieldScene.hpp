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
#include "AppMacros.hpp"

USING_NS_CC;

class BattleField: public Layer{
        
public:
        static Scene* createScene(int mapIdx);
        virtual bool init() override;
        CREATE_FUNC2(BattleField, int, idx);
        BattleField(int idx):_mapIdx(idx){};
        virtual ~BattleField();
        
private:
        void initMap();
        
private:
        int _mapIdx;
};

#endif /* BattleFieldScene_hpp */
