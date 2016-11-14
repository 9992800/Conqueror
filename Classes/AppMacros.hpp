#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"

USING_NS_CC;

typedef struct tagResource
{
    cocos2d::Size size;
    char directory[100];
}Resource;


static Resource smallResource  =  { cocos2d::Size(1136, 640),   "iphone5" };
static Resource mediumResource =  { cocos2d::Size(1334, 750),   "iphone6" };
static Resource largeResource  =  { cocos2d::Size(1920, 1080),  "iphone6p"};
static Size designResolutionSize = cocos2d::Size(1136, 640);

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

enum {
        GAME_OVER_WIN = 0,
        GAME_OVER_DEFEATED
};

#define AI_ATTACK_THINKING_SIZE (AREA_MAX * AREA_MAX) 

#define EFFECT_FILE_START_GAME          "sounds/start.mp3"
#define EFFECT_FILE_WIN                 "sounds/win.mp3"
#define EFFECT_FILE_DEFEAT              "sounds/defeat.mp3"
#define EFFECT_FILE_SELECTED            "sounds/selected.mp3"
#define EFFECT_FILE_DROP_DICE           "sounds/drop_dice.mp3"
#define EFFECT_FILE_SUPPLY              "sounds/supply.mp3"
#define EFFECT_FILE_FINISH_LOSE         "sounds/finish_lose.mp3"
#define EFFECT_FILE_FINISH_WIN          "sounds/finish_win.mp3"

#define SOUND_EFFECT_SWITCH_KEY         "_key_sound_effect_switch"
#define ANIMATION_SWITCH_KEY            "_key_animation_switch"
#define GAME_SPEED_KEY                  "_key_game_speed_value"
#define GAME_HISTORY_MAP_KEY            "_key_history_map_value"

#define LEVEL_4_LOCK_STATE_KEY          "_key_level_4_lock_state"
#define LEVEL_5_LOCK_STATE_KEY          "_key_level_5_lock_state"
#define LEVEL_6_LOCK_STATE_KEY          "_key_level_6_lock_state"
#define LEVEL_7_LOCK_STATE_KEY          "_key_level_7_lock_state"
#define ADS_LOCK_STATE_KEY              "_key_ads_lock_state"

#define LEVEL_4_PRODUCT_NAME_KEY        "open_level4"
#define LEVEL_5_PRODUCT_NAME_KEY        "open_level5"
#define LEVEL_6_PRODUCT_NAME_KEY        "open_level6"
#define LEVEL_7_PRODUCT_NAME_KEY        "open_level7"
#define REMOVE_ADS_PRODUCT_NAME_KEY     "remove_ads"


#define LAST_GAME_LEVEL_PLAYED_KEY      "_key_last_game_level_played"

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


#define WEB_SOCKET_SERVER_URL      "ws://192.168.1.35:9900/marco"


#endif /* __APPMACROS_H__ */
