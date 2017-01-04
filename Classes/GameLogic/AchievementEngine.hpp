//
//  AchievementEngine.hpp
//  Conqueror
//
//  Created by wsli on 17/1/1.
//
//

#ifndef AchievementEngine_hpp
#define AchievementEngine_hpp

#include "AppMacros.hpp"
#include "cocos2d.h"
USING_NS_CC;

class AchievementEngine: public Ref{
public:
        static AchievementEngine* getInstance();
        bool init();
        
        int dailyOpenReward(Node*, Vec2, Vec2,CallFunc* call_bakc = NULL);
        int dailyShareReward(Node*, Vec2, Vec2,CallFunc* call_bakc = NULL);
        void collectCoinsRewards(Node*, AchievementData, CallFunc* call_bakc = NULL);
        
        AchievementData winnerRewards(int playerNum);
        int winCounter();
        void resetWinCounter();
        void openReward(std::string);
        
        
private:
        void coinsAnimShow(Node*, Vec2, Vec2, CallFunc*,CallFunc* call_bakc = NULL);
};
#endif /* AchievementEngine_hpp */
