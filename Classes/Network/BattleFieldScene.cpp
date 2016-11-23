//
//  BattleFieldScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/11.
//
//
#include "AppMacros.hpp"
#include "ModalDialog.hpp"
#include "MapCreator.hpp"
#include "ScreenCoordinate.hpp"
#include "BattleFieldScene.hpp"
#include "UserSessionBean.hpp"

enum{
        ZORDER_BACK_GROUND = 0,
        ZORDER_MAP_GROUND,
        ZORDER_CRTL_LAYERS,
        ZORDER_ANIM_LAYER,
        ZORDER_DIALOG_LAYER
};
enum{
        key_map_tag             = 1,
        key_ctrl_layer_tag,
        key_anim_layer_tag,
        key_dialog_layer_tag
};

Scene* BattleField::createScene(OnlineGameData* data){
        auto scene = Scene::create();
        auto layer = BattleField::create(data);
        scene->addChild(layer);
        
        return scene;
}

BattleField::~BattleField(){
        _onlineMapData->release();
}

bool BattleField::init(){
        if ( !Layer::init() ) {
                return false;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        auto back_ground = Sprite::create("online_game_bg.png");
        back_ground->setPosition(center);
        this->addChild(back_ground, ZORDER_BACK_GROUND);
        
        auto return_back = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                                 CC_CALLBACK_1(BattleField::menuExit, this));
        return_back->setPosition(Vec2(origin.x + return_back->getContentSize().width + 10,
                                      origin.y + visibleSize.height - return_back->getContentSize().height - 10));
        
        auto menu = Menu::create(return_back, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, ZORDER_CRTL_LAYERS);
        
        
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
        
        return true;
}

void BattleField::menuExit(Ref* pSender){
        //TODO::show pop dialog
        Director::getInstance()->popScene();
}

void BattleField::startBattle(){
        auto map = MapCreator::instance()->createMap(_onlineMapData->getIntMapData());
        Size map_size = map->getContentSize();
        ScreenCoordinate::getInstance()->configScreen(map_size);
        
        _onlineMapData->reshDataByMapInfo(map);
        
        this->addChild(map, ZORDER_MAP_GROUND, key_map_tag);
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        _lowestPostion_y = visibleSize.height + origin.y - map_size.height - 6;//TODO::
}

#pragma mark - overrided scedule
void BattleField::onEnter(){
        Layer::onEnter();
}

void BattleField::update(float delta){
        this->sendKeepAliveData();
}

void BattleField::onExit(){
        Layer::onExit();
}


#pragma mark - websocket delegate method
void BattleField::sendKeepAliveData(){
        
        //5s * 60 frame/s
        static int counter = 0;
        if (++counter % 300 != 0){
                return;
        }
        
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        
        rapidjson::Value data(rapidjson::kObjectType);
        std::string fb_uid = UserSessionBean::getInstance()->getUserId();
        rapidjson::Value uid;
        uid.SetString(fb_uid.c_str(), (rapidjson::SizeType)fb_uid.length());
        data.AddMember("fb_uid", uid, allocator);
        
        rapidjson::Value s_id;
        s_id.SetString(_onlineMapData->getServerId().c_str(),
                       (rapidjson::SizeType)_onlineMapData->getServerId().length());
        data.AddMember("server_id", uid, allocator);
        
        document.AddMember("data", uid, allocator);
        document.AddMember("type", WEB_SOCKET_MSG_TYPE_KA, allocator);
        
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        
        this->sendMessage(buffer.GetString());
}


int  BattleField::sendMessage(std::string msg){
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

void BattleField::onOpen(cocos2d::network::WebSocket* ws){
        log("Websocket (%p) opened", ws);
}

void BattleField::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data){
        std::string msg(data.bytes);
        log("-----------%s---------------", msg.c_str());
        
        rapidjson::Document msg_d;
        msg_d.Parse<0>(msg.c_str());
        if (msg_d.HasParseError()) {
                CCLOG("GetParseError %u\n",msg_d.GetParseError());
        }
}

void BattleField::onClose(network::WebSocket* ws){
        log("websocket instance (%p) closed.", ws);
        
        _wsiSendText = nullptr;
        ModalLayer::dismissDialog(this);
        CC_SAFE_DELETE(ws);
}

void BattleField::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error){
        ModalLayer::dismissDialog(this);
        log("Error was fired, error code: %d", static_cast<int>(error));
}


