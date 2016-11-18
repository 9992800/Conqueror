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
        const int MARGIN = 40;
        const int SPACE = 35;         
        
        auto searchingBtn = MenuItemImage::create("search_oppent.png", "search_oppent_sel.png",
                                                  CC_CALLBACK_1(FindPlayer::menuSearching, this));
        searchingBtn->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100,
                                       origin.y +  visibleSize.height/ 4));
        
        auto createBtn = MenuItemImage::create("search_oppent_sel.png", "search_oppent.png",
                                                  CC_CALLBACK_1(FindPlayer::menuCreateGame, this));
        createBtn->setPosition(Vec2(origin.x + visibleSize.width / 2 + 100,
                                       origin.y +  visibleSize.height/ 4));
        
        auto menu = Menu::create(searchingBtn, createBtn, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu);
        
        // Response Code Label
        _labelStatusCode = Label::createWithTTF("waiting...", "fonts/arial.ttf", 18);
        _labelStatusCode->setPosition(winSize.width / 2,  winSize.height - MARGIN - 6 * SPACE);
        _labelStatusCode->setVisible(false);
        this->addChild(_labelStatusCode);
        
        return true;
}

void FindPlayer::menuSearching(Ref*){
        
}


void FindPlayer::menuCreateGame(Ref*){
        
        ModalLayer::showModalDialog(this); 
        
        HttpRequest* request = new (std::nothrow) HttpRequest();
        std::string base_url(GAME_SERVICE_SERVER_URL"/createBattle?");
        std::string uid = UserSessionBean::getInstance()->getUserId();
        
        base_url.append("user_id=");
        base_url.append(uid);
        
        request->setUrl(base_url);
        request->setRequestType(HttpRequest::Type::GET);
        request->setResponseCallback(CC_CALLBACK_2(FindPlayer::onHttpRequestCompleted, this));
        request->setTag(CREATE_BATTLEFIELD);
        HttpClient::getInstance()->sendImmediate(request);
        request->release();
}

void FindPlayer::onMessage(std::string message){
        CCLOGWARN("---online---=%s", message.c_str());
}

void FindPlayer::afterAnimation(){
        
}


void FindPlayer::onHttpRequestCompleted(HttpClient *sender,
                                        HttpResponse *response){
        
        ModalLayer::dismissDialog(this);
        
        if (!response){
                return;
        }
        
        log("response code: %ld", response->getResponseCode());
        if (!response->isSucceed()){
                log("error buffer: %s", response->getErrorBuffer());
                return;
        }
        std::string response_str = response->getResponseDataString();
        log("response string: %s", response_str.c_str());
        
        
        std::string tags = response->getHttpRequest()->getTag();
        if (0 == tags.compare(CREATE_BATTLEFIELD)){
                
        }else if (0 == tags.compare(SEARCHING_OPPENT)){
                
        }else{
                CCLOGWARN("---Unkown request tag:%s---", tags.c_str());
        }
}
