//
//  WebSocktUtil.cpp
//  Conqueror
//
//  Created by wsli on 16/11/8.
//
//

#include "AppMacros.hpp"
#include "WebSocktUtil.hpp"
USING_NS_CC;
USING_NS_CC_EXT;

WebSocktUtil::WebSocktUtil(wsCallBack cb)
:_wsiSendText(nullptr),
_msgReceiver(cb){
        
}

WebSocktUtil::~WebSocktUtil(){
        if (_wsiSendText){
                _wsiSendText->close();
        }
}


void WebSocktUtil::startConnect(){
        _wsiSendText = new network::WebSocket();
        if (!_wsiSendText->init(*this, WEB_SOCKET_SERVER_URL)){
                CC_SAFE_DELETE(_wsiSendText);
        }else{
                log("ERROR:failed init the websocket.");
        }
}


void WebSocktUtil::sendMessage(std::string){
        
        if (! _wsiSendText){
                return;
        }
        
        if (_wsiSendText->getReadyState() == network::WebSocket::State::OPEN){
                _wsiSendText->send("Hello WebSocket, I'm a text message.");
        }
        else{
                log("ERROR:failed init the websocket.");
        }
}


#pragma mark - websocket delegate method
void WebSocktUtil::onOpen(cocos2d::network::WebSocket* ws){
        log("Websocket (%p) opened", ws);
}

void WebSocktUtil::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data){
        std::string msg(data.bytes);
        log("%s", msg.c_str());
       
        if (_msgReceiver){
                _msgReceiver(msg);
        }
}

void WebSocktUtil::onClose(network::WebSocket* ws){
       log("websocket instance (%p) closed.", ws);
        
        _wsiSendText = nullptr;
        CC_SAFE_DELETE(ws);
}

void WebSocktUtil::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error){
        log("Error was fired, error code: %d", static_cast<int>(error));
}

