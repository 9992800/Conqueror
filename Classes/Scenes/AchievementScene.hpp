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
#include "APPConstants.hpp"

USING_NS_CC;
#include <stdio.h>
class  Achievement: public cocos2d::Layer
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(Achievement);
        virtual void update(float dt) override;
        Achievement() : _spawnCount(5),
        _bufferZone(90), //bufferZone should be larger than List item width
        _updateTimer(0),
        _updateInterval(1.0f / 24), // you could tweak this value to adjust ListView data update rate
        _lastContentPosY(0), //use this value to detect if we are scrolling left or right
        _itemTemplateHeight(0)
        {
        
        }
private:
        ui::Layout* createListItem();
        void initItemDetails(ui::Widget*, int);
        void updateItem(int itemID, int templateID);
        float getItemPositionYInView(cocos2d::ui::Widget* item) const;
        
        void actionButton(Ref*);
private:
        ui::ListView* _listView;
        int _totalCount, _spawnCount;
        float _itemTemplateHeight, _reuseItemOffset;
        float _updateTimer, _updateInterval;
        float _bufferZone, _lastContentPosY;
        std::vector<AchievementData> _achievementData;
};

#endif /* AchievementScene_hpp */
