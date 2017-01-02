//
//  AchievementEngine.cpp
//  Conqueror
//
//  Created by wsli on 17/1/1.
//
//

#include "AchievementEngine.hpp"
#include <ctime>

#define OPEN_DAILY_REWARD_KEY         "_key_daily_reward_achieve_open"
#define SHARE_DAILY_REWARD_KEY        "_key_daily_reward_achieve_share"

#define REWARDS_FOR_DAILY_OPEN          (1)
#define REWARDS_FOR_DAILY_SHARE         (2)

static AchievementEngine* s_SharedEngine;
AchievementEngine* AchievementEngine::getInstance(){
        if (!s_SharedEngine) {
                s_SharedEngine = new (std::nothrow) AchievementEngine();
                CCASSERT(s_SharedEngine, "FATAL: Not enough memory");
                s_SharedEngine->init();
        }
        
        return s_SharedEngine;
}

bool AchievementEngine::init(){
        return true;
}

int AchievementEngine::dailyOpenReward(){
        auto cache = UserDefault::getInstance();
        time_t t;
        time (&t);
        struct tm * timeinfo  = localtime (&t);
        int today = 100 * (timeinfo->tm_mon+1) + timeinfo->tm_mday;
        int date_last = cache->getIntegerForKey(OPEN_DAILY_REWARD_KEY, 0);
        if (today == date_last){
                return 0;
        }
        
        int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS, USER_DEFAULT_COINS_ONFIRST);
        cur_coins += REWARDS_FOR_DAILY_OPEN;
        cache->setIntegerForKey(USER_CURRENT_COINS, cur_coins);
        cache->setIntegerForKey(OPEN_DAILY_REWARD_KEY, today);
        cache->flush();
        
        return REWARDS_FOR_DAILY_OPEN;
}

int AchievementEngine::dailyShareReward(){
        auto cache = UserDefault::getInstance();
        time_t t;
        time (&t);
        struct tm * timeinfo  = localtime (&t);
        int today = 100 * (timeinfo->tm_mon+1) + timeinfo->tm_mday;
        int date_last = cache->getIntegerForKey(SHARE_DAILY_REWARD_KEY, 0);
        if (today == date_last){
                return 0;
        }
        
        int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS, USER_DEFAULT_COINS_ONFIRST);
        cur_coins += REWARDS_FOR_DAILY_SHARE;
        cache->setIntegerForKey(USER_CURRENT_COINS, cur_coins);
        cache->setIntegerForKey(SHARE_DAILY_REWARD_KEY, today);
        cache->flush();
        
        return REWARDS_FOR_DAILY_SHARE;
}
