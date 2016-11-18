//
//  FindPlayerScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//

#include "FindPlayerScene.hpp"
#include "PopUpOkCancelDialog.hpp"
#include "BattleFieldScene.hpp"
#include "ModalDialog.hpp"
#include "UserSessionBean.hpp"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/rapidjson.h"



#define SEARCHING_OPPENT        "searching"
#define CREATE_BATTLEFIELD      "create_field"

Scene* FindPlayer::createScene(){
        auto scene = Scene::create();
        auto layer = FindPlayer::create();
        scene->addChild(layer);
        
        return scene;
}


FindPlayer::~FindPlayer(){
}


bool FindPlayer::init(){
        
        if ( !Layer::init() ) {
                return false;
        }
        
        auto winSize = Director::getInstance()->getWinSize();
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        auto searchingBtn = MenuItemImage::create("search_oppent.png", "search_oppent_sel.png",
                                                  CC_CALLBACK_1(FindPlayer::menuSearching, this));
        searchingBtn->setPosition(Vec2(origin.x + visibleSize.width / 2 ,
                                       origin.y +  visibleSize.height/ 4));
        
        auto menu = Menu::create(searchingBtn, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu);

        return true;
}

void FindPlayer::menuSearching(Ref*){
        ModalLayer::showModalDialog(this);
        
        if ( nullptr != _wsiSendText
            && network::WebSocket::State::CLOSED != _wsiSendText->getReadyState()){
                return ModalLayer::dismissDialog(this);
        }
        
        
        _wsiSendText = new network::WebSocket();
        std::string socket_url = WEB_SOCKET_SERVER_URL"?userId=";
        std::string fb_uid = UserSessionBean::getInstance()->getUserId();
        socket_url.append(fb_uid);
        
        if (!_wsiSendText->init(*this, socket_url)){
                CC_SAFE_DELETE(_wsiSendText);
        }else{
                ModalLayer::dismissDialog(this);
                log("ERROR:failed init the websocket.");
        }
}

void FindPlayer::afterAnimation(){
        
}

void FindPlayer::sendAuthorData(){
        rapidjson::Document document;
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        std::string fb_uid = UserSessionBean::getInstance()->getUserId();
        document.AddMember("user_id", fb_uid, allocator);
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        
        this->sendMessage(buffer.GetString());
}

int  FindPlayer::sendMessage(std::string msg){
        if (!_wsiSendText){
                return -1;
        }
        
        if (_wsiSendText->getReadyState() == network::WebSocket::State::OPEN){
                _wsiSendText->send(msg);
                return (int)msg.length();
        }
        else{
                log("ERROR:failed init the websocket.");
                return -1;
        }
}

#pragma mark - websocket delegate method
void FindPlayer::onOpen(cocos2d::network::WebSocket* ws){
        log("Websocket (%p) opened", ws);
        this->sendAuthorData();
}

void FindPlayer::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data){
        std::string msg(data.bytes);
        log("-----------%s---------------", msg.c_str());
        
        rapidjson::Document msg_d;
        msg_d.Parse<0>(msg.c_str());
        if (msg_d.HasParseError()) {
                CCLOG("GetParseError %u\n",msg_d.GetParseError());
        }
}

void FindPlayer::onClose(network::WebSocket* ws){
        log("websocket instance (%p) closed.", ws);
        
        _wsiSendText = nullptr;
        ModalLayer::dismissDialog(this);
        CC_SAFE_DELETE(ws);
}

void FindPlayer::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error){
        ModalLayer::dismissDialog(this);
        log("Error was fired, error code: %d", static_cast<int>(error));
}
