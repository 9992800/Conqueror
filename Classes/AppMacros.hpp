#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"
#include "APPConstants.hpp"

typedef struct tagResource
{
    cocos2d::Size size;
    char directory[100];
}Resource;


static Resource smallResource  =  { cocos2d::Size(1136, 640),   "iphone5" }; 
static Resource largeResource  =  { cocos2d::Size(1704, 960),   "iphone6p"};
static cocos2d::Size designResolutionSize = cocos2d::Size(1136, 640);

//static Resource smallResource  =  { cocos2d::Size(480, 320),   "iphone5" };
//static Resource mediumResource =  { cocos2d::Size(1024, 768),   "iphone6" };
//static Resource largeResource  =  { cocos2d::Size(2048, 1536),  "iphone6p"};
//static Size designResolutionSize = cocos2d::Size(480, 320);


#define TITLE_FONT_SIZE(size)  (cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width / smallResource.size.width * (size))


#define XMAX (32)
#define YMAX (28)
#define PUT_DICE (3)
#define STOCK_MAX (64)
#define AREA_MAX (32)
#define CEL_MAX (XMAX * YMAX)
#define MAX_PLAYER (8)
#define MAX_DICE_PER_AREA (8)



#define SET_SIZE_TOZERO(target, SIZE) {\
for (int i = 0; i < SIZE; i++){\
target[i] = 0;\
}\
} \


#define SET_SIZE_TOZERO2(target1, target2, SIZE) {\
for (int i = 0; i < SIZE; i++){\
target1[i] = 0;\
target2[i] = 0;\
}\
} \

#define SET_SIZE_TOIDX(target, SIZE) {\
for (int i = 0; i < SIZE; i++){\
target[i] = i;\
}\
}

//
#define MAX_LINE_INAREA (100)
#define DIR_INAREA (6)
#define AREA_UNSELECTED (-1)

#define AREA_BASE_TAG_INMAP (200)
#define AREA_TAG_ID_INMAP(i) (i + AREA_BASE_TAG_INMAP)
#define AREA_SPRITE_ZORDER (3)
enum {
        CELL_DIR_RIGHT_TOP = 0,
        CELL_DIR_RIGHT_MID = 1,
        CELL_DIR_RIGHT_BOT = 2,
        CELL_DIR_LEFT_TOP  = 3,
        CELL_DIR_LEFT_MID  = 4,
        CELL_DIR_LEFT_BOT  = 5
};

enum {
        GAME_STATUS_INIT = 0,
        GAME_STATUS_AIRUNNING,
        GAME_STATUS_INUSERTURN,
        GAME_STATUS_FINISHED
};

enum {
        ATTACK_RES_NONE         = -1,
        ATTACK_RES_GOTSUPPLY    = 0,
        ATTACK_RES_WIN,
        ATTACK_RES_DEFEATED
};

#define AI_ATTACK_THINKING_SIZE (AREA_MAX * AREA_MAX) 

#define EFFECT_FILE_START_GAME          "sounds/start.mp3"
#define EFFECT_FILE_CHANGE_MAP          "sounds/new_map.mp3"
#define EFFECT_FILE_WIN                 "sounds/win.mp3"
#define EFFECT_FILE_DEFEAT              "sounds/defeat.mp3"
#define EFFECT_FILE_SELECTED            "sounds/selected.mp3" 
#define EFFECT_FILE_SUPPLY              "sounds/supply.mp3"
#define EFFECT_FILE_FINISH_LOSE         "sounds/finish_lose.mp3"
#define EFFECT_FILE_FINISH_WIN          "sounds/finish_win.mp3"
#define BACK_MUSIC_LEVEL_SELECT         "sounds/level_select_bm.mp3"
#define BACK_MUSIC_IN_BATTLE            "sounds/battle_scene_bm.mp3"
#define EFFECT_BATTLE_IN_ANIM           "sounds/fingt_inAnim.mp3"
#define EFFECT_MARCH_IN_ANIM            "sounds/march.mp3"


#define SOUND_MUSIC_TOTAL_KEY           "_key_sound_music_total_switch"
#define SOUND_EFFECT_SWITCH_KEY         "_key_sound_effect_switch"
#define SOUND_EFFECT_VALUE_KEY          "_key_sound_effect_value"
#define BACK_MUSIC_SWITCH_KEY           "_key_back_music_switch"
#define BACK_MUSIC_VALUE_KEY            "_key_back_music_value"
#define ANIMATION_SWITCH_KEY            "_key_animation_switch"
#define GAME_SPEED_KEY                  "_key_game_speed_value"


#define GAME_HISTORY_MAP_KEY            "_key_history_map_value"
#define GAME_HISTORY_PLAYER_NUM         "_key_history_player_num"
#define GAME_HISTORY_CELL_INFO          "_key_history_cell_info"
#define GAME_HISTORY_AREA_INFO          "_key_history_area_info"
#define GAME_HISTORY_FROM_KEY           "_key_history_from_value"
#define GAME_HISTORY_TO_KEY             "_key_history_to_value"
#define GAME_HISTORY_RES_KEY            "_key_history_res_value"
#define GAME_HISTORY_SUPPLY_KEY         "_key_history_supply_value"

#define LEVEL_2_LOCK_STATE_KEY          "_key_level_2_lock_state"
#define LEVEL_3_LOCK_STATE_KEY          "_key_level_3_lock_state"
#define LEVEL_4_LOCK_STATE_KEY          "_key_level_4_lock_state"
#define LEVEL_5_LOCK_STATE_KEY          "_key_level_5_lock_state"
#define LEVEL_6_LOCK_STATE_KEY          "_key_level_6_lock_state"
#define LEVEL_7_LOCK_STATE_KEY          "_key_level_7_lock_state"
#define LEVEL_8_LOCK_STATE_KEY          "_key_level_8_lock_state"
#define ADS_LOCK_STATE_KEY              "_key_ads_lock_state"


#define GAME_STATUS_GUIDE_FINISHED      "_key_game_status_guide_finished"
#define GAME_DIFFICULT_LEVEL            "_key_game_difficult_level"

#define CHARACTOR_1_LOCK_STATE_KEY      "_key_charactor_1_lock_state"
#define CHARACTOR_2_LOCK_STATE_KEY      "_key_charactor_2_lock_state"
#define CHARACTOR_3_LOCK_STATE_KEY      "_key_charactor_3_lock_state"
#define CHARACTOR_4_LOCK_STATE_KEY      "_key_charactor_4_lock_state"
#define CHARACTOR_5_LOCK_STATE_KEY      "_key_charactor_5_lock_state"
#define CHARACTOR_6_LOCK_STATE_KEY      "_key_charactor_6_lock_state"
#define CHARACTOR_7_LOCK_STATE_KEY      "_key_charactor_7_lock_state"
#define CHARACTOR_8_LOCK_STATE_KEY      "_key_charactor_8_lock_state"


#define LEVEL_2_PRODUCT_ID_KEY          "com.bellflower.Conqueror.open_level2"
#define LEVEL_3_PRODUCT_ID_KEY          "com.bellflower.Conqueror.open_level3"
#define LEVEL_4_PRODUCT_ID_KEY          "com.bellflower.Conqueror.open_level4"
#define LEVEL_5_PRODUCT_ID_KEY          "com.bellflower.Conqueror.open_level5"
#define LEVEL_6_PRODUCT_ID_KEY          "com.bellflower.Conqueror.open_level6"
#define LEVEL_7_PRODUCT_ID_KEY          "com.bellflower.Conqueror.open_level7"
#define REMOVE_ADS_PRODUCT_ID_KEY       "com.bellflower.Conqueror.removeAds"

#define SHOP_ITEM_ID_10COINS_KEY        "com.bellflower.Conqueror.coins_10"
#define SHOP_ITEM_ID_60COINS_KEY        "com.bellflower.Conqueror.coins_60"
#define SHOP_ITEM_ID_200COINS_KEY       "com.bellflower.Conqueror.coins_200"
#define SHOP_ITEM_ID_680COINS_KEY       "com.bellflower.Conqueror.coins_680"
#define SHOP_ITEM_ID_1480COINS_KEY      "com.bellflower.Conqueror.coins_1480"

#define SHOP_ITME_VALUE_10COINS         (10)
#define SHOP_ITME_VALUE_60COINS         (60)
#define SHOP_ITME_VALUE_200COINS        (200)
#define SHOP_ITME_VALUE_680COINS        (680)
#define SHOP_ITME_VALUE_1480COINS       (1480)

#define USER_CURRENT_COINS              "_key_user_current_coins_v"
#define USER_CURRENT_SUPPLY_NO          "_key_user_current_supply_no"

#if COCOS2D_DEBUG >= 1
        #define USER_DEFAULT_COINS_ONFIRST      (300)
#else
        #define USER_DEFAULT_COINS_ONFIRST      (30)
#endif


#define USER_DEFAULT_SUPPLYNO_ONFIRST   (3)
#define TURN_INTERVAL_FOR_SUPPLEMENTS   (4)
#define TC_VALUE_ONE_SUPPLY             (10)
#define PRICE_PER_SUPPLEMENT            (10)

#define FACEBOOK_INFO_USER_AVATAR_KEY     "_key_facebook_avatar_id"
#define FACEBOOK_INFO_USER_FB_BASIC       "_key_facebook_user_name"

template <typename T> std::string tostr(const T& t) { std::ostringstream os; os<<t; return os.str(); }

#define CREATE_FUNC2(__TYPE__, __ARGT__, __ARGV__) \
static __TYPE__* create(__ARGT__  __ARGV__) \
{ \
__TYPE__ *pRet = new(std::nothrow) __TYPE__(__ARGV__); \
if (pRet && pRet->init()) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = nullptr; \
return nullptr; \
} \
}


#define LAYER_NAME_IN_TILE_MAP "map"

#define NET_WORK_FIND_COMPONET          "findOponent"
#define WEB_SOCKET_SERVER_BASE_URL      "ws://192.168.1.200:9900/"
#define GAME_SERVICE_SERVER_URL         "http://192.168.1.39:9900/serverHome"

template<typename T> extern std::vector<T> parseData(const char* key);

extern std::vector<std::string> parseData(const char* key);


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define usleep(t) Sleep(t)
#else
#include <unistd.h>
#define usleep(t) usleep(t)
#endif

#define NETWORK_WORK_WELL 10000


enum{
        WEB_SOCKET_MSG_TYPE_KA = 1,
};

enum{
        BATTLE_FIELD_ROLE_CREATOR = 1,
        BATTLE_FIELD_ROLE_JOINER
};

#define TILE_COLOR_BACKGRUND Color4B(0x3E, 0xA5, 0xDD, 255)
//Color4F((float)0x3E/0xff, (float)0xA5/0xff, (float)0xDD/0xff, 1),//back ground

enum {
        PLAYER_ROLE_TYPE_ZHANSHI = 0,
        PLAYER_ROLE_TYPE_XUNSHOUSHI,
        PLAYER_ROLE_TYPE_GONGJIANSHOU,
        PLAYER_ROLE_TYPE_HUOQIANSHOU,
        PLAYER_ROLE_TYPE_MAONV,
        PLAYER_ROLE_TYPE_CHANGQIANGSHOU,
        PLAYER_ROLE_TYPE_SHOUREN,
        PLAYER_ROLE_TYPE_DAOZEI,
        
        FIGHT_ANIM_TYPE_XINYUN,
        FIGH_ANIM_MAX
};

enum {
        ONLINE_MESSAGE_REQUEST_TYPE_FIND_COMPONENT = 1001,
        ONLINE_MESSAGE_RESPONSE_TYPE_FIND_COMPONENT = 3001
};

#define USER_DEVICE_UUID        "user_device_uuid"


#define SUPER_LAYER_PRIVILIEGE 10000
#define MODAL_DIALOG_NODETAG 10000
#define COMMON_TIPS_NODETAG 11000


#define COINS_ANIM_SHOW_NUM (10)

#endif /* __APPMACROS_H__ */
