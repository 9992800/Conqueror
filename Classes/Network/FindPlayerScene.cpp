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
#include "OnlineGameData.hpp" 
#include "SpriteEx.hpp"


#define SEARCHING_OPPENT        "searching"
#define LIST_ALL_BATTLES        "list_all_battles"
#define CREATE_BATTLEFIELD      "create_field"

using namespace cocos2d::extension;

Scene* FindPlayer::createScene(){
        auto scene = Scene::create();
        auto layer = FindPlayer::create();
        scene->addChild(layer);
        
        return scene;
}


FindPlayer::~FindPlayer(){
//        for(auto& v: _battlList){
//                v->release();
//        }
//        _battlList.clear();
}


bool FindPlayer::init(){
        
        if ( !Layer::init() ) {
                return false;
        }
        
        auto winSize = Director::getInstance()->getWinSize();
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        auto bg = LayerColor::create(Color4B::WHITE);
        this->addChild(bg);
        
        auto searchingBtn = MenuItemImage::create("search_oppent.png", "search_oppent_sel.png",
                                                  CC_CALLBACK_1(FindPlayer::menuSearching, this));
        searchingBtn->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100,
                                       origin.y +  visibleSize.height/ 4));
        
        auto createBtn = MenuItemImage::create("search_oppent_sel.png", "search_oppent.png",
                                                  CC_CALLBACK_1(FindPlayer::menuCreateBattle, this));
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
        this->addChild(menu, 3);
        
        _searchingBgLayer = LayerColor::create(Color4B::GRAY, visibleSize.width/2, visibleSize.height/2);
        this->addChild(_searchingBgLayer, 4);
        _searchingBgLayer->setVisible(false);
        return true;
}


void FindPlayer::menuExit(Ref* pSender){
        Director::getInstance()->popScene();
}

void FindPlayer::menuRefresh(Ref* pSender){
        _refreshBtn->runAction(RotateBy::create(4, 360*4));
        
}

void FindPlayer::menuCreateBattle(Ref*){
        
        HttpRequest* request = new (std::nothrow) HttpRequest();
        request->setUrl(GAME_SERVICE_SERVER_URL"/createBattle");
        request->setRequestType(HttpRequest::Type::POST);
        request->setResponseCallback(CC_CALLBACK_2(FindPlayer::onHttpRequestCompleted, this));       
        
        std::string uid         = UserSessionBean::getInstance()->getUserId();
        std::string uname       = UserSessionBean::getInstance()->getUserName();
        
        auto data = OnlineGameData::create();
        data->retain();
        
        std::string parameters = "user_id=" +uid +"&user_name="+ uname +"&cell_data=" + data->getMapData();
        request->setRequestData(parameters.c_str(), parameters.length());
        request->setUserData(data);
        
        request->setTag(CREATE_BATTLEFIELD);
        HttpClient::getInstance()->send(request);
        request->release();
}

void FindPlayer::menuSearching(Ref*){
        ModalLayer::showModalDialog(this);
        
        if (nullptr == _waitingQueue ||
            WebSocket::State::OPEN != _waitingQueue->getReadyState()){
                _waitingQueue = new network::WebSocket();
                std::string fb_uid = UserSessionBean::getInstance()->getUserId();
                
                std::string socket_url = StringUtils::format("%s%s?userId=%s", WEB_SOCKET_SERVER_BASE_URL, NET_WORK_FIND_COMPONET, fb_uid.c_str());
                
                if (!_waitingQueue->init(*this, socket_url)){
                        CC_SAFE_DELETE(_waitingQueue);
                }else{
                        ModalLayer::dismissDialog(this);
                        log("ERROR:failed init the websocket.");
                }
        }else{
                std::string msg = StringUtils::format("");
                _waitingQueue->send(msg);
        }
        
}

void FindPlayer::afterAnimation(){
}

#pragma mark - update
void FindPlayer::onEnter(){
        Layer::onEnter();
        _loadingCount = 0;
}

void FindPlayer::update(float delta){
}

void FindPlayer::onExit(){
        Layer::onExit();
}

void FindPlayer::onHttpRequestCompleted(HttpClient *sender,
                                        HttpResponse *response){         
        ModalLayer::dismissDialog(this);
        
        picojson::value data;
        if (!UserSessionBean::checkResponse(response, data)){
                auto data = (Ref*)response->getHttpRequest()->getUserData();
                if (data){
                        data->release();
                }
                return;
        }
        
        std::string tags = response->getHttpRequest()->getTag();
        if (0 == tags.compare(CREATE_BATTLEFIELD)){
                CCLOG("---%s---", data.to_str().c_str());
                auto game_data = (OnlineGameData*)response->getHttpRequest()->getUserData();
                game_data->setServerId(data.to_str());
                game_data->setRole(BATTLE_FIELD_ROLE_CREATOR);
                auto scene = BattleField::createScene(game_data);
                Director::getInstance()->pushScene(scene);
                
        }else if (0 == tags.compare(SEARCHING_OPPENT)){
                
        }else if (0 == tags.compare(LIST_ALL_BATTLES)){
//                this->parseBattleFieldBeans(data);
        }else{
                CCLOGWARN("---Unkown request tag:%s---", tags.c_str());
        }
}

#pragma mark - websocket delegate
void FindPlayer::onOpen(cocos2d::network::WebSocket* ws){
        log("Websocket (%p) opened", ws);
}

void FindPlayer::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data){
        std::string msg(data.bytes);
        log("-----------%s---------------", msg.c_str());
        
        picojson::value json_result;
        std::string error = picojson::parse(json_result, msg);
        if (error.length() > 0) {
                CCLOGWARN("---data from game server failed: %s\n",error.c_str());
                return;
        }
        
        picojson::object result = json_result.get<picojson::object>();
}

void FindPlayer::onClose(network::WebSocket* ws){
        log("websocket instance (%p) closed.", ws);
        
        _waitingQueue = nullptr;
        ModalLayer::dismissDialog(this);
        CC_SAFE_DELETE(ws);
}

void FindPlayer::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error){
        ModalLayer::dismissDialog(this);
        log("Error was fired, error code: %d", static_cast<int>(error));
}

