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
#include "FightResultData.hpp"
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
        FightResultData* startPlayerAttack(int cell_sel);
        FightResultData* startRobootAttack();
        std::map<int, int> cleanUpBattleField(FightResultData* );
        void clearManulAction();
        void starSupplyDice(CallFunc*);
        
        inline int getUserTC(){return _data->_player[_data->_userId]->getAreaTc();}
        inline void next_player(){
                if (++_data->_ban >= _data->_curPlayerNum){
                        _data->_ban = 0;
                }
        }
        
public:
        void initHistoryRecord();
        void finishHistoryRecord();
        inline AreaData* getArea(int area_id){
                return _data->_areaData[area_id];
        }
private:
        void makeNewMapData();
        int percolate(int pt, int cmax, int an);
        void setAreaLine(int cell, int dir);
        int set_area_tc(int pid);
        void occupyArea(int newOwner, int area);
        FightResultData* startBattle();
        
        
        
private:
        GameData*               _data;
        GameData*               _clonedGameData;
        std::vector<int>        _historyFrom;
        std::vector<int>        _historyTo;
        std::vector<int>        _historyRes;
        std::vector<Vec2>       _historySup;
};

#endif /* DiceGame_hpp */
