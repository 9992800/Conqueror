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
#include "FindPlayerScene.hpp"
#include "ui/UIScale9Sprite.h"

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
        
        auto back_ground = Sprite::create("level/level_select_back.png");
        back_ground->setPosition(center);
        
        auto chengqiang = Sprite::create("level/chengqiang.png");
        auto size = chengqiang->getContentSize();
        chengqiang->setPosition(Vec2(size / 2));
        
        back_ground->addChild(chengqiang);
        this->addChild(back_ground, ZORDER_BACK_GROUND);
        
        this->initButtons(origin, visibleSize);
        
        this->initMenuSelections();
        
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

void LevelSelect::initMenuSelections(){
        auto character_back = Sprite::create("level/character_sel_back.png");
        auto size = character_back->getContentSize();
        auto visible_size = Director::getInstance()->getVisibleSize();
        auto position = Vec2(size.width, visible_size.height / 2);
        character_back->setPosition(position);
        this->addChild(character_back, ZORDER_BACK_LAYERS);
        
        _characterToChoose = std::vector<Sprite*>();
        _curChIdx = 0;
        for (int i = 0; i < 2; i++){
                std::string name = StringUtils::format("level/ch_player_%d.png", i);
                auto ch = Sprite::create(name);
                _characterToChoose.push_back(ch);
                ch->setPosition(Vec2(size / 2));
                ch->setVisible(false);
                character_back->addChild(ch);
        }
        _characterToChoose[_curChIdx]->setVisible(true);
        
        auto btn_up = MenuItemImage::create("level/arrow_up.png", "level/arrow_up.png",
                                            CC_CALLBACK_1(LevelSelect::menuCharactorUpDown, this, 0));
        btn_up->setPosition(Vec2(size.width / 2,
                                 size.height + btn_up->getContentSize().height / 2 + 5));
        
        auto btn_down = MenuItemImage::create("level/arrow_down.png", "level/arrow_down.png",
                                            CC_CALLBACK_1(LevelSelect::menuCharactorUpDown, this, 1));
        
        btn_down->setPosition(Vec2(size.width / 2,
                                 - btn_up->getContentSize().height / 2 - 5));
        
        auto menu = Menu::create(btn_up, btn_down, NULL);
        menu->setPosition(Vec2::ZERO);
        character_back->addChild(menu, ZORDER_ITEM_CONTROL);
        
        
        auto chose_num_back = Sprite::create("level/player_num_sel_back.png");
        auto num_size = chose_num_back->getContentSize();
        auto position_num = Vec2(position.x + num_size.width / 2 + 5 + size.width / 2,
                              position.y);
        chose_num_back->setPosition(position_num);
        this->addChild(chose_num_back, ZORDER_BACK_LAYERS);
        
        auto soldier_back = Sprite::create("level/soldier_back.png");
        soldier_back->setPosition(Vec2(num_size.width / 2,
                                         (num_size.height - soldier_back->getContentSize().height) / 2 + num_size.height / 2));
        chose_num_back->addChild(soldier_back);
        
        _levelPlayerNUm = 2;
        
        _num_sel_back_grd = Sprite::create("level/sel_num_btn_back.png");
        auto btn_size = _num_sel_back_grd->getContentSize();
        Size gap(btn_size.width / 3, btn_size.height / 4);
        btn_size = btn_size * 0.9;
        auto pos_1 = Vec2(btn_size / 2 + gap);
        
        for (int i = 2; i <= 8; i++){
                auto sel_num_2 = Button::create("");
                sel_num_2->setContentSize(btn_size);
                sel_num_2->setPosition(Vec2(pos_1.x + (i - 2) * btn_size.width, pos_1.y));
                std::string str = StringUtils::format("%d", i);
                sel_num_2->setTitleText(str);
                sel_num_2->setTitleFontSize(36);
                sel_num_2->addClickEventListener(CC_CALLBACK_1(LevelSelect::btnChosePlayerNum, this, i));
                chose_num_back->addChild(sel_num_2);
        }
        
        
        _num_sel_back_grd->setPosition(pos_1);
        chose_num_back->addChild(_num_sel_back_grd);
        
        
        auto chose_color_back = Sprite::create("level/chose_color_bck.png");
        chose_color_back->setPosition(Vec2(position_num.x +
                                           num_size.width / 2 + 5
                                           + chose_color_back->getContentSize().width / 2,
                                           position_num.y));
        this->addChild(chose_color_back, ZORDER_BACK_LAYERS);
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
        
        auto start_game = MenuItemImage::create("level/start_game.png","level/start_game_sel.png", CC_CALLBACK_1(LevelSelect::menuStartGame, this));
        start_game->setPosition(Vec2(visibleSize.width / 2,
                                     start_game->getContentSize().height));
        
        auto online_game = MenuItemImage::create("online_battle.png", "online_battle_sel.png",
                              CC_CALLBACK_1(LevelSelect::menuOnlineBattle, this));
        online_game->setPosition(Vec2(visibleSize.width -  online_game->getContentSize().width / 2,
                                        visibleSize.height/ 2 ));
        
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
                                 add_coins, dices_show, add_dices, start_game, online_game, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, ZORDER_ITEM_CONTROL);
}

#pragma mark - button action callback

void LevelSelect::btnChosePlayerNum(Ref* btn, int num){
        Button* sel_btn = ((Button*)btn);
        _levelPlayerNUm = num;
        _num_sel_back_grd->setPosition(sel_btn->getPosition());
}

void LevelSelect::menuCharactorUpDown(Ref* btn, int dir){
        if (dir == 0){
//                _cur
        }
} 

void LevelSelect::menuOnlineBattle(Ref* btn){
        Scene* scene = FindPlayer::createScene();
        Director::getInstance()->pushScene(scene);
}
void LevelSelect::menuStartGame(Ref* btn){
        
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

