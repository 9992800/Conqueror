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

class GolbalAnimations:public Ref{
public:
        static GolbalAnimations* getInstance();
        void initAnimation();
protected:
        GolbalAnimations();
        ~GolbalAnimations();
        bool init();
};

enum {
        ACHIEVE_BONUS_TYPE_NONE = -1,
        ACHIEVE_BONUS_TYPE_COINS = 1,
        ACHIEVE_BONUS_TYPE_MERCENARY,
        ACHIEVE_BONUS_TYPE_CHARACTER,
        ACHIEVE_BONUS_TYPE_BATTLEMAP
};

enum{
        REWARDS_BATTLEMAP_3P = 1,
        REWARDS_BATTLEMAP_4P,
        REWARDS_BATTLEMAP_5P,
        REWARDS_BATTLEMAP_6P,
        REWARDS_BATTLEMAP_7P,
        REWARDS_BATTLEMAP_8P
};


#define ACHIEVE_DATA_KEY_FIRST_WIN_2            "_key_achive_data_first_win_2"
#define ACHIEVE_DATA_KEY_FIRST_WIN_3            "_key_achive_data_first_win_3"
#define ACHIEVE_DATA_KEY_FIRST_WIN_4            "_key_achive_data_first_win_4"
#define ACHIEVE_DATA_KEY_FIRST_WIN_5            "_key_achive_data_first_win_5"
#define ACHIEVE_DATA_KEY_FIRST_WIN_6            "_key_achive_data_first_win_6"
#define ACHIEVE_DATA_KEY_FIRST_WIN_7            "_key_achive_data_first_win_7"
#define ACHIEVE_DATA_KEY_FIRST_WIN_8            "_key_achive_data_first_win_8"

#define ACHIEVE_DATA_KEY_WIN_2TIMES             "_key_achive_data_win_2times"
#define ACHIEVE_DATA_KEY_WIN_5TIMES             "_key_achive_data_win_5times"
#define ACHIEVE_DATA_KEY_WIN_10TIMES            "_key_achive_data_win_10times"

#define ACHIEVE_DATA_KEY_FIRST_USE_MERCENARY    "_key_achive_data_first_use_mercenary"
#define ACHIEVE_DATA_KEY_FIRST_BUY_COINS        "_key_achive_data_first_buy_coins"
#define ACHIEVE_DATA_KEY_FIRST_ENGAGE           "_key_achive_data_first_engage"
#define ACHIEVE_DATA_KEY_FIRST_SHARE            "_key_achive_data_first_share_game"


struct AchievementData {
        std::string cache_key;
        bool bonus_status;
        int bonus_type1;
        int bonus_value1;
        int bonus_type2;
        int bonus_value2;
        int bonus_type3;
        int bonus_value3;
        int bonus_type4;
        int bonus_value4;
        std::string title;
        std::string desc;
};

extern std::vector<AchievementData> SYSTEM_ACHIEVE_DATA;
extern void INIT_ACHIEVE_DATA_BYDEFAULT();

#endif /* APPConstants_hpp */
