//
//  LevelSelecteScene.cpp
//  Conqueror
//
//  Created by wsli on 16/11/2.
//
//
#include "GameSettingsScene.hpp"
#include "LevelSelectScene.hpp"
#include "GameScene.hpp"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "FindPlayerScene.hpp"
#include "ui/UIScale9Sprite.h"
#include "ShoppingScene.hpp"
#include "AchievementScene.hpp"
#include "BuySupplyScene.hpp"
#include "AchievementEngine.hpp"
#include "CommonTipsDialog.hpp"

enum{
        key_loading_bar1,
        key_loading_bar2,
        key_loading_bar3,
        kLevelLockedTag,
        kMenuGreatWallTag,
        kMainMenuBackTag,
        kSoldierBackTag,
        kChoseNumBackTag,
        key_cup_tips,
        key_charactor_back,
        key_charactor_pageview,
        key_charactor_lock
};

enum{
        ZORDER_BACK_GROUND = 0,
        ZORDER_BACK_LAYERS,
        ZORDER_SEL_BACK_LAYER,
        ZORDER_ITEM_SHOW,
        ZORDER_ITEM_CONTROL,
        ZORDER_TOP_LEVEL_SHOW
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

LevelSelect::~LevelSelect(){
}

bool LevelSelect::init()
{
        if (!Layer::init()){
                return false;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        this->initButtons(origin, visibleSize);
        
        this->initMainMenu();         
        
        _loadingBar = LoadingBar::create("sliderProgress.png");
        _loadingBar->setDirection(LoadingBar::Direction::LEFT);
        _loadingBar->setPercent(0);
        
        Vec2 pos = Vec2(visibleSize.width / 2 + origin.x, origin.y + visibleSize.height / 6);
        _loadingBar->setPosition(pos);
        
        Size bar_size = _loadingBar->getContentSize();
        auto label = Label::createWithSystemFont("Loading", "fonts/arial.ttf", 24);
        label->setPosition(Vec2(pos.x, pos.y + bar_size.height / 2));
        _loadingBar->addChild(label, 4, key_loading_bar2);
        this->addChild(_loadingBar, 3, key_loading_bar1);
        _loadingBar->setVisible(false);
        
        _loadingBarBack = Sprite::create("sliderProgress_back.png");
        _loadingBarBack->setPosition(pos);
        this->addChild(_loadingBarBack, 2, key_loading_bar3);
        _loadingBarBack->setVisible(false);
        
        
        this->runAction(Sequence::create(DelayTime::create(2.0f),
                                         CallFunc::create(CC_CALLBACK_0(LevelSelect::showDailyRewards, this)),
                                         nullptr));
        
        return true;
}

Size LevelSelect::initCenterMainFrame(Vec2 position_num){
        //中间的主界面背景
        auto chose_num_back = Sprite::create("level/player_num_sel_back.png");
        auto num_size = chose_num_back->getContentSize();
        chose_num_back->setPosition(position_num);
        this->addChild(chose_num_back, ZORDER_BACK_LAYERS, kChoseNumBackTag);
        
        auto soldier_back = Sprite::create("level/soldier_back.png");
        soldier_back->setPosition(Vec2(num_size.width / 2,
                                       num_size.height - soldier_back->getContentSize().height / 2 - 8));
        chose_num_back->addChild(soldier_back, ZORDER_ITEM_SHOW, kSoldierBackTag);
        
        _levelPlayerNum = 2;
        
        _num_sel_back_grd = Sprite::create("level/sel_num_btn_back.png");
        auto btn_size = _num_sel_back_grd->getContentSize();
        Size gap(btn_size.width / 3, btn_size.height / 6);
        btn_size = btn_size * 0.9;
        auto pos_1 = Vec2(btn_size / 2 + gap);
        
        for (int i = 2; i <= MAX_PLAYER; i++){
                auto sel_num_2 = Button::create("target_lock.png");
                sel_num_2->setPosition(Vec2(pos_1.x + (i - 2) * btn_size.width, pos_1.y));
                std::string str = StringUtils::format("%d", i);
                sel_num_2->setTitleText(str);
                sel_num_2->setTitleFontSize(42);
                sel_num_2->addClickEventListener(CC_CALLBACK_1(LevelSelect::btnChosePlayerNum, this, i));
                chose_num_back->addChild(sel_num_2, ZORDER_ITEM_SHOW, MODAL_DIALOG_NODETAG + i);
        }
        
        auto back_size = soldier_back->getContentSize();
        
                //tips:: default 2 players
        auto empty_chara_0 = ImageView::create("level/ch_player_samll_0.png");
        auto empty_chara_size = empty_chara_0->getContentSize();
        empty_chara_0->setPosition(Vec2(back_size.width / 2 -
                                        0.5 * empty_chara_size.width, empty_chara_size.height * 0.6));
        soldier_back->addChild(empty_chara_0);
        
        auto empty_chara_1 = ImageView::create("level/ch_player_samll_1.png");;
        empty_chara_1->setPosition(Vec2(back_size.width / 2 + 0.5 * empty_chara_size.width,
                                        empty_chara_size.height * 0.6));
        soldier_back->addChild(empty_chara_1);
        
        _num_sel_back_grd->setPosition(pos_1);
        chose_num_back->addChild(_num_sel_back_grd);
        
        auto chose_num_header = Sprite::create("level/sel_num_header.png");
        chose_num_header->setPosition(Vec2(num_size.width / 2,
                                           num_size.height - chose_num_header->getContentSize().height / 2 + 8));
        chose_num_back->addChild(chose_num_header, ZORDER_ITEM_SHOW);
        
        return num_size;
}
void LevelSelect::initCharactorSel(Vec2 position_num, Size num_size) {
        //人物选择
        auto character_back = Sprite::create("level/character_sel_back.png");
        auto size = character_back->getContentSize();
        auto position = Vec2(position_num.x -
                             num_size.width / 2 - 5
                             - character_back->getContentSize().width / 2,
                             position_num.y);
        character_back->setPosition(position);
        this->addChild(character_back, ZORDER_BACK_LAYERS, key_charactor_back);
        
        PageView* pageView = PageView::create();
        pageView->setContentSize(size);
        pageView->setDirection(ui::PageView::Direction::VERTICAL);
        pageView->removeAllItems();
        _curChIdx = 0;
        for (int i = 0; i < MAX_PLAYER; i++){
                std::string name = StringUtils::format("level/ch_player_%d.png", i);
                auto ch = ImageView::create(name);
                ch->setPosition(Vec2(size / 2));
                pageView->insertCustomItem(ch, i);
                bool char_lock_stat =  AchievementEngine::getInstance()->getCharLockStatus(i+1);
                
                auto ch_size = ch->getContentSize();
                int  price =  AchievementEngine::getInstance()->getCharUnlockPrice(i);
                if (false == char_lock_stat && price != 0){
                        auto lock = ui::Button::create("target_lock.png");
                        auto lock_size = lock->getContentSize();
                        lock->setPosition(ch_size * 0.5f);
                        ch->addChild(lock, 2, key_charactor_lock);
                        
                        Vec2 tips_pos(lock_size.width * 0.1f, -ch_size.height * 0.25f);
                        if (price > 0){
                                auto coin = Sprite::create("level/coins_show.png");
                                coin->setPosition(tips_pos);
                                coin->setScale(0.8f);
                                lock->addChild(coin);
                                
                                auto price_lb = Label::createWithSystemFont(StringUtils::format("X%d", price), "fonts/arial.ttf", 24);
                                price_lb->setPosition(Vec2(lock_size.width * 0.9f,
                                                           -ch_size.height * 0.25f));
                                lock->addChild(price_lb);
                        }else{
                                auto tips_lb = Sprite::create("not_for_sale.png");
                                tips_lb->setPosition(tips_pos);
                                lock->addChild(tips_lb);
                        }
                        
                        lock->addClickEventListener(CC_CALLBACK_1(LevelSelect::actionBuyCharacter, this, i, price));
                }
        }
        pageView->setTag(1);
        pageView->addEventListener(CC_CALLBACK_2(LevelSelect::pageViewEvent, this));
        character_back->addChild(pageView, 1, key_charactor_pageview);
        
        auto btn_up = Button::create("level/arrow_up.png");
        btn_up->setPosition(Vec2(size.width / 2,
                                 size.height + btn_up->getContentSize().height / 2 + 5));
        character_back->addChild(btn_up);
        
        
        auto btn_down = Button::create("level/arrow_down.png");
        btn_down->setPosition(Vec2(size.width / 2,
                                   - btn_down->getContentSize().height / 2 - 5));
        character_back->addChild(btn_down);
}
void LevelSelect::initColorSel(Vec2 position_num, Size num_size){
        //颜色选择
        _curColorIdx = 0;
        auto chose_color_back = Sprite::create("level/chose_color_bck.png");
        chose_color_back->setPosition(Vec2(position_num.x +
                                           num_size.width / 2 + 5
                                           + chose_color_back->getContentSize().width / 2,
                                           position_num.y));
        auto color_size = chose_color_back->getContentSize();
        this->addChild(chose_color_back, ZORDER_BACK_LAYERS);
        
        
        
        PageView* color_list = PageView::create();
        color_list->setDirection(ui::PageView::Direction::VERTICAL);
        color_list->setContentSize(color_size);
        color_list->removeAllItems();
        int color_num = 8;
        float width = 0;
        for (int i = 0; i < color_num; i++){
                std::string name = StringUtils::format("level/player_color_%d.png", i);
                auto color = ImageView::create(name);
                width = color->getContentSize().width;
                color_list->insertCustomItem(color, i);
        }
        
        color_list->setContentSize(Size(width, color_size.height));
        color_list->setCurrentPageIndex(0);
        
        color_list->setTag(2);
        color_list->setIndicatorEnabled(true);
        color_list->addEventListener(CC_CALLBACK_2(LevelSelect::pageViewEvent, this));
        color_list->setPosition(Vec2((color_size - color_list->getContentSize()) / 2));
        chose_color_back->addChild(color_list);
        
        auto color_up = Button::create("level/arrow_up.png");
        color_up->setPosition(Vec2(color_size.width / 2,
                                   color_size.height + color_up->getContentSize().height / 2 + 5));
        
        chose_color_back->addChild(color_up, ZORDER_ITEM_CONTROL);
        
        auto color_down = Button::create("level/arrow_down.png");
        color_down->setPosition(Vec2(color_size.width / 2,
                                     - color_down->getContentSize().height / 2 - 5));
        chose_color_back->addChild(color_down, ZORDER_ITEM_CONTROL);
}

void LevelSelect::initWalls(Sprite* back_ground){
        auto chengqiang = Sprite::create("level/chengqiang.png");
        auto great_wall_size = chengqiang->getContentSize();
        chengqiang->setPosition(Vec2(great_wall_size / 2));
        
        back_ground->addChild(chengqiang, ZORDER_BACK_LAYERS, kMenuGreatWallTag);
}

void LevelSelect::initMainMenu(){
        
        auto visible_size = Director::getInstance()->getVisibleSize();
        Vec2 center = visible_size / 2;

        auto back_ground = Sprite::create("level/level_select_back.png");
        back_ground->setPosition(center);
        this->addChild(back_ground, ZORDER_BACK_GROUND, kMainMenuBackTag);
        
        
        auto position_num = Vec2(visible_size.width / 2, visible_size.height * 0.55);
        Size num_size = this->initCenterMainFrame(position_num);
        this->initCharactorSel(position_num, num_size);
        this->initColorSel(position_num, num_size);
        this->initWalls(back_ground);
        
}

void LevelSelect::initButtons(Vec2 origin, Size visibleSize){
        auto cache = UserDefault::getInstance();
        _soundTotalOn =cache->getBoolForKey(SOUND_MUSIC_TOTAL_KEY, true);
        if (_soundTotalOn){
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
        /*
         /////////////////////////////////////////////////////////
                        coins buttons
         /////////////////////////////////////////////////////////
         */
        
        auto coins_back = Sprite::create("level/coind_back.png");
        auto coins_back_size = coins_back->getContentSize();
        Vec2 coins_back_pos = Vec2(visibleSize  - coins_back_size);
        coins_back->setPosition(coins_back_pos);
        
        auto coins_show = Sprite::create("level/coins_show.png");
        auto coins_show_size = coins_show->getContentSize();
        Vec2 coins_pos = Vec2(coins_back_size.width,
                              coins_back_size.height * 0.5f);
        coins_show->setPosition(coins_pos);
        coins_back->addChild(coins_show);
        
        auto add_coins = ui::Button::create("level/add_icon.png");
        add_coins->addClickEventListener(CC_CALLBACK_1(LevelSelect::menuGetMoreCoins, this));
        add_coins->setPosition(Vec2(0, coins_back_size.height * 0.5f));
        coins_back->addChild(add_coins);
        
        _coinsNumLb = Label::createWithSystemFont("0", "fonts/arial.ttf", 32);
        _coinsNumLb->setPosition(coins_back_size * 0.5f);
        coins_back->addChild(_coinsNumLb);
        
        this->addChild(coins_back, ZORDER_ITEM_CONTROL);
        
        
        /*
         /////////////////////////////////////////////////////////
                        dices buttons
         /////////////////////////////////////////////////////////
         */
        
        auto arm_back = Sprite::create("level/coind_back.png");
        Vec2 arm_back_pos = Vec2(coins_back_pos.x - coins_back_size.width * 1.4f , coins_back_pos.y);
        auto arm_back_size = arm_back->getContentSize();
        arm_back->setPosition(arm_back_pos);
        
        auto arm_show = MenuItemImage::create("level/dice_show.png", "");
        auto arm_show_size = arm_show->getContentSize();
        Vec2 arm_pos = Vec2(arm_back_size.width,
                            arm_back_size.height * 0.5f);
        arm_show->setPosition(arm_pos);
        arm_back->addChild(arm_show);
        
        _mercenAriesNumLb = Label::createWithSystemFont("0", "fonts/arial.ttf", 32);//
        _mercenAriesNumLb->setPosition(arm_back_size * 0.5f);
        arm_back->addChild(_mercenAriesNumLb);
        
        auto add_arm = ui::Button::create("level/add_icon.png");
        add_arm->addClickEventListener(CC_CALLBACK_1(LevelSelect::menuGetMoreDices, this));
        Vec2 add_arm_pos(Vec2(0, arm_back_size.height * 0.5f));
        add_arm->setPosition(add_arm_pos);
        arm_back->addChild(add_arm);
        
        this->addChild(arm_back, ZORDER_ITEM_CONTROL);
        
        
        
        _achievementCtrl = MenuItemImage::create("achievement.png", "achievement_sel.png", CC_CALLBACK_1(LevelSelect::menuShowAchievement, this));
        _achievementCtrl->setPosition(Vec2(_achievementCtrl->getContentSize().width ,system_setting->getPosition().y));
        
        auto cup_tips = Sprite::create("game_win_shine.png");
        cup_tips->setPosition(_achievementCtrl->getContentSize() * 0.75);
        cup_tips->setScale(0.2f);
        cup_tips->runAction(RepeatForever::create(RotateBy::create(2, 360)));
        _achievementCtrl->addChild(cup_tips, 1, key_cup_tips);
        
        auto online_game = MenuItemImage::create("online_battle.png", "online_battle_sel.png",
                                                 CC_CALLBACK_1(LevelSelect::menuOnlineBattle, this));
        online_game->setPosition(Vec2(online_game->getContentSize().width / 2,
                                      coins_back_pos.y));
        
        
        auto menu = Menu::create(_soundCtrl, system_setting, start_game,
                                 _achievementCtrl, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, ZORDER_ITEM_CONTROL);
}

#pragma mark - button action callback
void LevelSelect::btnChosePlayerNum(Ref* btn, int num){
        if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
        Button* sel_btn = ((Button*)btn);
        _levelPlayerNum = num;
        _num_sel_back_grd->setPosition(sel_btn->getPosition());
        
        auto center_back = this->getChildByTag(kChoseNumBackTag);
        auto soldier_back = center_back->getChildByTag(kSoldierBackTag);
        soldier_back->removeAllChildren();
        auto back_size = soldier_back->getContentSize();
        
        for (int i = 0; i < num; i++){
                std::string str = StringUtils::format("level/ch_player_samll_%d.png", i);
                auto empty_chara = ImageView::create(str);
                auto empty_chara_size = empty_chara->getContentSize();
                int  sign = (i % 2 == 0) ? -1 * (i + 1) : i;
                auto pos = Vec2(back_size.width / 2 + 0.5 * sign
                                * empty_chara_size.width,
                                empty_chara_size.height * 0.6);
                empty_chara->setPosition(pos);
                soldier_back->addChild(empty_chara);
        }
}
void LevelSelect::pageViewEvent(Ref *pSender, PageView::EventType type)
{
        if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
        PageView* pageView = dynamic_cast<PageView*>(pSender);
        
        CCLOGWARN("log:%zd tag(%d)", pageView->getCurrentPageIndex(), pageView->getTag());
        if (key_charactor_pageview == pageView->getTag()){
                _curChIdx = (int)pageView->getCurrentPageIndex();
                AchievementEngine::getInstance()->openReward(ACHIEVE_DATA_KEY_FIRST_CHANGE_PLAYER);
        }else if (2 == pageView->getTag()){
                _curColorIdx = (int)pageView->getCurrentPageIndex();
                AchievementEngine::getInstance()->openReward(ACHIEVE_DATA_KEY_FIRST_CHANGE_COLOR);
        }
}

void LevelSelect::menuOnlineBattle(Ref* btn){
        if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
        Scene* scene = FindPlayer::createScene();
        Director::getInstance()->pushScene(scene);
}
void LevelSelect::menuStartGame(Ref* btn){
        if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
        bool char_lock_stat =  AchievementEngine::getInstance()->getCharLockStatus(_curChIdx + 1);
        if (false == char_lock_stat){
                auto page_view = (PageView*)this->getChildByTag(key_charactor_back)->getChildByTag(key_charactor_pageview);
                auto charctor_img = page_view->getItem(_curChIdx);
                auto lock = charctor_img->getChildByTag(key_charactor_lock);
                int  price =  AchievementEngine::getInstance()->getCharUnlockPrice(_curChIdx);
                if (nullptr != lock){
                        
                        this->actionBuyCharacter(lock, _curChIdx, price);
                }
                
                return;
        }
        
        auto scene = GameScene::createScene(_levelPlayerNum, _curChIdx, _curColorIdx);
        Director::getInstance()->pushScene(scene);
}

void LevelSelect::menuShowSettigns(Ref* btn){
        if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
        auto settings = GameSettings::create();
        this->addChild(settings, ZORDER_TOP_LEVEL_SHOW);
}


void LevelSelect::menuGetMoreCoins(Ref* btn){
        if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
        auto shop = Shopping::createScene();
        Director::getInstance()->pushScene(shop);
}

void LevelSelect::menuGetMoreDices(Ref* btn){
        if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
        auto buy_tips = BuySupply::createScene();
        Director::getInstance()->pushScene(buy_tips);
}

void LevelSelect::menuShowAchievement(Ref* btn){
        if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
        auto scene = Achievement::createScene();
        Director::getInstance()->pushScene(scene);
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
                
                AreaData* area = AreaData::create(i, game_data);
                area->retain();
                
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
        
        if (_soundTotalOn) _soundEngine->playEffect(EFFECT_FILE_SELECTED);
}


void LevelSelect::menuSoundControl(Ref* btn){
        auto cache = UserDefault::getInstance();
        _soundTotalOn = !_soundTotalOn;
        cache->setBoolForKey(SOUND_MUSIC_TOTAL_KEY, _soundTotalOn);
        
        if (!_soundTotalOn){
                _soundCtrl->setNormalImage(Sprite::create("Sound_off.png"));
                _soundCtrl->setSelectedImage(Sprite::create("Sound_off_sel.png"));
                _soundEngine->pauseBackgroundMusic();
                _soundEngine->pauseAllEffects();
        }else{
                _soundCtrl->setNormalImage(Sprite::create("Sound_on.png"));
                _soundCtrl->setSelectedImage(Sprite::create("Sound_on_sel.png"));
                
                _soundEngine->playEffect(EFFECT_FILE_SELECTED);
                
                if (_soundSwitch){
                        float s_v = 0.01f * cache->getIntegerForKey(SOUND_EFFECT_VALUE_KEY);
                        _soundEngine->setEffectsVolume(s_v);
                        _soundEngine->resumeAllEffects();
                }
                
                if (_musicSwitch){
                        float m_v = 0.01f * cache->getIntegerForKey(BACK_MUSIC_VALUE_KEY);
                        _soundEngine->setBackgroundMusicVolume(m_v);
                        _soundEngine->resumeBackgroundMusic();
                }
        }
        
        cache->flush();
}


void LevelSelect::actionBuyCharacter(Ref*btn, int ch_idx, int price){
        auto cache = UserDefault::getInstance();
        
        if (price < 0){
                CommonTipsDialog::showModalDialog(this,"This role is not for sale , you need to win it from battle.");
                return;
        }
        
        int coins_no = cache->getIntegerForKey(USER_CURRENT_COINS, USER_DEFAULT_COINS_ONFIRST);
        if (coins_no < price){
                auto shop = Shopping::createScene();
                Director::getInstance()->pushScene(shop);
        }else{
                coins_no -= price;
                CommonTipsDialog::showModalDialog(this,"Do you want to buy this character?", [this, btn, coins_no, ch_idx](Ref*){
                        
                        auto v_size = Director::getInstance()->getVisibleSize();
                        
                        std::string lock_name = AchievementEngine::getInstance()->getLockName(ch_idx + 1, 0);
                        
                        
                        
                        auto cache = UserDefault::getInstance();
                        cache->setBoolForKey(lock_name.c_str(), true);
                        
                        cache->setIntegerForKey(USER_CURRENT_COINS, coins_no);
                        _coinsNumLb->setString(tostr(coins_no));
                        cache->flush();
                        
                        std::string img_path = AchievementEngine::getInstance()->getCharactorImg(lock_name);
                        auto character = Sprite::create(img_path);
                        auto ch_size = character->getContentSize();
                        character->setPosition(ch_size * 0.5f);
                        
                        auto cha_layer = Layer::create();
                        cha_layer->setContentSize(ch_size);
                        cha_layer->addChild(character, 2);
                        
                        auto shining_back = Sprite::create("game_win_shine.png");
                        shining_back->setPosition(ch_size * 0.5f);
                        shining_back->runAction(RepeatForever::create(RotateBy::create(2.0f, 360)));
                        cha_layer->addChild(shining_back, 1);
                        
                        auto cale_by = ScaleBy::create(1.5f, 3.f);
                        
                        auto btn_flag = (ui::Button*)btn;
                        
                        auto call_back = CallFunc::create([this, cha_layer, btn_flag](){
                                
                                cha_layer->removeFromParentAndCleanup(true);
                                btn_flag->removeFromParentAndCleanup(true);
                        });
                        
                        auto scale_by = ScaleBy::create(0.75f, 1.6f);
                        _coinsNumLb->runAction(Sequence::create(scale_by, scale_by->reverse(), NULL));
                        cha_layer->runAction(Sequence::create(cale_by,
                                                              DelayTime::create(0.3f),
                                                              call_back, NULL));
                        
                        cha_layer->setPosition((v_size - cha_layer->getContentSize()) * 0.5f);
                        this->addChild(cha_layer, SUPER_LAYER_PRIVILIEGE);
                });
        }
}

#pragma mark - loading bar

void LevelSelect::onEnter(){
        Layer::onEnter();
        auto data_cache = UserDefault::getInstance();
        auto back_layer = this->getChildByTag(kMainMenuBackTag);
        auto chengqiang = back_layer->getChildByTag(kMenuGreatWallTag);
        auto great_wall_size = chengqiang->getContentSize();
        int running_p_no = random(2, 4);
        auto cache = AnimationCache::getInstance();
        for (int i = 0; i < running_p_no; i++){
                int p_i_r = random(0, MAX_PLAYER - 1);
                std::string name = StringUtils::format("level/ch_player_%d.png", p_i_r);
                auto runner = Sprite::create(name);
                Vec2 gap = great_wall_size * 0.1;
                auto runner_size = runner->getContentSize();
                bool is_odd = i % 2 == 1;
                chengqiang->addChild(runner);
                
                auto run_anim = cache->getAnimation(ANIM_NAME_FIGHT_RUN[p_i_r])->clone();                
                run_anim->setRestoreOriginalFrame(true);
                run_anim->setLoops(8);
                float scale = (1 - 0.1 * i);
                auto run_action = Animate::create(run_anim);
                
                if (is_odd){
                        runner->setPosition(Vec2(great_wall_size.width + gap.x * i, gap.y * i + runner_size.height / 6));
                        auto move =  MoveBy::create(4.0f, -Vec2(scale * great_wall_size.width * 1.6, 0));
                        Spawn* p_run = Spawn::create(run_action, FlipX::create(true), move, NULL);
                        Spawn* p_run_back = Spawn::create(run_action, FlipX::create(false) , move->reverse(), NULL);
                        runner->runAction(RepeatForever::create(Sequence::create(p_run, p_run_back, NULL)));
                        
                        runner->setScale(1.4);
                }else{
                        runner->setPosition(Vec2(-gap.x * i, gap.y * i + runner_size.height / 6));
                        auto move =  MoveBy::create(4.0f, Vec2(scale * great_wall_size.width * 1.6, 0));
                        Spawn* p_run = Spawn::create(run_action, FlipX::create(false), move, NULL);
                        Spawn* p_run_back = Spawn::create(run_action, FlipX::create(true) , move->reverse(), NULL);
                        runner->runAction(RepeatForever::create( Sequence::create(p_run, p_run_back, NULL)));
                        
                        runner->setScale(1.4);
                }
        }
        
        int coins_no = UserDefault::getInstance()->getIntegerForKey(USER_CURRENT_COINS, USER_DEFAULT_COINS_ONFIRST);
        _coinsNumLb->setString(tostr(coins_no));
        
        
        int mercenary_no = UserDefault::getInstance() ->getIntegerForKey(USER_CURRENT_SUPPLY_NO, USER_DEFAULT_SUPPLYNO_ONFIRST);
        _mercenAriesNumLb->setString(tostr(mercenary_no));
        
        _soundEngine = CocosDenshion::SimpleAudioEngine::getInstance();
        _soundEngine->playBackgroundMusic(BACK_MUSIC_LEVEL_SELECT, true);
        
        _soundTotalOn = data_cache->getBoolForKey(SOUND_MUSIC_TOTAL_KEY, true);
        _soundSwitch = data_cache->getBoolForKey(SOUND_EFFECT_SWITCH_KEY, true);
        _musicSwitch = data_cache->getBoolForKey(BACK_MUSIC_SWITCH_KEY, true);
        
        if (!_soundTotalOn){
                _soundEngine->pauseAllEffects();
                _soundEngine->pauseBackgroundMusic();
        }else {
                if (_soundSwitch){
                        float s_v = 0.01f * data_cache->getIntegerForKey(SOUND_EFFECT_VALUE_KEY, 50);
                        _soundEngine->setEffectsVolume(s_v);
                }else{
                        _soundEngine->pauseAllEffects();
                }
                
                
                if (_musicSwitch){
                        float m_v = 0.01f * data_cache->getIntegerForKey(BACK_MUSIC_VALUE_KEY, 50);
                        _soundEngine->setBackgroundMusicVolume(m_v);
                }else{
                        _soundEngine->pauseBackgroundMusic();
                }
        }
        
        
        auto cup_tips = (Sprite*)_achievementCtrl->getChildByTag(key_cup_tips);
        int new_cup_num = data_cache->getIntegerForKey(ACHIEVE_DATA_KEY_NEW_ACH_NO, 0);
        if (new_cup_num > 0){
                cup_tips->setVisible(true);
        }else{
                cup_tips->setVisible(false);
        }
        
        auto back_grd = this->getChildByTag(kChoseNumBackTag);
        for (int i = 2; i <= MAX_PLAYER; i++){
                auto but_item = (ui::Button*)back_grd->getChildByTag(MODAL_DIALOG_NODETAG + i);
                bool lock_status =  AchievementEngine::getInstance()->getLevelLockStatus(i);
                if (false == lock_status){
                        but_item->setEnabled(false);
                        but_item->setBright(false);
                        but_item->loadTextureNormal("target_lock.png");
                }else{
                        but_item->setEnabled(true);
                        but_item->setBright(true);
                        but_item->loadTextureNormal("level/sel_num_btn_back_em.png");
                }
        }
        
        auto page_view = (PageView*)this->getChildByTag(key_charactor_back)->getChildByTag(key_charactor_pageview);
        for (int i = 0; i < MAX_PLAYER; i++){
                bool char_lock_stat =  AchievementEngine::getInstance()->getCharLockStatus(i + 1);
                if (char_lock_stat){
                        page_view->getItem(i)->removeAllChildren();
                }
        }
        
}

void LevelSelect::update(float delta){
}

void LevelSelect::showDailyRewards(){
        auto show_layer = this->getChildByTag(kChoseNumBackTag);
        auto visible_size = Director::getInstance()->getVisibleSize();
        auto world_p = _coinsNumLb->getParent()->convertToWorldSpace(_coinsNumLb->getPosition());
        
        auto call_back = CallFunc::create([this](){
                auto scale_by = ScaleBy::create(0.4f, 1.4f);
                _coinsNumLb->getParent()->runAction(Sequence::create(scale_by, scale_by->reverse(), NULL));
                int cur_coins = UserDefault::getInstance()->getIntegerForKey(USER_CURRENT_COINS);
                _coinsNumLb->setString(StringUtils::format("%d", cur_coins));
        });
        call_back->retain();
        auto dest = show_layer->convertToNodeSpace(world_p);
        AchievementEngine::getInstance()->dailyOpenReward(show_layer, visible_size * 0.5f, dest, call_back);
}


void LevelSelect::onExit(){
        Layer::onExit();
        _loadingBar->setVisible(false);
        _loadingBarBack->setVisible(false);
        _count = 0;
        unscheduleUpdate();
        AsyncTaskPool::getInstance()->stopTasks(AsyncTaskPool::TaskType::TASK_IO);
        
        auto back_layer = this->getChildByTag(kMainMenuBackTag);
        auto the_wall = back_layer->getChildByTag(kMenuGreatWallTag);
        the_wall->removeAllChildren();
}

