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
#define REWARDS_FOR_FIRST_SHARE         (15)

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

#define COINS_ANIM_SHOW_NUM (10)
void AchievementEngine::coinsAnimShow(Node* parent, Vec2 from, Vec2 dest,
                                      CallFunc* clean_call,  CallFunc* call_back){
        
        auto move_to = MoveTo::create(1.0f, dest);
        auto coins_change = AnimationCache::getInstance()->getAnimation("coins_changes");
        auto coins_rotate = coins_change->clone();
        coins_rotate->setRestoreOriginalFrame(true);
        coins_rotate->setDelayPerUnit(1.f / 60.f);
        coins_rotate->setLoops(4);
        auto to_dest = Spawn::create(move_to, Animate::create(coins_rotate), NULL);
        
        auto clean_call_self = CallFunc::create([parent](){
                for (int i = 0 ; i < COINS_ANIM_SHOW_NUM; i++){
                        parent->removeChildByTag(MODAL_DIALOG_NODETAG + i);
                }
        });
        
        auto seq_last = Sequence::create(to_dest->clone(), clean_call_self,
                                         clean_call, call_back, NULL);
        
        for (int i = 0; i < COINS_ANIM_SHOW_NUM; i++){
                auto item_s = Sprite::create("level/coins_show.png");;
                Size r_p(random(-100, 100), random(-100, 100));
                item_s->setPosition(from + r_p);
                parent->addChild(item_s, SUPER_LAYER_PRIVILIEGE, MODAL_DIALOG_NODETAG + i);
                if (i == (COINS_ANIM_SHOW_NUM - 1)
                    && nullptr != call_back){
                        item_s->runAction(seq_last->clone());
                }else{
                        item_s->runAction(to_dest->clone());
                }
        }
}

int AchievementEngine::dailyOpenReward(cocos2d::Node *parent, cocos2d::Vec2 from, cocos2d::Vec2 dest, CallFunc* call_back){
        auto cache = UserDefault::getInstance();
        time_t t;
        time (&t);
        struct tm * timeinfo  = localtime (&t);
        int today = 100 * (timeinfo->tm_mon+1) + timeinfo->tm_mday;
        int date_last = cache->getIntegerForKey(OPEN_DAILY_REWARD_KEY, 0);
        if (today == date_last){
                return 0;
        }
        
        auto clean_call = CallFunc::create([parent, cache, today](){
                int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS, USER_DEFAULT_COINS_ONFIRST);
                cur_coins += REWARDS_FOR_DAILY_OPEN;
                cache->setIntegerForKey(USER_CURRENT_COINS, cur_coins);
                cache->setIntegerForKey(OPEN_DAILY_REWARD_KEY, today);
                cache->flush();
        });
        
        this->coinsAnimShow(parent, from, dest, clean_call, call_back);
        return REWARDS_FOR_DAILY_OPEN;
}

int AchievementEngine::dailyShareReward(Node* parent, Vec2 from, Vec2 dest,
                                        CallFunc* call_bakc){
        auto cache = UserDefault::getInstance();
        time_t t;
        time (&t);
        struct tm * timeinfo  = localtime (&t);
        int today = 100 * (timeinfo->tm_mon+1) + timeinfo->tm_mday;
        int date_last = cache->getIntegerForKey(SHARE_DAILY_REWARD_KEY, 0);
        if (today == date_last){
                return 0;
        }
        
        auto clean_call = CallFunc::create([parent, cache, today](){
                int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS, USER_DEFAULT_COINS_ONFIRST);
                cur_coins += REWARDS_FOR_DAILY_SHARE;
                cache->setIntegerForKey(SHARE_DAILY_REWARD_KEY, today);
                
                int first_share = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_SHARE, REWARDS_STATUS_CLOSED);
                if (REWARDS_STATUS_CLOSED == first_share){
                        cache->setIntegerForKey(ACHIEVE_DATA_KEY_FIRST_SHARE, REWARDS_STATUS_OPEN);
                        int new_ach_no = cache->getIntegerForKey(ACHIEVE_DATA_KEY_NEW_ACH_NO, 0);
                        cache->setIntegerForKey(ACHIEVE_DATA_KEY_NEW_ACH_NO, ++new_ach_no);
                }
                cache->setIntegerForKey(USER_CURRENT_COINS, cur_coins);
                cache->flush();
        });
        
        this->coinsAnimShow(parent, from, dest, clean_call, call_bakc);
        return REWARDS_FOR_DAILY_SHARE;
}

AchievementData AchievementEngine::winnerRewards(int playerNum){
        int coins_num = 1, first_get = REWARDS_STATUS_CLOSED;
        std::string achieve_key = "";
        auto cache = UserDefault::getInstance();
        AchievementData result_obj;
        switch (playerNum) {
                case 2:{
                        coins_num = 1;
                        first_get = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_2, REWARDS_STATUS_CLOSED);
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_2;
                }
                        break;
                case 3:{
                        coins_num = 3;
                        first_get = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_3, REWARDS_STATUS_CLOSED);
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_3;
                }
                        break;
                case 4:{
                        coins_num = 4;
                        first_get = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_4, REWARDS_STATUS_CLOSED);
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_4;
                }
                        break;
                case 5:{
                        coins_num = 5;
                        first_get = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_5, REWARDS_STATUS_CLOSED);
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_5;
                }
                        break;
                case 6:{
                        coins_num = 7;
                        first_get = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_6, REWARDS_STATUS_CLOSED);
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_6;
                }
                        break;
                case 7:{
                        coins_num = 9;
                        first_get = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_7, REWARDS_STATUS_CLOSED);
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_7;
                }
                        break;
                case 8:{
                        coins_num = 10;
                        first_get = cache->getIntegerForKey(ACHIEVE_DATA_KEY_FIRST_WIN_8, REWARDS_STATUS_CLOSED);
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_8;
                }
                        break;
                default:
                        break;
        }
        
        int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS, USER_DEFAULT_COINS_ONFIRST);
        
        if (REWARDS_STATUS_CLOSED == first_get){
                cache->setIntegerForKey(achieve_key.c_str(), REWARDS_STATUS_OPEN);
                result_obj = GolbalConfig::getInstance()->getSingleAchievement(achieve_key);
        }else{
                cur_coins += coins_num;
                result_obj.emptyInstance(coins_num);
        }
        cache->setIntegerForKey(USER_CURRENT_COINS, cur_coins);
        cache->flush();
        
        return result_obj;
}

int AchievementEngine::winCounter(){
        auto cache = UserDefault::getInstance();
        int continuous_win = cache->getIntegerForKey(ACHIEVE_WIN_3TIME_COUNTER, 0);
        cache->setIntegerForKey(ACHIEVE_WIN_3TIME_COUNTER, ++continuous_win);
        int status = cache->getIntegerForKey(ACHIEVE_DATA_KEY_WIN_3TIMES,
                                             REWARDS_STATUS_CLOSED);
        if (REWARDS_STATUS_CLOSED == status){
                cache->setIntegerForKey(ACHIEVE_DATA_KEY_WIN_3TIMES,
                                        REWARDS_STATUS_OPEN);
                int new_ach_no = cache->getIntegerForKey(ACHIEVE_DATA_KEY_NEW_ACH_NO, 0);
                cache->setIntegerForKey(ACHIEVE_DATA_KEY_NEW_ACH_NO, ++new_ach_no);
        }
        
        cache->flush();
        return continuous_win;
}

void AchievementEngine::resetWinCounter(){
        
        auto cache = UserDefault::getInstance();
        cache->setIntegerForKey(ACHIEVE_WIN_3TIME_COUNTER, 0);
        cache->flush();
}


void AchievementEngine::openReward(std::string key){
        auto cache = UserDefault::getInstance();
        int status = cache->getIntegerForKey(key.c_str(), REWARDS_STATUS_CLOSED);
        
        if (REWARDS_STATUS_CLOSED == status){
                cache->setIntegerForKey(key.c_str(), REWARDS_STATUS_OPEN);
                int new_ach_no = cache->getIntegerForKey(ACHIEVE_DATA_KEY_NEW_ACH_NO, 0);
                cache->setIntegerForKey(ACHIEVE_DATA_KEY_NEW_ACH_NO, ++new_ach_no);
                cache->flush();
        }
}
