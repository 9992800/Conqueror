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
        
        AchievementData winnerRewards(int playerNum);
        int winCounter();
        void resetWinCounter();
        void openReward(std::string);
        void finishReward(std::string);
        
        void coinsAnimShow(Node*, Vec2, Vec2, CallFunc*,
                           int coin_num = COINS_ANIM_SHOW_NUM, CallFunc* call_bakc = NULL);
        std::string getCharactorImg(std::string key);
        std::string getMapName(std::string key);
        inline std::string getLockName(int indx, int type){
                if (0 == type){
                        return StringUtils::format("_key_charactor_%d_lock_state", indx);
                }else if (1 == type){
                        return StringUtils::format("_key_level_%d_lock_state", indx);
                }
                return "";
        }
        
        bool getCharLockStatus(int);
        int getCharUnlockPrice(int);
};
#endif /* AchievementEngine_hpp */
