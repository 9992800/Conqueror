//
//  GameAI.hpp
//  DiceWars
//
//  Created by Wansheng Li on 2016/10/23.
//
//

#ifndef GameAI_hpp
#define GameAI_hpp

#include "cocos2d.h"
#include "GameConfig.hpp"

USING_NS_CC;

class GameAI: public Ref{
public:
        static GameAI* getInstance();
        int com_thinking();
private:
        GameAI();
        bool init();
        
private:
        std::vector<int> _list_from;
        std::vector<int> _list_to;
};
#endif /* GameAI_hpp */
