//
//  GameConfig.h
//  DiceWars
//
//  Created by wsli on 16/10/20.
//
//

#ifndef GameConfig_h
#define GameConfig_h


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
        GAME_STATUS_AITHINKING,
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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define EFFECT_FILE        "effect2.ogg"
#elif( CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
#define EFFECT_FILE        "effect1.raw"
#else
#define EFFECT_FILE        "effect1.wav"
#endif // CC_PLATFOR_ANDROID

#endif /* GameConfig_h */
