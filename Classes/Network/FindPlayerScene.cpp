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
        this->addChild(menu, 9);
        
//        
//        _loadingBar = LoadingBar::create("sliderProgress.png");
//        _loadingBar->setDirection(LoadingBar::Direction::LEFT);
//        _loadingBar->setPercent(0);
//        
//        Vec2 pos = Vec2(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height / 6);
//        _loadingBar->setPosition(pos);
//        this->addChild(_loadingBar, 10);
        
        TableView* tableView = TableView::create(this, Size(250, 150));
        tableView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
        tableView->setPosition(Vec2(20,winSize.height/2-30));
        tableView->setDelegate(this);
        this->addChild(tableView);
        tableView->reloadData();
        
        auto testNode = Node::create();
        testNode->setName("testNode");
        tableView->addChild(testNode);
        tableView->removeChild(testNode, true);
        CCAssert(nullptr == tableView->getChildByName("testNode"), "The added child has been removed!");
        
        
        tableView = TableView::create(this, Size(150, 250));
        tableView->setDirection(extension::ScrollView::Direction::VERTICAL);
        tableView->setPosition(Vec2(winSize.width-250,winSize.height/2-120));
        tableView->setDelegate(this);
        tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
        this->addChild(tableView);
        tableView->reloadData();

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
        
        
        HttpRequest* request = new (std::nothrow) HttpRequest();
        request->setUrl(GAME_SERVICE_SERVER_URL"/createBattle");
        request->setRequestType(HttpRequest::Type::POST);
        request->setResponseCallback(CC_CALLBACK_2(FindPlayer::onHttpRequestCompleted, this));       
        
        std::string uid = UserSessionBean::getInstance()->getUserId();
        std::string parameters("user_id=");
        parameters.append(uid);
        parameters.append("&cell_data=");
        auto data = OnlineGameData::create();
        parameters.append(data->getMapData());
        request->setRequestData(parameters.c_str(), parameters.length());
        
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
        base_url.append("&curPgaeNo=");
        base_url.append(tostr(_curPgaeNo));
        
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
        _curPgaeNo = 0;
        _battlList = std::vector<BattleFieldBean*>();
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
                CCLOG("---%s---", data.GetString());
        }else if (0 == tags.compare(SEARCHING_OPPENT)){
                
        }else if (0 == tags.compare(LIST_ALL_BATTLES)){
                
                this->parseBattleFieldBeans(data);
                
                
        }else{
                CCLOGWARN("---Unkown request tag:%s---", tags.c_str());
        }
}

void FindPlayer::parseBattleFieldBeans(rapidjson::Value& data){
        for (rapidjson::Value::ConstValueIterator itr = data.Begin();
             itr != data.End(); ++itr){
                auto BattleFieldBean = BattleFieldBean::create(itr);
                _battlList.insert(_battlList.begin(), BattleFieldBean);
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


#pragma mark - table view delegate , datasource

void FindPlayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
        CCLOG("cell touched at index: %ld", static_cast<long>(cell->getIdx()));
}

Size FindPlayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
        if (idx == 2) {
                return Size(200, 200);
        }
        return Size(200, 200);
}

TableViewCell* FindPlayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
        auto string = StringUtils::format("%ld", static_cast<long>(idx));
        TableViewCell *cell = table->dequeueCell();
        if (!cell) {
                cell = new (std::nothrow) BattleFieldCell();
                cell->autorelease();
                auto sprite = Sprite::create("HelloWorld.png");
                sprite->setAnchorPoint(Vec2::ZERO);
                sprite->setPosition(Vec2(0, 0));
                cell->addChild(sprite);
                
                auto label = Label::createWithSystemFont(string, "Helvetica", 20.0);
                label->setPosition(Vec2::ZERO);
                label->setAnchorPoint(Vec2::ZERO);
                label->setTag(123);
                cell->addChild(label);
        }
        else {
                auto label = (Label*)cell->getChildByTag(123);
                label->setString(string);
        }
        
        
        return cell;
}

ssize_t FindPlayer::numberOfCellsInTableView(TableView *table)
{
        return 20;
}


#pragma mark - battle field table view cell

void BattleFieldCell::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
        TableViewCell::draw(renderer, transform, flags);
        // draw bounding box
// 	auto pos = getPosition();
// 	auto size = Size(178, 200);
// 	Vec2 vertices[4]={
// 		Vec2(pos.x+1, pos.y+1),
// 		Vec2(pos.x+size.width-1, pos.y+1),
// 		Vec2(pos.x+size.width-1, pos.y+size.height-1),
// 		Vec2(pos.x+1, pos.y+size.height-1),
// 	};
// 	DrawPrimitives::drawPoly(vertices, 4, true);
}

