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
#endif /* APPConstants_hpp */
