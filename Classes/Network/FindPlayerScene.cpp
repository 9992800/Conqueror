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
#define LIST_ALL_BATTLES        "list_all_battles"
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
        searchingBtn->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100,
                                       origin.y +  visibleSize.height/ 4));
        
        auto createBtn = MenuItemImage::create("search_oppent_sel.png", "search_oppent.png",
                                                  CC_CALLBACK_1(FindPlayer::menuSearching, this));
        createBtn->setPosition(Vec2(origin.x + visibleSize.width / 2 + 100,
                                       origin.y +  visibleSize.height/ 4));
        
        auto return_back = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                                 CC_CALLBACK_1(FindPlayer::menuExit, this));
        return_back->setPosition(Vec2(origin.x + return_back->getContentSize().width + 10,
                                      origin.y + visibleSize.height - return_back->getContentSize().height - 10));
        
        _refreshBtn = MenuItemImage::create("waiting_ring.png","", CC_CALLBACK_1(FindPlayer::menuRefresh, this));
        _refreshBtn->setPosition(Vec2(visibleSize.width - _refreshBtn->getContentSize().width,
                                      _refreshBtn->getContentSize().height));
        
        auto menu = Menu::create(searchingBtn, createBtn, return_back, _refreshBtn, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 9);
        
//        
//        _loadingBar = LoadingBar::create("sliderProgress.png");
//        _loadingBar->setDirection(LoadingBar::Direction::LEFT);
//        _loadingBar->setPercent(0);
//        
//        Vec2 pos = Vec2(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height / 6);
//        _loadingBar->setPosition(pos);
//        this->addChild(_loadingBar, 10);

        return true;
}


void FindPlayer::menuExit(Ref* pSender){
        Director::getInstance()->popScene();
}

void FindPlayer::menuRefresh(Ref* pSender){
        this->getBattleListFromServer();
        _refreshBtn->runAction(RotateBy::create(4, 360*4));
        
}

void FindPlayer::menuCreateBattle(Ref*){
        std::string base_url(GAME_SERVICE_SERVER_URL"/createBattle?");
        std::string uid = UserSessionBean::getInstance()->getUserId();
        
        base_url.append("user_id=");
        base_url.append(uid);
        
        HttpRequest* request = new (std::nothrow) HttpRequest();
        request->setUrl(base_url);
        request->setRequestType(HttpRequest::Type::GET);
        request->setResponseCallback(CC_CALLBACK_2(FindPlayer::onHttpRequestCompleted, this));
        request->setTag(CREATE_BATTLEFIELD);
        HttpClient::getInstance()->sendImmediate(request);
        request->release();
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
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        std::string fb_uid = UserSessionBean::getInstance()->getUserId();
        rapidjson::Value s;
        s.SetString(fb_uid.c_str(), (rapidjson::SizeType)fb_uid.length());
        document.AddMember("user_id", s, allocator);
        
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

void FindPlayer::getBattleListFromServer(){
        std::string base_url(GAME_SERVICE_SERVER_URL"/battleFields?");
        std::string uid = UserSessionBean::getInstance()->getUserId();
        
        base_url.append("user_id=");
        base_url.append(uid);
        
        HttpRequest* request = new (std::nothrow) HttpRequest();
        request->setUrl(base_url);
        request->setRequestType(HttpRequest::Type::GET);
        request->setResponseCallback(CC_CALLBACK_2(FindPlayer::onHttpRequestCompleted, this));
        request->setTag(LIST_ALL_BATTLES);
        HttpClient::getInstance()->sendImmediate(request);
        request->release();
}

#pragma mark - update
void FindPlayer::onEnter(){
        Layer::onEnter();
        _loadingCount = 0;
        this->getBattleListFromServer();
        
        
//        scheduleUpdate();
}

void FindPlayer::update(float delta){
//        _loadingBar->setPercent(_loadingCount++);
}

void FindPlayer::onExit(){
        Layer::onExit();
}

void FindPlayer::onHttpRequestCompleted(HttpClient *sender,
                                        HttpResponse *response){
        
        rapidjson::Value data;
        if (!UserSessionBean::checkResponse(response, data)){
                return;
        }
        
        std::string tags = response->getHttpRequest()->getTag();
        if (0 == tags.compare(CREATE_BATTLEFIELD)){
                
        }else if (0 == tags.compare(SEARCHING_OPPENT)){
                
        }else if (0 == tags.compare(LIST_ALL_BATTLES)){
                
        }else{
                CCLOGWARN("---Unkown request tag:%s---", tags.c_str());
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
