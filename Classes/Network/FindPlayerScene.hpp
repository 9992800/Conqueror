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
#include "WebSocktUtil.hpp"
#include "network/HttpClient.h"

USING_NS_CC;
using namespace network;


class FindPlayer : public cocos2d::Layer , public cocos2d::network::WebSocket::Delegate
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(FindPlayer);
        virtual ~FindPlayer();
        
protected:
        
        virtual void onOpen(cocos2d::network::WebSocket* ws)override;
        virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)override;
        virtual void onClose(cocos2d::network::WebSocket* ws)override;
        virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)override;
        
        int  sendMessage(std::string);
        void afterAnimation();
        void menuSearching(Ref*);
        void sendAuthorData();
private:
        network::WebSocket* _wsiSendText;
        int             _curMapSel;
};

#endif /* FindPlayerScene_hpp */
