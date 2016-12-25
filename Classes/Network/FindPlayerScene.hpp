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
#include "picojson.h"
#include "network/WebSocket.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace network;

class BattleFieldCell : public cocos2d::extension::TableViewCell
{
public:
        BattleFieldCell(Size);
        virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
};


class FindPlayer : public cocos2d::Layer , public cocos2d::network::WebSocket::Delegate
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(FindPlayer);
        FindPlayer():_waitingQueue(nullptr){}
        virtual ~FindPlayer();
        
protected:
        
        void onEnter() override;
        void update(float delta)override;
        void onExit()override;
        
        virtual void onOpen(network::WebSocket* ws)override;
        virtual void onMessage(network::WebSocket* ws, const network::WebSocket::Data& data)override;
        virtual void onClose(network::WebSocket* ws)override;
        virtual void onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error)override;
        
        void onHttpRequestCompleted(HttpClient *sender,
                                    HttpResponse *response);
        
        void afterAnimation();
        void menuSearching(Ref*);
        void menuCreateBattle(Ref*);
        void menuExit(Ref* pSender);
        void menuRefresh(Ref*);
        
private:
        int             _curMapSel;
        int             _loadingCount;
        LoadingBar*     _loadingBar;
        
        MenuItemImage* _refreshBtn;
        LayerColor*    _searchingBgLayer;        
        network::WebSocket* _waitingQueue;
};

#endif /* FindPlayerScene_hpp */
