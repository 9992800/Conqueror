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
        GameData initGameData(int num);
protected:
        bool init();
};

#endif /* DiceGame_hpp */
