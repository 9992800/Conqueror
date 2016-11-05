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
        void clearManulAction();
        void starSupplyDice(CallFunc*);
        
        inline int getUserTC(){return _data->_player[_data->_userId]->getAreaTc();}
        inline void next_player(){
                if (++_data->_ban >= _data->_curPlayerNum){
                        _data->_ban = 0;
                }
        }
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
