//
//  WebSocktUtil.hpp
//  Conqueror
//
//  Created by wsli on 16/11/8.
//
//

#ifndef WebSocktUtil_hpp
#define WebSocktUtil_hpp

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/WebSocket.h"

typedef std::function<void(const std::string)> wsCallBack;

class WebSocktUtil:public cocos2d::Ref, public cocos2d::network::WebSocket::Delegate{
public:
        WebSocktUtil(wsCallBack);
        virtual bool init(){return true;}
        virtual ~WebSocktUtil();
        CREATE_FUNC2(WebSocktUtil, wsCallBack, callback);
public:
        void startConnect();
        void sendMessage(std::string);
        void setMessageReceiver();
protected:
        
        virtual void onOpen(cocos2d::network::WebSocket* ws)override;
        virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)override;
        virtual void onClose(cocos2d::network::WebSocket* ws)override;
        virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)override;
        
private:
        cocos2d::network::WebSocket* _wsiSendText;
        wsCallBack      _msgReceiver;
};

#endif /* WebSocktUtil_hpp */
