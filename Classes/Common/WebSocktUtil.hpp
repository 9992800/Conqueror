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

USING_NS_CC;

typedef std::function<void(std::string)> wsCallBack;

class WebSocktUtil:public cocos2d::Ref, public cocos2d::network::WebSocket::Delegate{
public:
        WebSocktUtil(wsCallBack);
        virtual bool init(){return true;}
        virtual ~WebSocktUtil();
        CREATE_FUNC2(WebSocktUtil, wsCallBack, callback);
public:
        void startConnect();
        int  sendMessage(std::string);
        void setMessageReceiver();
        inline network::WebSocket* getCurrentSocekt(){
                return _wsiSendText;
        }
        
protected:
        
        virtual void onOpen(cocos2d::network::WebSocket* ws)override;
        virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)override;
        virtual void onClose(cocos2d::network::WebSocket* ws)override;
        virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)override;
        
private:
        network::WebSocket* _wsiSendText;
        wsCallBack      _msgReceiver;
};

#endif /* WebSocktUtil_hpp */
