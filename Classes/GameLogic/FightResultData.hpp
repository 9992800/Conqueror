//
//  FightResultData.hpp
//  Conqueror
//
//  Created by wsli on 16/11/30.
//
//

#ifndef FightResultData_hpp
#define FightResultData_hpp
#include "AppMacros.hpp"
#include "cocos2d.h"
USING_NS_CC;
class DiceGame;
class GameScene;

class FightResultData: public Ref{
        friend class GameScene;
        friend class DiceGame;
public:
        CREATE_FUNC(FightResultData);
        virtual bool init(){
                _result = ATTACK_RES_NONE;
                _from = std::vector<int>(MAX_DICE_PER_AREA, 0);
                _to = std::vector<int>(MAX_DICE_PER_AREA, 0);
                return true;
        }
private:
        int     _fromArea;
        int     _toArea;
        int     _fromSum;
        int     _toSum;
        int     _result;
        std::vector<int> _from;
        std::vector<int> _to;
};
#endif /* FightResultData_hpp */
