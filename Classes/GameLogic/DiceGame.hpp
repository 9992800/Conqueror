//
//  DiceGame.hpp
//  Conqueror
//
//  Created by wsli on 16/11/3.
//
//

#ifndef DiceGame_hpp
#define DiceGame_hpp

#include "cocos2d.h"
#include "GameData.hpp"


USING_NS_CC;

class DiceGame : public Ref{
public:
        CREATE_FUNC(DiceGame);
        bool init();
        ~DiceGame(){
                _data->release();
                _clonedGameData->release();
        }
public:
        GameData* initGameData(int num);
        GameData* resetInitData();
        int startPlayerAttack(int cell_sel);
        int startRobootAttack();
        std::map<int, int> cleanUpBattleField(int);
private:
        void makeNewMapData();
        int percolate(int pt, int cmax, int an);
        void setAreaLine(int cell, int dir);
        int set_area_tc(int pid);
        void occupyArea(int newOwner, int area);
        
private:
        int startBattle();
        
private:
        GameData*       _data;
        GameData*       _clonedGameData;
};

#endif /* DiceGame_hpp */
