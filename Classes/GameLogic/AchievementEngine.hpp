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
        
        int dailyOpenReward();
        int dailyShareReward();
};
#endif /* AchievementEngine_hpp */
