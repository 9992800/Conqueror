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
#include "network/WebSocket.h"
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


class FindPlayer : public cocos2d::Layer , public cocos2d::network::WebSocket::Delegate, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
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
        
        virtual void onOpen(cocos2d::network::WebSocket* ws)override;
        virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)override;
        virtual void onClose(cocos2d::network::WebSocket* ws)override;
        virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)override;
        void onHttpRequestCompleted(HttpClient *sender,
                               HttpResponse *response);
        
        virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {};
        virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
        virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
        virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
        virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
        virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
        
        int  sendMessage(std::string);
        void afterAnimation();
        void menuSearching(Ref*);
        void menuCreateBattle(Ref*);
        void menuExit(Ref* pSender);
        void menuRefresh(Ref*);
        void sendAuthorData();
        void initTableView(Size, Vec2);
        
private:
        void getBattleListFromServer();
        void parseBattleFieldBeans(rapidjson::Value&);
        
        
private:
        network::WebSocket* _wsiSendText;
        int             _curMapSel;
        
        
        int             _loadingCount;
        LoadingBar*     _loadingBar;
        
        MenuItemImage* _refreshBtn;
        
        extension::TableView*           _battleTableView;
        std::vector<BattleFieldBean*>   _battlList;
        int                             _curPgaeNo;
};

#endif /* FindPlayerScene_hpp */
