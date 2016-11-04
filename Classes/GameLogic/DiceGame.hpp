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
        
public:
        GameData* initGameData(int num); 
protected:
        bool init();
        
private:
        void makeNewMapData();
        int percolate(int pt, int cmax, int an);
        void setAreaLine(int cell, int dir);
        int set_area_tc(int pid);
        
private:
        GameData*        _data;
};

#endif /* DiceGame_hpp */
