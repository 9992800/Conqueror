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
        static void coinsAnimShow(Node*, Vec2, Vec2, CallFunc* call_bakc = NULL);
        bool init();
        
        int dailyOpenReward();
        int dailyShareReward();
        AchievementData winnerRewards(int playerNum);
        int winCounter();
        void resetWinCounter();
        void openReward(std::string);
};
#endif /* AchievementEngine_hpp */
