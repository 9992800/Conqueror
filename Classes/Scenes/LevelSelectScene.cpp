//
//  LevelSelecteScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/2.
//
//
#include "SimpleAudioEngine.h"
#include "GameSettingsScene.hpp"
#include "LevelSelectScene.hpp"
#include "GameScene.hpp"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "PlayerSettingScene.hpp"

enum {
        GAME_LEVEL_INDEX_1 = 1,
        GAME_LEVEL_INDEX_2,
        GAME_LEVEL_INDEX_3,
        GAME_LEVEL_INDEX_4,
        GAME_LEVEL_INDEX_5,
        GAME_LEVEL_INDEX_6,
        GAME_LEVEL_INDEX_7,
};

enum{
        kLevelShowLevel1Tag = 2,
        kLevelShowLevel2Tag = 3,
        kLevelShowLevel3Tag = 4,
        kLevelShowLevel4Tag = 5,
        kLevelShowLevel5Tag = 6,
        kLevelShowLevel6Tag = 7,
        kLevelShowLevel7Tag = 8,
        key_loading_bar1,
        key_loading_bar2,
        key_loading_bar3,
        kLevelLockedTag,
};

enum{
        ZORDER_BACK_GROUND = 0,
        ZORDER_BACK_LAYERS,
        ZORDER_SEL_BACK_LAYER,
        ZORDER_ITEM_SHOW,
        ZORDER_ITEM_CONTROL
};


template<typename T>
std::vector<T> parseData(const char* key)
{
        Data data = UserDefault::getInstance()->getDataForKey(key);
        T* buffer = (T*) data.getBytes();
        ssize_t length = data.getSize() / sizeof(T);
        
        std::vector<T> result;
        for (int i = 0; i < length; i++) {
                result.push_back(buffer[i]);
        }
        
        return result;
}

std::vector<std::string> parseData(const char* key){
        Data data = UserDefault::getInstance()->getDataForKey(key);
        std::string::value_type* buffer = (std::string::value_type*) data.getBytes();
        std::vector<std::string> result;
        std::istringstream f(buffer);
        std::string str;
        while (getline(f, str, '\n')){
                result.push_back(str);
        }
        return result;
}

Scene* LevelSelect::createScene()
{
        auto scene = Scene::create();
        auto layer = LevelSelect::create();
        scene->addChild(layer);
        
        return scene;
}

bool LevelSelect::init()
{
        if (!Layer::init()){
                return false;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 center = origin + visibleSize / 2;
        
        auto back_ground = Sprite::create("level_select_back.png");
        back_ground->setPosition(center);
        this->addChild(back_ground, ZORDER_BACK_GROUND);
        
        
        this->loadLevelShow(center, visibleSize);
        this->initButtons(origin, visibleSize);
        
        IAP::setDebug(true);
        IAP::setListener(this);
        IAP::init();
        
        _loadingBar = LoadingBar::create("sliderProgress.png");
        _loadingBar->setDirection(LoadingBar::Direction::LEFT);
        _loadingBar->setPercent(0);
        
        Vec2 pos = Vec2(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height / 6);
        _loadingBar->setPosition(pos);
        
        Size bar_size = _loadingBar->getContentSize();
        auto label = Label::createWithTTF("Loading", "fonts/Marker Felt.ttf", 24);
        label->setPosition(Vec2(pos.x, pos.y + bar_size.height / 2));
        _loadingBar->addChild(label, 4, key_loading_bar2);
        this->addChild(_loadingBar, 3, key_loading_bar1);
        _loadingBar->setVisible(false);
        
        _loadingBarBack = Sprite::create("sliderProgress_back.png");
        _loadingBarBack->setPosition(pos);
        this->addChild(_loadingBarBack, 2, key_loading_bar3);
        _loadingBarBack->setVisible(false);

        
        return true;
}

void LevelSelect::loadLevelShow(Vec2 center, Size visibleSize){
        
        _levelBackTopHalf       = LayerColor::create();
        _levelBackTopHalf->setContentSize(Size(visibleSize.width, visibleSize.height / 2));
        _levelBackTopHalf->setPosition(Vec2(-visibleSize.width, visibleSize.height / 2));
        
        _levelBackBottomHalf    = LayerColor::create();
        _levelBackBottomHalf->setContentSize(Size(visibleSize.width, visibleSize.height / 2));
        _levelBackBottomHalf->setPosition(Vec2(visibleSize.width, 0));
        
        auto level_back_1 = Button::create("level/players_back.png");        
        level_back_1->addClickEventListener(CC_CALLBACK_1(LevelSelect::menuStartGame, this));
        
        Size leve1_size = level_back_1->getContentSize();
        auto leve1_1 = Sprite::create("level/2player.png");
        leve1_1->setPosition(leve1_size / 2);
        level_back_1->addChild(leve1_1);
        level_back_1->setPosition(Vec2(visibleSize.width * 2 / 13, visibleSize.height * 2/ 11));
        _levelBackTopHalf->addChild(level_back_1, ZORDER_ITEM_SHOW, kLevelShowLevel1Tag);
        level_back_1->setTag(2);
        
        
        auto level_back_2 = (Button*)level_back_1->clone();
        auto leve1_2 = Sprite::create("level/3player.png");
        leve1_2->setPosition(leve1_size / 2);
        bool level_2_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_2_LOCK_STATE_KEY, false);
        if (false == level_2_opened){
                auto lock = Sprite::create("level/level_lock.png");
                lock->setPosition(leve1_size / 2);
                level_back_2->addChild(lock, 100, kLevelLockedTag);
        }
        level_back_2->addChild(leve1_2);
        level_back_2->setPosition(Vec2(visibleSize.width * 5 / 13, visibleSize.height * 2/ 11));
        _levelBackTopHalf->addChild(level_back_2, ZORDER_ITEM_SHOW, kLevelShowLevel2Tag);
        level_back_2->setTag(3);
        
        
        
        auto level_back_3 = (Button*)level_back_1->clone();
        auto level_3 = Sprite::create("level/4player.png");
        level_3->setPosition(leve1_size / 2);
        bool level_3_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_3_LOCK_STATE_KEY, false);
        if (false == level_3_opened){
                auto lock = Sprite::create("level/level_lock.png");
                lock->setPosition(leve1_size / 2);
                level_back_3->addChild(lock, 100, kLevelLockedTag);
        }
        level_back_3->addChild(level_3);
        level_back_3->setPosition(Vec2(visibleSize.width * 8 / 13, visibleSize.height * 2/ 11));
        _levelBackTopHalf->addChild(level_back_3, ZORDER_ITEM_SHOW, kLevelShowLevel3Tag);
        level_back_3->setTag(4);
        
        
        auto level_back_4 = (Button*)level_back_1->clone();
        auto level_4 = Sprite::create("level/5player.png");
        level_4->setPosition(leve1_size / 2);
        bool level_4_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_4_LOCK_STATE_KEY, false);
        if (false == level_4_opened){
                auto lock = Sprite::create("level/level_lock.png");
                lock->setPosition(leve1_size / 2);
                level_back_4->addChild(lock, 100, kLevelLockedTag);
        }
        level_back_4->addChild(level_4);
        level_back_4->setPosition(Vec2(visibleSize.width * 11 / 13, visibleSize.height * 2/ 11));
        _levelBackTopHalf->addChild(level_back_4, ZORDER_ITEM_SHOW, kLevelShowLevel4Tag);
        level_back_4->setTag(5);
        
        
        auto level_back_5 = (Button*)level_back_1->clone();
        auto level_5 = Sprite::create("level/6player.png");
        level_5->setPosition(leve1_size / 2);
        bool level_5_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_5_LOCK_STATE_KEY, false);
        if (false == level_5_opened){
                auto lock = Sprite::create("level/level_lock.png");
                lock->setPosition(leve1_size / 2);
                level_back_5->addChild(lock, 100, kLevelLockedTag);
        }
        level_back_5->addChild(level_5);
        level_back_5->setPosition(Vec2(visibleSize.width * 2 / 13, visibleSize.height * 4 / 11));
        _levelBackBottomHalf->addChild(level_back_5, ZORDER_ITEM_SHOW, kLevelShowLevel5Tag);
        level_back_5->setTag(6);
        
        
        auto level_back_6 = (Button*)level_back_1->clone();
        auto level_6 = Sprite::create("level/7player.png");
        level_6->setPosition(leve1_size / 2);
        bool level_6_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_6_LOCK_STATE_KEY, false);
        if (false == level_6_opened){
                auto lock = Sprite::create("level/level_lock.png");
                lock->setPosition(leve1_size / 2);
                level_back_6->addChild(lock, 100, kLevelLockedTag);
        }
        level_back_6->addChild(level_6);
        level_back_6->setPosition(Vec2(visibleSize.width * 5 / 13, visibleSize.height * 4 / 11));
        _levelBackBottomHalf->addChild(level_back_6, ZORDER_ITEM_SHOW, kLevelShowLevel6Tag);
        level_back_6->setTag(7);
        
        
        auto level_back_7 = (Button*)level_back_1->clone();
        auto level_7 = Sprite::create("level/8player.png");
        level_7->setPosition(leve1_size / 2);
        bool level_7_opened = UserDefault::getInstance()->getBoolForKey(LEVEL_7_LOCK_STATE_KEY, false);
        if (false == level_7_opened){
                auto lock = Sprite::create("level/level_lock.png");
                lock->setPosition(leve1_size / 2);
                level_back_7->addChild(lock, 100, kLevelLockedTag);
        }
        level_back_7->addChild(level_7);
        level_back_7->setPosition(Vec2(visibleSize.width * 8 / 13, visibleSize.height * 4 / 11));
        _levelBackBottomHalf->addChild(level_back_7, ZORDER_ITEM_SHOW, kLevelShowLevel7Tag);
        level_back_7->setTag(8);
        
        this->addChild(_levelBackTopHalf, ZORDER_BACK_LAYERS);
        this->addChild(_levelBackBottomHalf, ZORDER_BACK_LAYERS);
}

void LevelSelect::initButtons(Vec2 origin, Size visibleSize){
        
        bool is_effect_on = UserDefault::getInstance()->getBoolForKey(SOUND_EFFECT_SWITCH_KEY, true);
        if (is_effect_on){
                _soundCtrl = MenuItemImage::create("Sound_on.png", "Sound_on_sel.png",
                                                   CC_CALLBACK_1(LevelSelect::menuSoundControl, this));
        }else{
                _soundCtrl = MenuItemImage::create("Sound_off.png", "Sound_off_sel.png",
                                                   CC_CALLBACK_1(LevelSelect::menuSoundControl, this));
        }
        _soundCtrl->setPosition(Vec2(2 * _soundCtrl->getContentSize().width,
                                     origin.y + 60));
        
        auto system_setting = MenuItemImage::create("level/game_setting.png", "level/game_setting_sel.png",
                                                  CC_CALLBACK_1(LevelSelect::menuShowSettigns, this));
        system_setting->setPosition(Vec2(origin.x + visibleSize.width -  system_setting->getContentSize().width,
                                     origin.y + 60));
        
        
        
        _historyPlayItem = MenuItemImage::create("history.png", "history_sel.png",
                                                 CC_CALLBACK_1(LevelSelect::menuPlayHistory, this));
        _historyPlayItem->setPosition(Vec2(2 * _historyPlayItem->getContentSize().width,
                                           origin.y + visibleSize.height - _historyPlayItem->getContentSize().height - 10));
        
        /*
         /////////////////////////////////////////////////////////
                        coins buttons
         /////////////////////////////////////////////////////////
         */
        auto coins_show = MenuItemImage::create("level/coins_show.png", "");
        Vec2 coins_pos = Vec2(origin.x + visibleSize.width - 2 * coins_show->getContentSize().width,
                              origin.y + visibleSize.height - coins_show->getContentSize().height);
        coins_show->setPosition(coins_pos);
        
        auto coins_back = Sprite::create("level/coind_back.png");
        Vec2 coins_back_pos = Vec2(coins_pos.x - 60, coins_pos.y);
        coins_back->setPosition(coins_back_pos);
        _coinsNum = 100;//查询消费记录 apple
        auto coins_label = Label::createWithSystemFont(tostr(_coinsNum), "fonts/Marker Felt.ttf", 24);//
        coins_back->addChild(coins_label);
        coins_label->setPosition(coins_back->getContentSize() / 2);
        this->addChild(coins_back, ZORDER_BACK_LAYERS);
        
        
        auto add_coins = MenuItemImage::create("level/add_icon.png", "",CC_CALLBACK_1(LevelSelect::menuGetMoreCoins, this));
        Vec2 add_coins_pos = Vec2(coins_back_pos.x - 100, coins_pos.y);
        add_coins->setPosition(add_coins_pos);
        
        
        /*
         /////////////////////////////////////////////////////////
                        dices buttons
         /////////////////////////////////////////////////////////
         */
        auto dices_show = MenuItemImage::create("level/dice_show.png", "");
        Vec2 dices_pos = Vec2(add_coins_pos.x - 100, coins_pos.y);
        dices_show->setPosition(dices_pos);
        
        auto dices_back = Sprite::create("level/coind_back.png");
        Vec2 dices_back_pos = Vec2(dices_pos.x - 60, dices_pos.y);
        dices_back->setPosition(dices_back_pos);
        _dicesNum = 100;//查询消费记录 apple
        auto dices_label = Label::createWithSystemFont(tostr(_coinsNum), "fonts/Marker Felt.ttf", 24);//
        dices_back->addChild(dices_label);
        dices_label->setPosition(dices_back->getContentSize() / 2);
        this->addChild(dices_back, ZORDER_BACK_LAYERS);
        
        auto add_dices = MenuItemImage::create("level/add_icon.png", "", CC_CALLBACK_1(LevelSelect::menuGetMoreDices, this));
        Vec2 add_dices_pos(dices_back_pos.x - 100, dices_pos.y);
        add_dices->setPosition(add_dices_pos);
        
        auto menu = Menu::create(_soundCtrl, system_setting, coins_show,
                                 add_coins, dices_show, add_dices, _historyPlayItem, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, ZORDER_ITEM_CONTROL);
}

#pragma mark - button action callback


void LevelSelect::purchaseCallBack(sdkbox::Product& p){
        
}

void LevelSelect::menuStartGame(Ref* btn){
        
        _levelPlayerNUm = ((Node*)btn)->getTag();
        std::string product_name;
        bool need_buy = false;
        switch (_levelPlayerNUm) {
                case 3:
                        need_buy = !UserDefault::getInstance()->getBoolForKey(LEVEL_2_LOCK_STATE_KEY, false);
                        product_name = LEVEL_2_PRODUCT_NAME_KEY;
                        break;
                        
                case 4:
                        need_buy = !UserDefault::getInstance()->getBoolForKey(LEVEL_3_LOCK_STATE_KEY, false);
                        product_name = LEVEL_3_PRODUCT_NAME_KEY;
                        break;
                case 5:
                        need_buy = !UserDefault::getInstance()->getBoolForKey(LEVEL_4_LOCK_STATE_KEY, false);
                        product_name = LEVEL_4_PRODUCT_NAME_KEY;
                        break;
                case 6:
                        need_buy = !UserDefault::getInstance()->getBoolForKey(LEVEL_5_LOCK_STATE_KEY, false);
                        product_name = LEVEL_5_PRODUCT_NAME_KEY;
                        break;
                case 7:
                        need_buy = !UserDefault::getInstance()->getBoolForKey(LEVEL_6_LOCK_STATE_KEY, false);
                        product_name = LEVEL_6_PRODUCT_NAME_KEY;
                        break;
                case 8:
                        need_buy = !UserDefault::getInstance()->getBoolForKey(LEVEL_7_LOCK_STATE_KEY, false);
                        product_name = LEVEL_7_PRODUCT_NAME_KEY;
                        break;
                default:
                        need_buy = false;
                        break;
        }
        
        if (need_buy){
                IAP::purchase(product_name);
                return;
        }
        
        auto scene = GameScene::createScene(_levelPlayerNUm);
        Director::getInstance()->pushScene(scene);
}

void LevelSelect::menuShowSettigns(Ref* btn){
        auto scene = GameSettings::createScene();
        Director::getInstance()->pushScene(scene);
}


void LevelSelect::menuGetMoreCoins(Ref* btn){
        IAP::restore();
}
void LevelSelect::menuGetMoreDices(Ref* btn){
        
}

void LevelSelect::loadResourceInBg(int* loader, HistoryReplayData* data){
        
        GameData* game_data = data->gameData;
        std::vector<int>  map_data = parseData<int>(GAME_HISTORY_MAP_KEY);
        game_data->_mapData = map_data;
        
        *loader += 10;
        std::vector<int>  cell_data = parseData<int>(GAME_HISTORY_CELL_INFO);
        game_data->_cel = cell_data;
        
        *loader += 10;
        
        std::vector<std::string> area_data = parseData(GAME_HISTORY_AREA_INFO);
        
        for (int i = 0; i < AREA_MAX; i++){
                std::string area_str = area_data[i];
                
                rapidjson::Document area_d;
                area_d.Parse<0>(area_str.c_str());
                if (area_d.HasParseError()) {
                        CCLOG("GetParseError %u\n",area_d.GetParseError());
                }
                
                AreaData* area = new AreaData(i);
                
                const rapidjson::Value& basic = area_d["basic"];
                area->_arm      = basic["_arm"].GetInt();
                area->_size     = basic["_size"].GetInt();
                area->_cpos     = basic["_cpos"].GetInt();
                area->_dice     = basic["_dice"].GetInt();
                area->_left     = basic["_left"].GetInt();
                area->_right    = basic["_right"].GetInt();
                area->_top      = basic["_top"].GetInt();
                area->_bottom   = basic["_bottom"].GetInt();
                area->_cx       = basic["_cx"].GetInt();
                area->_cy       = basic["_cy"].GetInt();
                area->_len_min  = basic["_len_min"].GetInt();
                area->_areaId   = basic["_areaId"].GetInt();
                
                
                const rapidjson::Value& _line_cel = area_d["_line_cel"];
                for (int j = 0; j < _line_cel.Size(); j++){
                        int cel = _line_cel[(rapidjson::SizeType)j].GetInt();
                        area->_line_cel[j] = cel;
                }
                
                const rapidjson::Value& _line_dir = area_d["_line_dir"];
                for (int j = 0; j < _line_dir.Size(); j++){
                        int dir = _line_dir[j].GetInt();
                        area->_line_dir[j] = dir;
                }
                
                const rapidjson::Value& _cell_idxs = area_d["_cell_idxs"];
                for (int j = 0; j < _cell_idxs.Size(); j++){
                        int cel = _cell_idxs[j].GetInt();
                        area->_cell_idxs.insert(cel);
                }
                
                game_data->_areaData[i] = area;
                *loader += 2;
        }
        
        
        data->from = parseData<int>(GAME_HISTORY_FROM_KEY);
        *loader += 10;
        
        data->to   = parseData<int>(GAME_HISTORY_TO_KEY);
        *loader += 10;
        
        data->res  = parseData<int>(GAME_HISTORY_RES_KEY);
        *loader += 10;
        
        data->sup  = parseData<Vec2>(GAME_HISTORY_SUPPLY_KEY);
        *loader += 10;
}


void LevelSelect::afterParseArea(void* param){
        _count = 100;
        auto scene = ReplayLast::createScene(_historyData);
        Director::getInstance()->pushScene(scene);
}

void LevelSelect::menuPlayHistory(Ref* pSender){
        int player_num = UserDefault::getInstance()->getIntegerForKey(GAME_HISTORY_PLAYER_NUM);
        
        if (0 == player_num){
                return;
        }
        scheduleUpdate();
        _loadingBar->setVisible(true);
        _loadingBarBack->setVisible(true);
        std::function<void(void*)> callback_area = CC_CALLBACK_1(LevelSelect::afterParseArea, this);
        GameData* gameData = GameData::create(player_num);
        gameData->retain();
        
        _historyData.gameData = gameData;
        HistoryReplayData* data_ptr = &_historyData;
        _count = 0;
        int *loader = &_count;
        AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_IO, callback_area, nullptr, [loader, data_ptr, this](){
                this->loadResourceInBg(loader, data_ptr);
        });
}


void LevelSelect::menuSoundControl(Ref* btn){
        bool is_effect_on = UserDefault::getInstance()->getBoolForKey(SOUND_EFFECT_SWITCH_KEY, true);
        UserDefault::getInstance()->setBoolForKey(SOUND_EFFECT_SWITCH_KEY, !is_effect_on);
        
        if (is_effect_on){
                _soundCtrl->setNormalImage(Sprite::create("Sound_off.png"));
                _soundCtrl->setSelectedImage(Sprite::create("Sound_off_sel.png"));
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
        }else{
                _soundCtrl->setNormalImage(Sprite::create("Sound_on.png"));
                _soundCtrl->setSelectedImage(Sprite::create("Sound_on_sel.png"));
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE_START_GAME);
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);
        }
}

#pragma mark - loading bar

void LevelSelect::onEnter(){
        Layer::onEnter();
        IAP::refresh();
        
        Data data = UserDefault::getInstance()->getDataForKey(GAME_HISTORY_FROM_KEY);
        Rect c(Vec2(0,0), _historyPlayItem->getContentSize());
        if (data.isNull()){
                _historyPlayItem->setEnabled(false);
                _historyPlayItem->setNormalSpriteFrame(SpriteFrame::create("history.png", c));
                _historyPlayItem->setSelectedSpriteFrame(SpriteFrame::create("history_sel.png", c));
        }else{
                _historyPlayItem->setEnabled(true);
                _historyPlayItem->setNormalSpriteFrame(SpriteFrame::create("history_data.png", c));
                _historyPlayItem->setNormalSpriteFrame(SpriteFrame::create("history_data_sel.png", c));
        }
        
        auto visible_size = Director::getInstance()->getVisibleSize();
        auto actionTo = MoveTo::create(1, Vec2(0,visible_size.height / 2));
        _levelBackTopHalf->runAction(actionTo);
        
        auto actionTo2 = MoveTo::create(1, Vec2::ZERO);
        _levelBackBottomHalf->runAction(actionTo2);
}

void LevelSelect::update(float delta){
        
        if (_count < 100)
                _loadingBar->setPercent(_count);
}

void LevelSelect::onExit(){
        Layer::onExit();
        _loadingBar->setVisible(false);
        _loadingBarBack->setVisible(false);
        _count = 0;
        unscheduleUpdate();
        AsyncTaskPool::getInstance()->stopTasks(AsyncTaskPool::TaskType::TASK_IO);
}


#pragma mark - payment callback
void LevelSelect::onSuccess(const Product& p){
        auto user_default = UserDefault::getInstance();
        if (LEVEL_2_PRODUCT_NAME_KEY == p.name){
                user_default->setBoolForKey(LEVEL_2_LOCK_STATE_KEY, true);
                auto button = _levelBackTopHalf->getChildByTag(kLevelShowLevel2Tag);
                button->removeChildByTag(kLevelLockedTag);
                
        }else if(LEVEL_3_PRODUCT_NAME_KEY == p.name){
                user_default->setBoolForKey(LEVEL_3_LOCK_STATE_KEY, true);
                auto button = _levelBackTopHalf->getChildByTag(kLevelShowLevel3Tag);
                button->removeChildByTag(kLevelLockedTag);
                
        }else if(LEVEL_4_PRODUCT_NAME_KEY == p.name){
                user_default->setBoolForKey(LEVEL_4_LOCK_STATE_KEY, true);
                auto button = _levelBackTopHalf->getChildByTag(kLevelShowLevel4Tag);
                button->removeChildByTag(kLevelLockedTag);
                
        }else if(LEVEL_5_PRODUCT_NAME_KEY == p.name){
                user_default->setBoolForKey(LEVEL_5_LOCK_STATE_KEY, true);
                auto button = _levelBackTopHalf->getChildByTag(kLevelShowLevel5Tag);
                button->removeChildByTag(kLevelLockedTag);
                
        }else if(LEVEL_6_PRODUCT_NAME_KEY == p.name){
                user_default->setBoolForKey(LEVEL_6_LOCK_STATE_KEY, true);
                auto button = _levelBackTopHalf->getChildByTag(kLevelShowLevel6Tag);
                button->removeChildByTag(kLevelLockedTag);
                
        }else if(LEVEL_7_PRODUCT_NAME_KEY == p.name){
                user_default->setBoolForKey(LEVEL_7_LOCK_STATE_KEY, true);
                auto button = _levelBackTopHalf->getChildByTag(kLevelShowLevel7Tag);
                button->removeChildByTag(kLevelLockedTag);
                
        }else if (REMOVE_ADS_PRODUCT_NAME_KEY == p.name){
                
                UserDefault::getInstance()->setBoolForKey(ADS_LOCK_STATE_KEY, false);
        }
        
        UserDefault::getInstance()->flush();       
}
void LevelSelect::onFailure(const Product& p, const std::string& msg){
         CCLOG("Purchase Failed: %s", msg.c_str());
}
void LevelSelect::onCanceled(const Product& p){
        CCLOG("Purchase Canceled: %s", p.id.c_str());
}

void LevelSelect::onProductRequestSuccess(const std::vector<Product>& products){
        _productsMap.clear();
        for (int i=0; i < products.size(); i++){
                CCLOG("IAP: ========= IAP Item =========");
                CCLOG("IAP: Name: %s", products[i].name.c_str());
                CCLOG("IAP: ID: %s", products[i].id.c_str());
                CCLOG("IAP: Title: %s", products[i].title.c_str());
                CCLOG("IAP: Desc: %s", products[i].description.c_str());
                CCLOG("IAP: Price: %s", products[i].price.c_str());
                CCLOG("IAP: Price Value: %f", products[i].priceValue);
                _productsMap.insert(pair<string, Product>(products[i].name, products[i]));
        }
}

void LevelSelect::onProductRequestFailure(const std::string& msg){
        CCLOG("onProductRequestFailure Restored: %s", msg.c_str());
}
void LevelSelect::onRestoreComplete(bool ok, const std::string &msg){
        CCLOG("%s:%d:%s", __func__, ok, msg.data());
}

