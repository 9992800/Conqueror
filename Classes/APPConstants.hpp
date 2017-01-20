//
//  APPConstants.hpp
//  Conqueror
//
//  Created by wsli on 16/12/13.
//
//

#ifndef APPConstants_hpp
#define APPConstants_hpp

#include "cocos2d.h"
USING_NS_CC;
extern int READY_DISTANCE_POS;
extern std::string ANIM_NAME_FIGHT_RUN[];
extern std::string ANIM_NAME_FIGHT_STAND[];
extern Vec2 invader_pos[];
extern Vec2 keeper_pos[];
extern std::string DICE_PIC_NAME_STR[8][6];
extern std::string CHARACTER_NAME[];
extern std::string CHARACTER_NAME_2[];
extern std::string ANIM_NAME_DEFEATED_SHOW[2][8];
extern Color4F AreaBackGroundColors[];
extern Color4F border_color;
extern Color4F selected_color;


#define ACHIEVE_DATA_KEY_FIRST_WIN_2            "_key_achive_data_first_win_2"
#define ACHIEVE_DATA_KEY_FIRST_WIN_3            "_key_achive_data_first_win_3"
#define ACHIEVE_DATA_KEY_FIRST_WIN_4            "_key_achive_data_first_win_4"
#define ACHIEVE_DATA_KEY_FIRST_WIN_5            "_key_achive_data_first_win_5"
#define ACHIEVE_DATA_KEY_FIRST_WIN_6            "_key_achive_data_first_win_6"
#define ACHIEVE_DATA_KEY_FIRST_WIN_7            "_key_achive_data_first_win_7"
#define ACHIEVE_DATA_KEY_FIRST_WIN_8            "_key_achive_data_first_win_8"

#define ACHIEVE_DATA_KEY_WIN_3TIMES             "_key_achive_data_win_3times" 
#define ACHIEVE_DATA_KEY_FIRST_USE_MERCENARY    "_key_achive_data_first_use_mercenary"
#define ACHIEVE_DATA_KEY_FIRST_BUY_COINS        "_key_achive_data_first_buy_coins"
#define ACHIEVE_DATA_KEY_FIRST_SHARE            "_key_achive_data_first_share_game"
#define ACHIEVE_DATA_KEY_FIRST_CLOSE_ANIM       "_key_achive_data_first_close_anim"
#define ACHIEVE_DATA_KEY_FIRST_RETRY            "_key_achive_data_first_replay_game"
#define ACHIEVE_DATA_KEY_FIRST_CHANGE_COLOR     "_key_achive_data_first_change_color"
#define ACHIEVE_DATA_KEY_FIRST_CHANGE_PLAYER    "_key_achive_data_first_change_PLAYER"
#define ACHIEVE_DATA_KEY_FIRST_ENGAGE           "_key_achive_data_first_engage"


#define ACHIEVE_DATA_KEY_NEW_ACH_NO             "_key_achive_data_new_achievement_no"
#define ACHIEVE_WIN_3TIME_COUNTER               "_key_achieve_win_3time_conter"

enum {
        ACHIEVE_BONUS_TYPE_NONE = -2,
        ACHIEVE_BONUS_TYPE_COINS = 0,
        ACHIEVE_BONUS_TYPE_MERCENARY,
        ACHIEVE_BONUS_TYPE_CHARACTER,
        ACHIEVE_BONUS_TYPE_BATTLEMAP,
        ACHIEVE_BONUS_TYPE_MAX
};
 
enum{
        REWARDS_STATUS_OPEN = 1,
        REWARDS_STATUS_CLOSED,
        REWARDS_STATUS_FINISHED,
};


struct AchievementData {
        bool isNUll(){
                return bonus_status == ACHIEVE_BONUS_TYPE_NONE;
        }
        AchievementData emptyInstance(int num){
                bonus_coinsNum = num;
                bonus_status = ACHIEVE_BONUS_TYPE_NONE;
                return *this;
        }
        int     _index;
        std::string cache_key;
        int bonus_status;
        int bonus_coinsNum;
        int bonus_mercenaryNum;
        std::string bonus_charactor_key;
        std::string bonus_map_key;
        std::string title;
        std::string desc;
        std::string cup_name;
};

typedef struct MercenaryItem_tag{
        int             itemValue;
        int             itemPrice;
        std::string     itemTips;
        std::string     valueNoImg;
        std::string     priceNoImg;
}MercenaryItem;

enum {
        CURRENT_GAME_LEVEL_EASY = 1,
        CURRENT_GAME_LEVEL_NORMAL,
        CURRENT_GAME_LEVEL_HARD
};

class GolbalConfig:public Ref{
public:
        static GolbalConfig* getInstance();
        
        std::vector<AchievementData> getAchievementData();
        
        inline  const std::vector<MercenaryItem>& getMercenaryPriceData(){
                return this->_mercenaryItemPriceData;
        }
        AchievementData getSingleAchievement(std::string key){
                return this->_systemAchievementData.at(key);
        }
        static int CURRENT_GAME_LEVEL;
protected:
        GolbalConfig();
        ~GolbalConfig();
        bool init();
        void initBasic();
        void initAnimation();
        void initAchievementData();
        void initMercenaryItemData();
        
private:
        void initZhanShi();
        void initXunShouShi();
        void initGongJianShou();
        void initHuoQianShou();
        void initMaonv();
        void initChangQiangShou();
        void initMonv();
        void initRenZhe();

private:
        std::map<std::string, AchievementData>  _systemAchievementData;
        std::vector<MercenaryItem>              _mercenaryItemPriceData;
};
#endif /* APPConstants_hpp */
