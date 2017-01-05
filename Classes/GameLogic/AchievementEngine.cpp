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


void AchievementEngine::coinsAnimShow(Node* parent, Vec2 from, Vec2 dest,
                                      CallFunc* clean_call,  int coins_num, CallFunc* call_back){
        
        auto move_to = MoveTo::create(1.4f, dest);
        auto coins_change = AnimationCache::getInstance()->getAnimation("coins_changes");
        auto coins_rotate = coins_change->clone();
        coins_rotate->setRestoreOriginalFrame(true);
        coins_rotate->setDelayPerUnit(1.f / 60.f);
        coins_rotate->setLoops(4);
        auto to_dest = Spawn::create(move_to, Animate::create(coins_rotate), NULL);
        
        auto clean_call_self = CallFunc::create([parent, coins_num](){
                for (int i = 0 ; i <= coins_num; i++){
                        parent->removeChildByTag(MODAL_DIALOG_NODETAG + i);
                }
        });
        
        auto seq_last = Sequence::create(to_dest->clone(), clean_call_self,
                                         clean_call, call_back, NULL);
        auto seq_last_2 = Sequence::create(to_dest->clone(), clean_call_self,
                                         clean_call, NULL);
        
        for (int i = 0; i < coins_num; i++){
                auto item_s = Sprite::create("level/coins_show.png");;
                Size r_p(random(-100, 100), random(-100, 100));
                item_s->setPosition(from + r_p);
                parent->addChild(item_s, SUPER_LAYER_PRIVILIEGE, MODAL_DIALOG_NODETAG + i);
                
                if (i == (coins_num - 1)){
                    
                    if(nullptr != call_back){
                            item_s->runAction(seq_last->clone());
                    }else{
                            item_s->runAction(seq_last_2->clone());
                    }
                        
                }else{
                        item_s->runAction(to_dest->clone());
                }
        }
        
        std::string num_str = StringUtils::format("+%d", coins_num);
        auto num_tips = Label::createWithSystemFont(num_str, "fonts/arial.ttf", 68);
        num_tips->setTextColor(Color4B::RED);
        num_tips->setPosition(from);
        parent->addChild(num_tips, SUPER_LAYER_PRIVILIEGE, MODAL_DIALOG_NODETAG + coins_num);
        num_tips->runAction(move_to->clone());
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
        
        this->coinsAnimShow(parent, from, dest, clean_call, REWARDS_FOR_DAILY_OPEN, call_back);
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
        
        this->coinsAnimShow(parent, from, dest, clean_call, REWARDS_FOR_DAILY_SHARE, call_bakc);
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
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_2;
                }
                        break;
                case 3:{
                        coins_num = 3;
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_3;
                }
                        break;
                case 4:{
                        coins_num = 4;
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_4;
                }
                        break;
                case 5:{
                        coins_num = 5;
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_5;
                }
                        break;
                case 6:{
                        coins_num = 7;
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_6;
                }
                        break;
                case 7:{
                        coins_num = 9;
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_7;
                }
                        break;
                case 8:{
                        coins_num = 10;
                        achieve_key = ACHIEVE_DATA_KEY_FIRST_WIN_8;
                }
                        break;
                default:
                        break;
        }
        
        int cur_coins = cache->getIntegerForKey(USER_CURRENT_COINS, USER_DEFAULT_COINS_ONFIRST);
        
        first_get = cache->getIntegerForKey(achieve_key.c_str(), REWARDS_STATUS_CLOSED);
        if (REWARDS_STATUS_CLOSED == first_get){
                cache->setIntegerForKey(achieve_key.c_str(), REWARDS_STATUS_OPEN);
                result_obj = GolbalConfig::getInstance()->getSingleAchievement(achieve_key);
                int new_cup_num = cache->getIntegerForKey(ACHIEVE_DATA_KEY_NEW_ACH_NO, 0);
                cache->setIntegerForKey(ACHIEVE_DATA_KEY_NEW_ACH_NO, ++new_cup_num);
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

void AchievementEngine::finishReward(std::string key){
        auto cache = UserDefault::getInstance();
        int status = cache->getIntegerForKey(key.c_str(), REWARDS_STATUS_CLOSED);
        
        if (REWARDS_STATUS_OPEN == status){
                cache->setIntegerForKey(key.c_str(), REWARDS_STATUS_FINISHED);
                int new_ach_no = cache->getIntegerForKey(ACHIEVE_DATA_KEY_NEW_ACH_NO, 0);
                --new_ach_no;
                if (new_ach_no < 0){
                        new_ach_no = 0;
                }
                cache->setIntegerForKey(ACHIEVE_DATA_KEY_NEW_ACH_NO, new_ach_no);
                cache->flush();
        }
}


std::string AchievementEngine::getCharactorImg(std::string key){
        
        if (CHARACTOR_1_LOCK_STATE_KEY == key){
                return "level/ch_player_0.png";
        }else if (CHARACTOR_2_LOCK_STATE_KEY == key){
                return "level/ch_player_1.png";
        }else if (CHARACTOR_3_LOCK_STATE_KEY == key){
                return "level/ch_player_2.png";
        }else if (CHARACTOR_4_LOCK_STATE_KEY == key){
                return "level/ch_player_3.png";
        }else if (CHARACTOR_5_LOCK_STATE_KEY == key){
                return "level/ch_player_4.png";
        }else if (CHARACTOR_6_LOCK_STATE_KEY == key){
                return "level/ch_player_5.png";
        }else if (CHARACTOR_7_LOCK_STATE_KEY == key){
                return "level/ch_player_6.png";
        }else if (CHARACTOR_8_LOCK_STATE_KEY == key){
                return "level/ch_player_7.png";
        }else{
                return "";
        }
}
std::string AchievementEngine::getMapName(std::string key){
        
        if (LEVEL_1_LOCK_STATE_KEY == key){
                return "2 Players Map Opened";
        }else if (LEVEL_2_LOCK_STATE_KEY == key){
                return "3 Players Map Opened";
        }else if (LEVEL_3_LOCK_STATE_KEY == key){
                return "4 Players Map Opened";
        }else if (LEVEL_4_LOCK_STATE_KEY == key){
                return "5 Players Map Opened";
        }else if (LEVEL_5_LOCK_STATE_KEY == key){
                return "6 Players Map Opened";
        }else if (LEVEL_6_LOCK_STATE_KEY == key){
                return "7 Players Map Opened";
        }else if (LEVEL_7_LOCK_STATE_KEY == key){
                return "8 Players Map Opened";
        }else{
                return "";
        }
}
 
