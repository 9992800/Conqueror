//
//  FindPlayerScene.hpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//

#ifndef FindPlayerScene_hpp
#define FindPlayerScene_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AppMacros.hpp"
#include "extensions/cocos-ext.h"
#include "BattleFieldBean.hpp"
#include "network/HttpClient.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace network;

class BattleFieldCell : public cocos2d::extension::TableViewCell
{
public:
        BattleFieldCell(Size);
        virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
};


class FindPlayer : public cocos2d::Layer 
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(FindPlayer);
        virtual ~FindPlayer();
        
protected:
        
        void onEnter() override;
        void update(float delta)override;
        void onExit()override;
        
        void onHttpRequestCompleted(HttpClient *sender,
                                    HttpResponse *response);
        
        void pageViewEvent(cocos2d::Ref* sender, cocos2d::ui::PageView::EventType type);
        void onBattleSelected(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
        
        
        void afterAnimation();
        void menuSearching(Ref*);
        void menuCreateBattle(Ref*);
        void menuExit(Ref* pSender);
        void menuRefresh(Ref*);
        void initPageViews(Size, Vec2);
        void reloadPageData();
        
private:
        void getBattleListFromServer();
        void parseBattleFieldBeans(rapidjson::Value&);
        
        
private:
        int             _curMapSel;
        
        int             _loadingCount;
        LoadingBar*     _loadingBar;
        
        MenuItemImage* _refreshBtn;
        PageView*                       _batllePageViews;
        std::vector<BattleFieldBean*>   _battlList;
        int                             _curPgaeNo;
};

#endif /* FindPlayerScene_hpp */
