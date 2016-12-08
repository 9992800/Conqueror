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
        
//        this->initPageViews(visibleSize, center); 

        return true;
}


void FindPlayer::menuExit(Ref* pSender){
        Director::getInstance()->popScene();
}

void FindPlayer::menuRefresh(Ref* pSender){
//        this->getBattleListFromServer();
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

//void FindPlayer::getBattleListFromServer(){
//        std::string base_url(GAME_SERVICE_SERVER_URL"/battleFields?");
//        std::string uid = UserSessionBean::getInstance()->getUserId();
//        if (uid.length() == 0){
//                return;
//        }
//        
//        base_url.append("user_id=");
//        base_url.append(uid);
//        base_url.append("&curPgaeNo=");
//        base_url.append(tostr(_curPgaeNo));
//        
//        HttpRequest* request = new (std::nothrow) HttpRequest();
//        request->setUrl(base_url);
//        request->setRequestType(HttpRequest::Type::GET);
//        request->setResponseCallback(CC_CALLBACK_2(FindPlayer::onHttpRequestCompleted, this));
//        request->setTag(LIST_ALL_BATTLES);
//        HttpClient::getInstance()->sendImmediate(request);
//        request->release();
//}

#pragma mark - update
void FindPlayer::onEnter(){
        Layer::onEnter();
        _loadingCount = 0;
//        _curPgaeNo = 0;
//        _battlList = std::vector<BattleFieldBean*>();
//        this->getBattleListFromServer();
        
        
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

//void FindPlayer::parseBattleFieldBeans(picojson::value& data){
//        
//        _battlList.clear();
//        picojson::array& array = data.get<picojson::array>();
//        for (picojson::array::iterator it = array.begin(); it != array.end(); it++){
//                picojson::object& tmpObject = it->get<picojson::object>();
//                auto BattleFieldBean = BattleFieldBean::create(tmpObject);
//                BattleFieldBean->retain();
//                _battlList.insert(_battlList.begin(), BattleFieldBean);
//        }
//        this->reloadPageData();
//}

//#pragma mark - page view actions
//void FindPlayer::initPageViews(Size visibleSize, Vec2 center){
//        
//        _batllePageViews = PageView::create();
//        _batllePageViews->setContentSize(visibleSize * 0.85);
//        _batllePageViews->setIgnoreAnchorPointForPosition(false);
//        _batllePageViews->setAnchorPoint(Vec2(0.5, 0.5));
//        _batllePageViews->setPosition(center);
//        _batllePageViews->removeAllItems();
//        _batllePageViews->addEventListener((PageView::ccPageViewCallback)CC_CALLBACK_2(FindPlayer::pageViewEvent, this));
//        
//        _batllePageViews->setIndicatorEnabled(true);
//        _batllePageViews->setIndicatorSpaceBetweenIndexNodes(5);
//        _batllePageViews->setIndicatorIndexNodesScale(0.5);
//        _batllePageViews->setIndicatorIndexNodesTexture("green_edit.png");
//        _batllePageViews->setIndicatorIndexNodesColor(Color3B::BLACK);
//        
//        this->addChild(_batllePageViews);
//}

//#define BATTLE_ROWS  2
//#define BATTLE_COLUM 3
//
//void FindPlayer::reloadPageData(){
//        if (!_batllePageViews){
//                return;
//        }
//        
//        _batllePageViews->removeAllPages();
//        
//        int total_cnt = (int)_battlList.size();
//        int battle_idx = 0;
//        int pageCount =  total_cnt / (BATTLE_ROWS * BATTLE_COLUM) + 1;
//        auto size = _batllePageViews->getContentSize();
//        for (int i = 0; i < pageCount; ++i) {
//                HBox* outerBox = HBox::create();
//                outerBox->setContentSize(size);
//                
//                for (int k = 0; k < BATTLE_COLUM && total_cnt > battle_idx; ++k) {
//                        VBox* innerBox = VBox::create();
//                        
//                        for (int j = 0; j < BATTLE_ROWS && total_cnt > battle_idx; j++) {
//                                BattleFieldBean* bean = _battlList.at(battle_idx++);
//                                
//                                Button *btn = Button::create("battle_field.png","");
//                                btn->setTitleText(StringUtils::format("button %d", battle_idx));
//                                btn->setTitleColor(Color3B::RED);
//                                btn->setTitleFontSize(24);
//                                btn->addTouchEventListener( CC_CALLBACK_2(FindPlayer::onBattleSelected, this));
//                                
//                                std::set<std::string> players = bean->getCurrentPlayers();
//                                for (std::set<std::string>::iterator it = players.begin();
//                                     it != players.end(); it++) {
//                                        
//                                        sdkbox::FBGraphUser player = UserSessionBean::getInstance()->getPlayerInfo(it->c_str());
//                                        
//                                        auto name = Label::createWithSystemFont(player.getName(), "", 24);
//                                        name->setColor(Color3B::RED);
//                                        auto btn_size = btn->getContentSize();
//                                        name->setPosition(Vec2(btn_size/2));
//                                        btn->addChild(name, 1, 111);
//                                        
//                                        
//                                        auto avatar = SpriteEx::createWithUrl(player.getPictureURL());
//                                        avatar->setPosition(Vec2(btn_size/2) - Vec2(0, 50));
//                                        btn->addChild(avatar, 2, 112);
//                                }
//                                innerBox->addChild(btn);
//                        }
//                        
//                        LinearLayoutParameter *parameter = LinearLayoutParameter::create();
//                        parameter->setMargin(Margin(size.width/8, 5,size.width/8, 5));
//                        innerBox->setLayoutParameter(parameter);
//                        
//                        outerBox->addChild(innerBox);
//                }
//                _batllePageViews->insertCustomItem(outerBox, i);
//        }
//}
//
//void FindPlayer::pageViewEvent(cocos2d::Ref* sender, cocos2d::ui::PageView::EventType type){
//        CCLOG("---pageViewEvent:%d---", type);
//}
//void FindPlayer::onBattleSelected(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
//        CCLOG("---onBattleSelected:%d---", type);
//}



//#pragma mark - table view delegate , datasource
//
//void FindPlayer::tableCellTouched(TableView* table, TableViewCell* cell)
//{
//        CCLOG("cell touched at index: %ld", static_cast<long>(cell->getIdx()));
//}
//
//Size FindPlayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
//{
//        if (idx == 2) {
//                return Size(200, 200);
//        }
//        return Size(200, 200);
//}
//
//TableViewCell* FindPlayer::tableCellAtIndex(TableView *table, ssize_t idx)
//{
//        auto string = StringUtils::format("%ld", static_cast<long>(idx));
//        TableViewCell *cell = table->dequeueCell();
//        if (!cell) {
//                cell = new (std::nothrow) BattleFieldCell(table->cocos2d::Node::getContentSize());
//                cell->autorelease();
//                auto label = (Label*)cell->getChildByTag(123);
//                label->setString(string);
//        }
//        else {
//                auto label = (Label*)cell->getChildByTag(123);
//                label->setString(string);
//        }
//        
//        
//        return cell;
//}
//
//ssize_t FindPlayer::numberOfCellsInTableView(TableView *table)
//{
//        return 20;
//}
//
//void FindPlayer::initTableView(Size visibleSize, Vec2 center){
//        _battleTableView = TableView::create(this, visibleSize * 0.75);
//        _battleTableView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
//        _battleTableView->setIgnoreAnchorPointForPosition(false);
//        _battleTableView->setAnchorPoint(Vec2(0.5, 0.5));
//        _battleTableView->setPosition(center);
//        _battleTableView->setDelegate(this);
//        
//        auto bg = LayerColor::create(Color4B::RED);
//        bg->setIgnoreAnchorPointForPosition(false);
//        bg->setAnchorPoint(Vec2(0.5, 0.5));
//        bg->setPosition(center);
//        _battleTableView->addChild(bg);
//        
//        
//        this->addChild(_battleTableView);
//        _battleTableView->reloadData();
//}
//
//#pragma mark - battle field table view cell
//
//void BattleFieldCell::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
//{
//        TableViewCell::draw(renderer, transform, flags);
//        
//// 	auto pos = getPosition();
//// 	auto size = Size(178, 200);
//// 	Vec2 vertices[4]={
//// 		Vec2(pos.x+1, pos.y+1),
//// 		Vec2(pos.x+size.width-1, pos.y+1),
//// 		Vec2(pos.x+size.width-1, pos.y+size.height-1),
//// 		Vec2(pos.x+1, pos.y+size.height-1),
//// 	};
//// 	DrawPrimitives::drawPoly(vertices, 4, true);
//}
//
//BattleFieldCell::BattleFieldCell(Size size){ 
//        
//        auto sprite = Sprite::create("HelloWorld.png");
//        sprite->setAnchorPoint(Vec2::ZERO);
//        sprite->setPosition(Vec2(size/2));
//        this->addChild(sprite);
//        
//        auto label = Label::createWithSystemFont("s", "Helvetica", 20.0);
//        label->setPosition(Vec2::ZERO);
//        label->setAnchorPoint(Vec2::ZERO);
//        label->setTag(123);
//        this->addChild(label);
//}

#pragma mark - websocket delegate
void FindPlayer::onOpen(cocos2d::network::WebSocket* ws){
        log("Websocket (%p) opened", ws);
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
        
        _waitingQueue = nullptr;
        ModalLayer::dismissDialog(this);
        CC_SAFE_DELETE(ws);
}

void FindPlayer::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error){
        ModalLayer::dismissDialog(this);
        log("Error was fired, error code: %d", static_cast<int>(error));
}

