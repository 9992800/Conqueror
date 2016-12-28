//
//  AchievementScene.hpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/12/27.
//
//

#ifndef AchievementScene_hpp
#define AchievementScene_hpp

#include "AppMacros.hpp"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
#include <stdio.h>
class  Achievement: public cocos2d::Layer
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(Achievement);
        
private:
        ui::Layout* createListItem();
private:
        ui::ListView* _listView;
        int _totalCount;
};

#endif /* AchievementScene_hpp */
